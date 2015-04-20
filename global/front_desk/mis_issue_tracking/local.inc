<?

function get_current_fixed($issue_id) {
	if ($issue_id!="") $issue_id=addslashes($issue_id);
	$issue_info=getoneb("select * from mis_issue_history where issue_id = '$issue_id' and status = 'current'");
	if ($issue_info) return $issue_info;
	@mysql_query("update mis_issue_history set status = 'updated' where issue_id = '$issue_id'");
	@mysql_query("update mis_issue_history set status = 'current' where issue_id = '$issue_id' order by issue_history_id desc limit 1");
	$issue_info=getone("select * from mis_issue_history where issue_id = '$issue_id' and status = 'current'");	
	return $issue_info;
	}

function change_action ($issue_id,$note_text) {
	global $global_user_id,$global_link_id;
	$today_mysql=date('Y-m-d');
	if ($note_text=="") {
		echo "<p>Warning: add_note to issue failed. *Note is null*.<p>";
		return 1;
		}
	//echo "select * from mis_issue_index where issue_id = '$issue_id'";
	$issue_info=getone("select * from mis_issue_index where issue_id = '$issue_id'");
	//$history_info=getone("select * from mis_issue_history where issue_id = '$issue_info->issue_id' and current_hist_item = 'Y'");
	$history_info=get_current_fixed($issue_id);
		$history_info->action_required=addslashes($note_text);
		$history_info->it_notes=addslashes($history_info->it_notes);
	
	$query1="update mis_issue_history set current_hist_item = 'N',status = 'updated', actual_completion_date = '$today_mysql' where issue_id = '$issue_info->issue_id' and current_hist_item = 'Y' and issue_history_id = '$history_info->issue_history_id'";
	$query2="update mis_issue_history set current_hist_item = 'N',status = 'updated' where issue_id = '$issue_info->issue_id'";
	$query3="insert into mis_issue_history set
	issue_id = '$issue_info->issue_id',
	action_required = '$history_info->action_required',
	notes = '** Next Action Changed **',
	it_notes = '$history_info->it_notes',
	status = '$history_info->status',
	expected_completion_date = '$history_info->expected_completion_date',
	actual_completion_date = '$history_info->actual_completion_date',
	us_or_them = '$history_info->us_or_them',
	current_hist_item = 'Y',
	email_sent = 'N',
	entered_by = '$global_user_id'";

	//echo "<hr>$query1<hr>$query2<hr>";
	@mysql_query($query1);
	@mysql_query($query2);
	@mysql_query($query3);

	$last_insert_id=@mysql_insert_id($global_link_id);
	
	$c_qry="select * from mis_issue_concerned_parties where issue_history_id = '$history_info->issue_history_id'";
	$c_res=@mysql_query($c_qry);
	while($c_row=@mysql_fetch_object($c_res)) {
		@mysql_query("insert into mis_issue_concerned_parties set
		issue_history_id = '$last_insert_id',
		contacts_id = '$c_row->contacts_id',
		us_or_them = '$c_row->us_or_them'");
		}
	return 0;	
	}

function add_note ($issue_id,$note_text,$next_action=1) {
	global $global_user_id,$global_link_id;
	$today_mysql=date('Y-m-d');
	if ($note_text!="") {
		$note_text=addslashes($note_text);
		} else {
		echo "<p>Warning: add_note to issue failed. *Note is null*.<p>";
		return 1;
		}
	//echo "select * from mis_issue_index where issue_id = '$issue_id'";
	$issue_info=getone("select * from mis_issue_index where issue_id = '$issue_id'");
	//$history_info=getone("select * from mis_issue_history where issue_id = '$issue_info->issue_id' and current_hist_item = 'Y'");
	$history_info=get_current_fixed($issue_info->issue_id);
		$history_info->action_required=addslashes($history_info->action_required);
		$history_info->it_notes=addslashes($history_info->it_notes);
	
	if ($next_action < 1) $history_info->action_required="";
	
	$query1="update mis_issue_history set current_hist_item = 'N',status = 'updated', actual_completion_date = '$today_mysql' where issue_id = '$issue_info->issue_id' and current_hist_item = 'Y' and issue_history_id = '$history_info->issue_history_id'";
	$query2="update mis_issue_history set current_hist_item = 'N',status = 'updated' where issue_id = '$issue_info->issue_id'";
	$query3="insert into mis_issue_history set
	issue_id = '$issue_info->issue_id',
	action_required = '$history_info->action_required',
	notes = '$note_text',
	it_notes = '$history_info->it_notes',
	status = '$history_info->status',
	expected_completion_date = '$history_info->expected_completion_date',
	actual_completion_date = '$history_info->actual_completion_date',
	us_or_them = '$history_info->us_or_them',
	current_hist_item = 'Y',
	email_sent = 'N',
	entered_by = '$global_user_id'";

	//echo "<hr>$query1<hr>$query2<hr>";
	@mysql_query($query1);
	@mysql_query($query2);
	@mysql_query($query3);

	$last_insert_id=@mysql_insert_id($global_link_id);
	
	$c_qry="select * from mis_issue_concerned_parties where issue_history_id = '$history_info->issue_history_id'";
	$c_res=@mysql_query($c_qry);
	while($c_row=@mysql_fetch_object($c_res)) {
		@mysql_query("insert into mis_issue_concerned_parties set
		issue_history_id = '$last_insert_id',
		contacts_id = '$c_row->contacts_id',
		us_or_them = '$c_row->us_or_them'");
		}
	return 0;	
	}

function category_show_last_query() {
	global $global_user_id;
	$qry=getone("select * from mis_issue_query_history where contacts_id = '$global_user_id' order by ts desc limit 1");
	category_show($qry->query);
	}

function category_show_item($row,$colors2,$td_option="") {
global $category_show_item_toggle;
$today=date('Y-m-d');
	//$colors->color="#ffffff";
	//$colors->bc1="#ffffff";
	//$colors->bc2="#ffffff";
	//$colors->bc3="#ffffff";
	$colors=clone $colors2;
	$bc1=clone $colors->bc1;
	$bc2=clone $colors->bc2;
	$bc3=clone $colors->bc3;
	
	if ($row->reference_this_issue>0) {
		$row->status="superseded";
		}	

	#######################################
	#	Set weave pattern
	#######################################
	$creator_info=getone("select login_name from contacts where contacts_id = '$row->creator'");
	#######################################
	#	Set weave pattern
	#######################################
	if ($category_show_item_toggle!=1) {
		$category_show_item_toggle=1;$b1=$bc1;$b2=$bc2;
		} else {
		$category_show_item_toggle=0;$b1=$bc2;$b2=$bc3;
		}	
	#######################################
	#	Initialized color shift (blue)
	#######################################
	if($row->status=="initialized") {
		$b1=colorshift($b1,"yellow",3);
		$b2=colorshift($b2,"yellow",3);
		}
	#######################################
	#	Active/current settings
	#######################################
	
	if(($row->status=="active")||($row->status=="current")) {
		if (($row->expected_completion!="")&&($row->expected_completion <= $today)) {
			$b1=colorshift($b1,"red",2);
			$b2=colorshift($b2,"red",2);
			}
		#######################################
		#	Current Project is blue shifted
		#######################################
		if ($row->status=="current") {
			$b1=colorshift($b1,"blue",2);
			$b2=colorshift($b2,"blue",2);
			}
		}
	#######################################
	#	Completed item is just shaded darker
	#######################################
	if(($row->status=="complete")||($row->status=="superseded")) {
		$b1=colorshift($b1,"darker",4);
		$b2=colorshift($b2,"darker",4);
		}
	#######################################
	#	Approved item shaded even darker
	#######################################
	if($row->status=="approved") {
		$b1=colorshift($b1,"darker",7);
		$b2=colorshift($b2,"darker",7);
		}
	$current_hist=getoneb("select * from mis_issue_history where issue_id = '$row->issue_id' and status = 'current'");
	if ($current_hist->us_or_them=="them") $b1=colorshift($b1,"desaturate",2);
	if ($current_hist->us_or_them=="them") $b2=colorshift($b2,"desaturate",2);
	if (($current_hist->expected_completion_date < $today)&&($current_hist->expected_completion_date!="")) $next_action_color=colorshift($b2,"red",6);
		else $next_action_color=$b2;
	$row->expected_completion=invali_date($row->expected_completion);
	$row->requested_completion=invali_date($row->requested_completion);
	
	$next_action_date=invali_date($current_hist->expected_completion_date);

	if ($current_hist->us_or_them=="them") {
		$list="";
		$listcount=0;
		$list_res=@mysql_query("select * from mis_issue_concerned_parties,contacts where mis_issue_concerned_parties.contacts_id = contacts.contacts_id and us_or_them = 'them' and issue_history_id = '$current_hist->issue_history_id'");
		while($listrow=@mysql_fetch_object($list_res)) {
			$list="$list $listrow->login_name";
			$listcount++;
			}
		if ($listcount <= 4) $ut_text="USER(S): $list";
		else $ut_text="USER(S): *GROUP*";
		}
	
	if ($current_hist->us_or_them=="us") {
		$list="";
		$listcount=0;
		$list_res=@mysql_query("select * from mis_issue_concerned_parties,contacts where mis_issue_concerned_parties.contacts_id = contacts.contacts_id and us_or_them = 'us' and issue_history_id = '$current_hist->issue_history_id'");
		while($listrow=@mysql_fetch_object($list_res)) {
			$list="$list $listrow->login_name";
			$listcount++;
			}
		if ($listcount <= 4) $ut_text="IT: $list";
		else $ut_text="IT: *GROUP*";
		}
	
	
	if ($current_hist->us_or_them=="") $ut_text="n/a";
	if ($row->status=="superseded") {
		$itemlink= "
		<font color=white>$row->issue_id</font>
		<a href=$pagename?mode=issue_edit&issue_id=$row->reference_this_issue><font color=blue>$row->reference_this_issue</font></a>
		";
		} else {
		$itemlink= "<a href=$pagename?mode=issue_edit&issue_id=$row->issue_id><font color=blue>$row->issue_id</font></a>";
		}
	if ($row->status=="initialized") $showstatus="new";
	else $showstatus=$row->status;

	//$b1->color="#ffffff";
	//$b2->color="#eeeeee";
	//$b3->color="#dddddd";
	
	echo "<tr><td bgcolor=$b1->color $td_option>
	$itemlink
	</td><td bgcolor=$b2->color $td_option>
		$showstatus
	</td><td bgcolor=$b1->color $td_option>
		$row->name
	</td><td bgcolor=$b2->color $td_option>
		$row->requested_completion
	</td><td bgcolor=$b1->color $td_option>
		$row->expected_completion
	</td><td bgcolor=$next_action_color->color $td_option>
		$next_action_date
	</td><td bgcolor=$b1->color $td_option>
		$creator_info->login_name
	</td><td bgcolor=$b2->color $td_option>
		$ut_text	
	</td></tr>
	";	
	if ($row->status=="superseded") {
		$newrow=getone("select * from mis_issue_index where issue_id = '$row->reference_this_issue'");
		category_show_item($newrow,$colors);
		}
}

function category_show($query,$alternate_title="",$userui=0,$saved_search="0",$repeat="0") {
	global $global_user_id;
	$today=date('Y-m-d');
	$res=@mysql_query($query);
	$rowcount=@mysql_num_rows($res);
	//echo "$query";
	
	$it_category_id=ereg_replace("^.* it_category_id *=","",$query);
	$it_category_id=ereg_replace("^[^0-9]*'","",$it_category_id);
	$it_category_id=ereg_replace("'.*$","",$it_category_id);
	$it_category_info=getoneb("select * from mis_issue_categories where it_category_id = '$it_category_id'");

	if($rowcount<1) {
		echo "<br><b><i>No Records Found...</i></b><br>";
		if ($userui!=1) {
			if ($saved_search=="0") echo "<a href=$pagename?mode=save_last_query><font color=blue>Save Search</font></a><br>";
			echo "<a href=$pagename?mode=show_query_history><font color=blue>History</font></a><br>";
		}
		echo"
		<a href=$pagename?mode=issue_new_item&it_category_id=$it_category_id><font color=blue>New Request</font></a><p>
		";
		} else {
		$today=date("Y-m-d");
		$colors->bc1=colorshift("#ffffff","darker",2);
		$colors->bc2=colorshift("#ffffff","darker",1);
		$colors->bc3=colorshift("#ffffff","darker",0);

		if ($alternate_title!="") {
			$table_title=$alternate_title;
			} else {
			$table_title=$it_category_info->category_name;
			}

		echo "
		<table border=0 cellspacing=0 cellpadding=0><tr><td align=center>
		<h2>$table_title</h2><p>
		<a href=$pagename?mode=issue_new_item&it_category_id=$it_category_id><font color=blue>New Request</font></a><p>
		
		";
			if($userui!=1) { 
		echo"</td></tr><tr><td align=right>";
		if ($saved_search=="0") {
			echo "<a href=$pagename?mode=save_last_query><font color=blue>Save Search</font></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
			}
		echo "<a href=$pagename?mode=show_query_history><font color=blue>Show Search History</font></a>";
		}

		echo "
		<table border=1 cellpadding=0 cellspacing=0>
		<tr><td>
		<table border=0 cellpadding=6 cellspacing=0>
		";


	//////////////////////////////////////////////////////////////////////
	//$colors->bc1=$bc1;
	//$colors->bc2=$bc2;
	//$colors->bc3=$bc3;
	$colorsb=clone $colors;
	category_show_header($colors);
	$colors=clone $colorsb;
	$linecount=0;
	//$same_date_count=0;
	$last_date="";
	while($row=@mysql_fetch_object($res)) {
		if (($repeat!=0) && ( ($linecount % $repeat) == 0 ) && ($linecount>0)) {
			$colorsb=clone $colors;
			category_show_header($colors);
			$colors=clone $colorsb;
			}
		if ($today < $row->expected_completion) {
			if ($row->expected_completion!=$last_date) {
				$td_option="style=\"border-top: solid 1px\"";
				} else { 
				$td_option="";
				}
			
			
			//echo "<tr><td>$row->expected_completion</td></tr>";
			$last_date=$row->expected_completion;
			}
			
		$linecount++;
		$colorsb=clone $colors;
		category_show_item($row,$colors,$td_option);
		$colors=clone $colorsb;
		}
	//////////////////////////////////////////////////////////////////////

	echo "</table></td></tr></table>";

	echo "</td></tr></table>";

	}
	
	if ($userui!=1) {
		$query=addslashes($query);
		$lastquery=getoneb("select * from mis_issue_query_history where contacts_id = '$global_user_id' and query = '$query' limit 1");
		if (!($lastquery)) {
			@mysql_query("insert into mis_issue_query_history set contacts_id = '$global_user_id',query='$query'");
			} else {
			@mysql_query("update mis_issue_query_history set ts = now() where query_id = '$lastquery->query_id'");
			}
		}
	}


function category_show_header ($colors) {
	//return;
	global $category_show_item_toggle;
	$bc1=$colors->bc1;
	$bc2=$colors->bc2;
	$bc3=$colors->bc3;

	if ($category_show_item_toggle!=1) {
		$category_show_item_toggle=1;$b1=$bc1;$b2=$bc2;
	} else {
		$category_show_item_toggle=0;$b1=$bc2;$b2=$bc3;
	}
	            
	echo "
	<tr><td bgcolor=$b1->color style=\"border: solid 1pt\">
		<font size=+1 style=bold>Issue #</font>
	</td><td bgcolor=$b2->color style=\"border: solid 1pt\">
		<font size=+1 style=bold>Status</font>
	</td><td bgcolor=$b1->color style=\"border: solid 1pt\">
		<font size=+1 style=bold>Subject</font>
	</td><td bgcolor=$b2->color style=\"border: solid 1pt\">
		<font size=+1 style=bold>Requested Completion</font>
	</td><td bgcolor=$b1->color style=\"border: solid 1pt\">
		<font size=+1 style=bold>Expected Completion</font>
	</td><td bgcolor=$b2->color style=\"border: solid 1pt\">
		<font size=+1 style=bold>Next Action Date</font>
	</td><td bgcolor=$b1->color style=\"border: solid 1pt\">
		<font size=+1 style=bold>Creator</font>
	</td><td bgcolor=$b2->color style=\"border: solid 1pt\">
		<font size=+1 style=bold>Who's Court</font>
	</td></tr>
	";
	}

function weekday_advance($date,$days=1) {
	$date=vali_date($date);
	$count=0;
	
	$unix_date=@strtotime("$date");
	while($count<$days) {
		$unix_date=strtotime("+1 days",$unix_date);
		while (!((date('D',$unix_date)!='Sat')&&(date('D',$unix_date)!='Sun'))) {
			$unix_date=strtotime("+1 days",$unix_date);
			}
		$count++;
		}	
	return date('m-d-Y',$unix_date);	
	}

function issue_history_print($issue_history_object,$user_interface=0) {
	global $issue_history_print_toggle;
	
	$bgcolor->color="#ffffff";

	if ($issue_history_print_toggle==1) {
		$issue_history_print_toggle=0;
		$bgcolor=colorshift($bgcolor,"darker",1);
		} else {
		$issue_history_print_toggle=1;
		$bgcolor=colorshift($bgcolor,"darker",0);
		}
		
	if ($issue_history_object->us_or_them=="us") {
		$usorthem="IT";
		$bgcolor=colorshift($bgcolor,"blue",4);
		$bgcolor=colorshift($bgcolor,"darker",2);
		} else {
		$bgcolor=colorshift($bgcolor,"green",5);
		$bgcolor=colorshift($bgcolor,"red",2);
		$bgcolor=colorshift($bgcolor,"darker",2);
		}
	
	if ($issue_history_object->status!='current') {
		if ($issue_history_print_toggle==1) {
			$bgcolor=colorshift("","darker",2);
			} else {
			$bgcolor=colorshift("","darker",4);
			}			
		}
	else {
		$today=date('Y-m-d');
		if ($today > $issue_history_object->expected_completion_date) $bgcolor=colorshift($bgcolor,"red",4);
		}

	if ($issue_history_object->expected_completion_date < $issue_history_object->actual_completion_date) $bgcolor=colorshift($bgcolor,"red",4);
	$issue_history_object->expected_completion_date=invali_date($issue_history_object->expected_completion_date);

	if ($issue_history_object->us_or_them=="them") $usorthem="USER";

	$issue_history_object->actual_completion_date=invali_date($issue_history_object->actual_completion_date);

	echo "<tr bgcolor=$bgcolor->color><td>
		$issue_history_object->action_required
	</td><td>
		$issue_history_object->notes
	</td><td>
		";if ($user_interface==0) echo "$issue_history_object->it_notes
	</td><td>";echo"
		$issue_history_object->status
	</td><td>
		$issue_history_object->expected_completion_date
	</td><td>
		$issue_history_object->actual_completion_date
	</td><td>
		$usorthem
	</td><td>";
		$query="select login_name from contacts,mis_issue_concerned_parties where contacts.contacts_id = mis_issue_concerned_parties.contacts_id and issue_history_id = '$issue_history_object->issue_history_id' and us_or_them = 'them' order by login_name";
		$ulistres=@mysql_query($query);
		$rowcount=mysql_num_rows($ulistres);
		if ($rowcount>3) {
			$more="";
			while($urow=@mysql_fetch_object($ulistres)) {
				$userlist="$userlist$more$urow->login_name";
				$more="-";
				}
			echo "<a href=javascript:alert('$userlist')>List</a>";
			} else {
			while($urow=@mysql_fetch_object($ulistres)) echo "$urow->login_name<br>";
			}		
	
	
	echo "
	</td><td>";
		$query="select login_name from contacts,mis_issue_concerned_parties where contacts.contacts_id = mis_issue_concerned_parties.contacts_id and issue_history_id = '$issue_history_object->issue_history_id' and us_or_them = 'us' order by login_name";
		$ulistres=@mysql_query($query);
		$rowcount=mysql_num_rows($ulistres);
		if ($rowcount>3) {
			$more="";
			while($urow=@mysql_fetch_object($ulistres)) {
				$userlist="$userlist$more$urow->login_name";
				$more="-";
				}
			echo "<a href=javascript:alert('$userlist')>List</a>";
			} else {
			while($urow=@mysql_fetch_object($ulistres)) echo "$urow->login_name<br>";
			}		
			echo "
	</td></tr>";
	}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
//
//	COLOR Shifting / Parsing functions
//
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////



function csub($letter,$count=1) {
	$letter=substr($letter,0,1);
	while($count>0) {
		switch($letter){
			case ("0"): $letter="0";break;
			case ("1"): $letter="0";break;
			case ("2"): $letter="1";break;
			case ("3"): $letter="2";break;
			case ("4"): $letter="3";break;
			case ("5"): $letter="4";break;
			case ("6"): $letter="5";break;
			case ("7"): $letter="6";break;
			case ("8"): $letter="7";break;
			case ("9"): $letter="8";break;
			case ("a"): $letter="9";break;
			case ("b"): $letter="a";break;
			case ("c"): $letter="b";break;
			case ("d"): $letter="c";break;
			case ("e"): $letter="d";break;
			case ("f"): $letter="e";break;
			default:	$letter="f";break;
			}
		$count=$count - 1;
		}
	return($letter . $letter);
	}

function cadd($letter,$count=1) {
	$letter=substr($letter,0,1);
	while($count>0) {
		switch($letter){
			case ("0"): $letter="1";break;
			case ("1"): $letter="2";break;
			case ("2"): $letter="3";break;
			case ("3"): $letter="4";break;
			case ("4"): $letter="5";break;
			case ("5"): $letter="6";break;
			case ("6"): $letter="7";break;
			case ("7"): $letter="8";break;
			case ("8"): $letter="9";break;
			case ("9"): $letter="a";break;
			case ("a"): $letter="b";break;
			case ("b"): $letter="c";break;
			case ("c"): $letter="d";break;
			case ("d"): $letter="e";break;
			case ("e"): $letter="f";break;
			case ("f"): $letter="f";break;
			default:	$letter="f";break;
			}
		$count=$count - 1;
		}
	return($letter . $letter);
	}

function colorshift($color_pointer="white",$param="darker",$scale=0) {
	if (!($color_pointer->red)) $color_pointer=colordistill($color_pointer);
	switch($param) {
		case ("red"):	$color_pointer->green=csub($color_pointer->green,$scale);
						$color_pointer->blue=csub($color_pointer->blue,$scale);
						break;
		case ("green"):	
						$color_pointer->red=csub($color_pointer->red,$scale);
						$color_pointer->blue=csub($color_pointer->blue,$scale);
						break;
		case ("blue"):	
						$color_pointer->red=csub($color_pointer->red,$scale);
						$color_pointer->green=csub($color_pointer->green,$scale);
						break;
		case ("yellow"):	
						$color_pointer->blue=csub($color_pointer->blue,$scale);
							break;
		case ("darker"):	
						$color_pointer->red=csub($color_pointer->red,$scale);
						$color_pointer->green=csub($color_pointer->green,$scale);
						$color_pointer->blue=csub($color_pointer->blue,$scale);
						break;
		case ("desaturate"):	
						$color_pointer=color_desaturate($color_pointer,$scale);
						break;
		case ("lighter"):	$color_pointer->red=cadd($color_pointer->red,$scale);
							$color_pointer->green=cadd($color_pointer->green,$scale);
							$color_pointer->blue=cadd($color_pointer->blue,$scale);
							break; // Dangerous(ish)
		}
	return(colorfast($color_pointer));
	}

function color_desaturate($color_pointer,$scale=1) {
	$rednum=hexdec($color_pointer->red);
	$greennum=hexdec($color_pointer->green);
	$bluenum=hexdec($color_pointer->blue);
	
	//echo"<hr>red: $rednum<hr>green: $greennum<hr>blue: $bluenum<hr><p>";
	
	$total=$rednum + $greennum + $bluenum;
	$avg=$total/3;

	if ($rednum > $avg) {
		$diff=$rednum - $avg;
		//echo "<p><hr>red: $diff<hr><p>";
		if ($diff > $scale) $rednum=$rednum - $scale;
		else $rednum=$avg;
		} else {
		$diff=$avg - $rednum;
		//echo "<p><hr>red: $diff<hr><p>";
		if ($diff > $scale) $rednum=$rednum + $scale;
		else $rednum=$avg;
		}
		
	if ($greennum > $avg) {
		$diff=$greennum - $avg;
		//echo "<p><hr>green: $diff<hr><p>";
		if ($diff > $scale) $greennum=$greennum - $scale;
		else $greennum=$avg;
		} else {
		$diff=$avg - $greennum;
		//echo "<p><hr>green: $diff<hr><p>";
		if ($diff > $scale) $greennum=$greennum + $scale;
		else $greennum=$avg;
		}
		
	if ($bluenum > $avg) {
		$diff=$bluenum - $avg;
		//echo "<p><hr>blue: $diff<hr><p>";
		if ($diff > $scale) $bluenum=$bluenum - $scale;
		else $bluenum=$avg;
		} else {
		$diff=$avg - $bluenum;
		//echo "<p><hr>blue: $diff<hr><p>";
		if ($diff > $scale) $bluenum=$bluenum + $scale;
		else $bluenum=$avg;
		}
	
	$rednum=round($rednum);
	$greennum=round($greennum);
	$bluenum=round($bluenum);
			
	//$color_pointer->red=dechex($rednum) . dechex($rednum);
	//$color_pointer->green=dechex($greennum) . dechex($greennum);
	//$color_pointer->blue=dechex($bluenum) . dechex($bluenum);

	$color_pointer->red=dechex($rednum);
	$color_pointer->green=dechex($greennum);
	$color_pointer->blue=dechex($bluenum);

	//echo "<hr>red: $color_pointer->red<hr>green: $color_pointer->green<hr>blue: $color_pointer->blue<hr>";exit;
	return colorfast($color_pointer);
	}	

function colordistill($color="") {
	if ($color=="") $color="#ffffff";
	switch ($color) {
		case ("white"):	$color="#ffffff";break;
		case ("blue"):	$color="#0000ff";break;
		case ("red"):	$color="#ff0000";break;
		case ("green"):	$color="#00ff00";break;
		case ("brown"):	$color="#aa2222";break;
		case ("yellow"):	$color="#ffff00";break;
		case ("orange"):	$color="#ffaa00";break;
		case ("black"):	$color="#000000";break;
		}

	//if (is_object($color)) return $color;
	if (is_object($color)) $color="#ffffff";;
	$color=ereg_replace("#","",$color);
	$color=substr($color,0,6);
	$color_comp->red=$color[0] . $color[0];
	$color_comp->green=$color[2] . $color[2];
	$color_comp->blue=$color[4] . $color[4];
	return(colorfast($color_comp));
	}

function colorfast($color_pointer) {
	$color_pointer->color="#$color_pointer->red$color_pointer->green$color_pointer->blue";
	return ($color_pointer);
	}
?>
