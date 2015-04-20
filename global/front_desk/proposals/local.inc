<?

function percent_off_calc($bidlist_id) {
	//die("Percent off calc has been run!!!");
	$bidlist_id=addslashes($bidlist_id);
	$set_applies='Y';
	$bid_results_count=@mysql_num_rows(@mysql_query("select * from bidlist_results where bidlist_id = '$bidlist_id' and bid_amount > 0"));
	if ($bid_results_count<2) $set_applies='N';
	
	$our_bid_info=getoneb("select * from bidlist_results where bidlist_id = '$bidlist_id' and company = '968' order by winning_bid desc,bid_amount limit 1");
	if (!($our_bid_info)) $set_applies='N';
	$winning_bid_info=getoneb("select sum(bid_amount) as bid_amount from bidlist_results where bidlist_id = '$bidlist_id' and winning_bid = 'Y' and bid_amount > 0 and company > 0 group by winning_bid order by bid_amount desc limit 1");
	//tabledump("select * from bidlist_results where bidlist_id = '$bidlist_id' and winning_bid = 'Y'");
	//die("select * from bidlist_results where bidlist_id = '$bidlist_id' and winning_bid = 'Y'");
	//if (!($winning_bid_info)) die("crap");
	if (!($winning_bid_info)) $set_applies='N';
	
	if 	(($winning_bid_info->air_only=='Y') || ($winning_bid_info->hvac_only=='Y') || ($winning_bid_info->plumb_only=='Y'))
		$set_applies='C';
	
	if ($set_applies!='Y') {
		@mysql_query("update bidlist set percent_from_pace = '0', percent_from_pace_applies = '$set_applies' where bidlist_id = '$bidlist_id'");
		return FALSE;
		}
	
	if ($winning_bid_info->bidlist_results_id==$our_bid_info->bidlist_results_id) {
		$other_bid=getoneb("select * from bidlist_results where bidlist_id = '$bidlist_id' and company != '968' and bid_amount > 0 order by bid_amount limit 1");
		$off_by=$our_bid_info->bid_amount / $other_bid->bid_amount;
		$off_by=100 * ($off_by - 1);
		$off_by=round(($off_by * 1000))/1000;
		mysql_query("update bidlist set percent_from_pace_applies = 'Y', percent_from_pace = '$off_by' where bidlist_id = '$bidlist_id'");
		} else {
		$off_by=$our_bid_info->bid_amount / $winning_bid_info->bid_amount;
		$off_by=100 * ($off_by - 1);
		$off_by=round(($off_by * 1000))/1000;
		mysql_query("update bidlist set percent_from_pace_applies = 'Y', percent_from_pace = '$off_by' where bidlist_id = '$bidlist_id'");
		}
	
	
	
	
	
	////$bid_info=load_proposal_info($bidlist_id);
	
	//$bid_info->results_count=@mysql_num_rows(@mysql_query("select * from bidlist_results where bidlist_id = '$bid_info->bidlist_id'"));
	
	
	//$bid_info->winning_results_count=@mysql_num_rows(@mysql_query("select * from bidlist_results where bidlist_id = '$bid_info->bidlist_id' and winning_bid = 'Y'"));
	//$our_bid_info=getoneb("select * from bidlist_results where bidlist_id = '$bid_info->bidlist_id' and company = '968' order by winning_bid desc,bid_amount limit 1");
	//
	//$bid_info->umc_bid_won='?';
	//if ($bid_info->winning_results_count>0) $bid_info->umc_bid_won="N";
	//if ($our_bid_info->winning_bid=='Y') $bid_info->umc_bid_won='Y';
	
	//$bid_info->winning_results_count=@mysql_num_rows(@mysql_query("select * from bidlist_results where bidlist_id = '$bid_info->bidlist_id' and winning_bid = 'Y'"));
	
	
	
	
	
	
	
	
	}

function associate_bidlist_items($bidlist_id_1,$bidlist_id_2,$description="",$description_2="") {
	$bidlist_id_1=addslashes($bidlist_id_1);
	$bidlist_id_2=addslashes($bidlist_id_2);
	$description=addslashes($description);
	$description_2=addslashes($description_2);
	$b1_info=getoneb("select * from bidlist_associations where bidlist_id = '$bidlist_id_1' limit 1");
	$b2_info=getoneb("select * from bidlist_associations where bidlist_id = '$bidlist_id_2' limit 1");
	//if ($b1_info) {
	//	echo "<hr>found b1_info<hr>";
	//	}
	//if ($b2_info) {
	//	echo "<hr>found b2_info<hr>";
	//	}
	
	if ($b1_info) $mode="b1";
	if ($b2_info) $mode="b2";
	if (($b1_info)&&($b2_info)) $mode="both";
	if ((!($b1_info))&&(!($b2_info))) $mode="neither";
	
	if ($mode=="both") {
		if ($b1_info->bidlist_group==$b2_info->bidlist_group) {
			mysql_query("update bidlist_associations set description = '$description' where bidlist_id = '$bidlist_id_1'");
			if ($description_2!="")
				mysql_query("update bidlist_associations set description = '$description_2' where bidlist_id = '$bidlist_id_2'");
			return 0;
			}
		}
	//die($mode);
	switch ($mode) {
		
		case "b1":			
							mysql_query("insert into bidlist_associations set bidlist_id = '$bidlist_id_2', bidlist_group = '$b1_info->bidlist_group'");
							break;;
		case "b2":
							mysql_query("insert into bidlist_associations set bidlist_id = '$bidlist_id_1', bidlist_group = '$b2_info->bidlist_group'");
							break;;
		case "both":
							if ($b1_info->bidlist_group < $b2_info->bidlist_group) {
								$newgroup=$b1_info->bidlist_group;
								$gonegroup=$b2_info->bidlist_group;
								} else {
								$newgroup=$b2_info->bidlist_group;
								$gonegroup=$b1_info->bidlist_group;
								}
							mysql_query("update bidlist_associations set bidlist_group = '$newgroup' where bidlist_group = '$gonegroup'");
							break;;
		case "neither":
							$maxgrp=getone("select max(bidlist_group) as bidlist_group from bidlist_associations");
							if (($maxgrp->bidlist_group=="")||($maxgrp->bidlist_group==0)) {
								$maxgrp->bidlist_group=1;
								}
							$newgroup=$maxgrp->bidlist_group + 1;
							mysql_query("insert into bidlist_associations set bidlist_id = '$bidlist_id_1', bidlist_group = '$newgroup'");
							mysql_query("insert into bidlist_associations set bidlist_id = '$bidlist_id_2', bidlist_group = '$newgroup'");
							
							break;;
		default:
						die("ERROR: This shouldn't happen. associate_bidlist_items! Impossible mode detected!");
		
		}
	mysql_query("update bidlist_associations set description = '$description' where bidlist_id = '$bidlist_id_1'");
	if ($description_2!="")
		mysql_query("update bidlist_associations set description = '$description_2' where bidlist_id = '$bidlist_id_2'");
	bidlist_cache_update($bidlist_id_1);
	bidlist_cache_update($bidlist_id_2);
	return 0;
	}



function load_bid_info_for_job($jobinfo_id) {
	echo "$jobinfo_id";
	if ($test=getoneb("select bidlist_id from bidlist where jobinfo_id = '$jobinfo_id'")) return load_proposal_info($test->bidlist_id);
	else return false;
	}


function prop_side_menu ($title,$array_menu,$array_target) {
	if ($GLOBALS['prop_side_menu_loaded']!="1") {
		$GLOBALS['prop_side_menu_loaded']="1";
		echo "
	<script type=\"text/javascript\">

	var i=-135
	var intHide
	var speed=5

	function showmenu() {
		clearInterval(intHide)
		intShow=setInterval(\"show()\",10)
		}

	function hidemenu() {
		clearInterval(intShow)
		intHide=setInterval(\"hide()\",10)
		}

	function show() {
		if (i<-30) {
			i=i+speed
			document.getElementById('myMenu').style.left=i
			}
		}
	function hide() {
		if (i>-165) {
			i=i-speed
			document.getElementById('myMenu').style.left=i
			}
		}

	function show_hide_menu() {
	if (c==0) {
			c=1
			clearInterval(intHide)
			intShow=setInterval(\"show()\",10)
		} else {
			c=0
			clearInterval(intShow)
			intHide=setInterval(\"hide()\",10)
		}
	}

	</script>
	";
		}
	$title_real="";
	$length=strlen($title);
	$count=0;
	while($count<$length) {
		$title_real=$title_real . $title[$count] . "<br>";
		$count++;
		}
	echo "
	<style>
	

	table.smtab {
	background:#dddddd;
	position:relative;
	top:1px;
	left:-165px;
	}
	
	a.MenuText {
	text-decoration:none;font:bold;
	}
	
	</style>
	
	<div class=\"sidemenu\">
	<table border=2 class=\"smtab\" cellspacing=0 cellpadding=3 width=160 id=\"myMenu\" onmouseover=\"showmenu()\" onmouseout=\"hidemenu()\">
	";
	$array_size=sizeof($array_menu);
	
	$titlesent=false;
	while ($menu_item=@array_shift($array_menu)) {
		$menu_target=@array_shift($array_target);
		if ($titlesent==false) {
			$titlesent=true;
			echo "<tr bgcolor=darkblue onmouseover=\"this.style.backgroundColor='blue';\" onmouseout=\"this.style.backgroundColor='darkblue';\" onclick=\"document.location='$menu_target'\"><td>
					<b><font color=white>$menu_item</font></b>
				</td><td width=2 rowspan=\"$array_size\" align=\"center\" bgcolor=\"yellow\"><b>$title_real</b></td></tr>
				";
		} else {
			echo "<tr bgcolor=\"darkblue\" onmouseover=\"this.style.backgroundColor='blue';\"onmouseout=\"this.style.backgroundColor='darkblue';\" onclick=\"document.location='$menu_target'\"><td>
					<b><font color=white>$menu_item</font></b>
				</td></tr>
				";
		}				

		
	}	
	
	echo "

	</table>
	</div>
	";
	}




function list_projtypes($bidlist_id) {
	$query="select * from bidlist_projecttypes,bidlist_bid_projtypes where bidlist_id = '$bidlist_id' and bidlist_bid_projtypes.project_type_id = bidlist_projecttypes.project_type_id order by projgroup,project_type";
	$res=@mysql_query($query);
	$rowcount=@mysql_num_rows($res);
	echo "
	<script>
	function project_types_edit() {
		open('$pagename?mode=project_types_edit&bidlist_id=$bidlist_id','proj_type_win','height=500,width=500');
		}
	</script>
	<table border=1 cellspacing=0 cellpadeding=1>
	<tr><td rowspan=$rowcount><a href=javascript:project_types_edit();><font color=blue>Proj&nbsp;Type:</font></a>&nbsp;</td>";
	while($row=@mysql_fetch_object($res)) {
		echo "$trstart<td>$row->project_type</td></tr>";
		$trstart="<tr>";
		}
	echo "</table>";
	}

function list_projsubtypes($bidlist_id) {
	$query="select * from bidlist_projectsubtypes,bidlist_bid_projsubtypes where bidlist_id = '$bidlist_id' and bidlist_bid_projsubtypes.project_subtype_id = bidlist_projectsubtypes.project_subtype_id order by projgroup,project_subtype";
	$res=@mysql_query($query);
	$rowcount=@mysql_num_rows($res);
	echo "
	<script>
	function project_subtypes_edit() {
		open('$pagename?mode=project_subtypes_edit&bidlist_id=$bidlist_id','proj_subtype_win','height=500,width=500');
		}
	</script>
	<table border=1 cellspacing=0 cellpadeding=1>
	<tr><td rowspan=$rowcount><a href=javascript:project_subtypes_edit();><font color=blue>Proj&nbsp;Sub&nbsp;Type:</font></a>&nbsp;</td>";
	while($row=@mysql_fetch_object($res)) {
		echo "$trstart<td>$row->project_subtype</td></tr>";
		$trstart="<tr>";
		}
	echo "</table>";
	}


function show_bidlist_log ($bidlist_id,$type="",$description="All Comment Types",$limit='3') {
	global $table_dark;
	$query="select * from bidlist_log where bidlist_id = '$bidlist_id' order by bidlist_log_id desc";
	if ($type!="") $query="select * from bidlist_log where bidlist_id = '$bidlist_id' and type = '$type' order by bidlist_log_id desc";
	
	$res=@mysql_query($query);
	$total_count=@mysql_num_rows($res);
	$count=0;
	if ($total_count==0) return 0;
	
	echo "<table border=1 class=\"tableobj\" cellspacing=0 cellpadding=3><tr bgcolor=$table_dark><td colspan=3 align=center><b><font size=+1>$description&nbsp;</font></b></td></tr>";
	
	while($row=@mysql_fetch_object($res)) {
		$date=invali_date($row->created,'/');
		$contact=getoneb("select * from contacts where contacts_id = '$row->creator'");
		$row->comment=ereg_replace('','<p>',$row->comment);
		//echo "<tr><th>$date&nbsp;<i>$row->ctime</i></th><th width=100%><i>&nbsp;$contact->login_name</i></td></tr>
		//<tr><td colspan=2>$row->comment</tr>";
		echo "<tr><td>
			$date&nbsp;<i>$row->ctime</i>
		</td><td>
			<i>&nbsp;$contact->login_name</i>
		</td><td>
			$row->comment
		</td></tr>";
		
		
		$count++;
		if ($count>=$limit) {
			echo "<tr><td colspan=3><a target=bidlist_log_win href=$pagename?mode=bidlist_log_full&type=$type&bidlist_id=$bidlist_id&description=$description><font color=blue>Total: $total_count...</font></a></td></tr></table>";
			return 0;
			}
		}
	echo "</table>";
	}

$plcount=0;	

function bidlist_cache_update ($bidlist_id) {
	@mysql_query("delete from bidlist_cache where bidlist_id = '$bidlist_id'");
	$prop_info=real_load_proposal_info($bidlist_id);
	$prop_info_return=$prop_info;
	include('bl_cache_query_escape.phtml');
	include('bl_cache_query_insert.phtml');
	@mysql_query($query_insert);
	return $prop_info_return;
	}

function load_proposal_info($bidlist_id) {
	$pinfo=getoneb("select * from bidlist_cache where bidlist_id = '$bidlist_id'");
	if ($pinfo) {
		#####################################################
		# Decide what the default color should be (based
		# on the bid_date, and the current day)
		#####################################################
		$today_mysql=date('Y-m-d');
		if ($today_mysql > $pinfo->bid_date_mysql) 
			$pinfo->bid_date_color="#ffdddd";
		else $pinfo->bid_date_color="#ddffdd";
		if ($today_mysql == $pinfo->bid_date_mysql) 
			$pinfo->bid_date_color="yellow";
		if ($pinfo->status=='Lead') $pinfo->bid_date_color="#ccccff";
		#####################################################
		
		#####################################################
		# Try to determine some permission information
		#####################################################
		$current_user_id=$GLOBALS['global_user_id'];
		$adminuser=$GLOBALS['adminuser'];

		$pinfo->record_owner="false";
		$pinfo->record_maintainer="false";
		if ($pinfo->creator==$current_user_id) {
			$pinfo->record_owner="true";
			$pinfo->record_maintainer="true";
			}
		if ($pinfo->record_maintainer=="false") {
			$rm_test=getoneb("select * from bidlist_permissions where bidlist_id = '$pinfo->bidlist_id' and contact = '$current_user_id' limit 1");
			if ($rm_test) { $pinfo->record_maintainer="true"; }
			}

		if ($adminuser) $pinfo->record_maintainer="true";
		#####################################################
		
		$binfo=getone("select last_touched from bidlist where bidlist_id = '$bidlist_id'");
		if ($binfo->last_touched==$pinfo->last_touched) return $pinfo;
		else return bidlist_cache_update($bidlist_id);
	} else {
		return bidlist_cache_update($bidlist_id);
	}
	
	
	}

function real_load_proposal_info($bidlist_id) {
	$bid_info=getone("select * from bidlist where bidlist_id = '$bidlist_id'");
	
	////////////////////////////////////////////////////////////////////
	// Fix revision number (convert from old scheme to know
	// one if it's necessasary.. using letters as guide)
	////////////////////////////////////////////////////////////////////
	$orig_revision=$bid_info->proposal_revision;
	if ($bid_info->proposal_num > 0) {
		if ($bid_info->proposal_revision < 1) {
			$revisions=getoneb("select sum(1) as count from bidlist where proposal_num = '$bid_info->proposal_num'");
			if ($revisions->count > 1) {
				$revision_for_testing=$bid_info->revision;
				if ($revision_for_testing=="") {
					$max_rev=getoneb("select max(revision) as biggest_letter from bidlist where proposal_num = '$bid_info->proposal_num'");
					$newmax=$max_rev->biggest_letter;
					$newmax++;
					$revision_for_testing=$newmax;
					}
				switch ($revision_for_testing) {
					case 1:
					case "1":
					case "a": 	$bid_info->proposal_revision=0;
								break;;
					case "b": 	$bid_info->proposal_revision=1;
								break;;
					case "c": 	$bid_info->proposal_revision=2;
								break;;
					case "d": 	$bid_info->proposal_revision=3;
								break;;
					case "e": 	$bid_info->proposal_revision=4;
								break;;
					case "f": 	$bid_info->proposal_revision=5;
								break;;
					case "g": 	$bid_info->proposal_revision=6;
								break;;
					case "h": 	$bid_info->proposal_revision=7;
								break;;
					case "i": 	$bid_info->proposal_revision=8;
								break;;
					case "j": 	$bid_info->proposal_revision=9;
								break;;
					case "k": 	$bid_info->proposal_revision=10;
								break;;
					case "l": 	$bid_info->proposal_revision=11;
								break;;
					case "m": 	$bid_info->proposal_revision=12;
								break;;
					case "n": 	$bid_info->proposal_revision=13;
								break;;
					case "o": 	$bid_info->proposal_revision=14;
								break;;
					case "p": 	$bid_info->proposal_revision=15;
								break;;
					case "q": 	$bid_info->proposal_revision=16;
								break;;
					case "r": 	$bid_info->proposal_revision=17;
								break;;
					case "s": 	$bid_info->proposal_revision=18;
								break;;
					case "t": 	$bid_info->proposal_revision=19;
								break;;
					case "u": 	$bid_info->proposal_revision=20;
								break;;
					case "v": 	$bid_info->proposal_revision=21;
								break;;
					case "w": 	$bid_info->proposal_revision=22;
								break;;
					case "x": 	$bid_info->proposal_revision=23;
								break;;
					case "y": 	$bid_info->proposal_revision=24;
								break;;
					case "z": 	$bid_info->proposal_revision=25;
								break;;
					}
				}
			}
		}
	
	if ($bid_info->proposal_revision!=$orig_revision) {
		@mysql_query("update bidlist set proposal_revision = '$bid_info->proposal_revision' where bidlist_id = '$bid_info->bidlist_id'");
		}
	
	$bid_info->project=ereg_replace('"',"&quot;",$bid_info->project);	
	$bid_info->comments=ereg_replace('"',"&quot;",$bid_info->comments);	
	$today=date('m-d-Y');
	$today_mysql=date('Y-m-d');

	if ($bid_info->proposal_num <=1) $bid_info->prop_num_text="L#$bid_info->bidlist_id";
	else $bid_info->prop_num_text=$bid_info->proposal_num;
	
	$bid_info->bid_timetext=timebreak($bid_info->bid_time);
	$bid_info->prop_num=$bid_info->proposal_num;
	$bid_info->bid_datetext=invali_date($bid_info->bid_date);
	$bid_info->bid_date_mysql=$bid_info->bid_date;
	
	if ($today_mysql > $bid_info->bid_date) 
		$bid_info->bid_date_color="#ffdddd";
	else $bid_info->bid_date_color="#ddffdd";
	if ($today_mysql == $bid_info->bid_date) 
		$bid_info->bid_date_color="yellow";
	
	///////////////Do a little permission checking////////////////
	$current_user_id=$GLOBALS['global_user_id'];
	$adminuser=$GLOBALS['adminuser'];
	$record_owner="false";
	$record_maintainer="false";
	if ($bid_info->creator==$current_user_id) {
		$record_owner="true";
		$record_maintainer="true";
		}
	if ($record_maintainer=="false") {
		$rm_test=getoneb("select * from bidlist_permissions where bidlist_id = '$bid_info->bidlist_id' and contact = '$current_user_id' limit 1");
		if ($rm_test) { $record_maintainer="true"; }
		}
	if ($adminuser) $record_maintainer="true";
	$record_owner_info=getone("select * from contacts where contacts_id = '$bid_info->creator'");
	$bid_info->record_owner=$record_owner;
	$bid_info->record_maintainer=$record_maintainer;
	$bid_info->record_owner_display_name=$record_owner_info->display_name;
	$bid_info->record_owner_first_name=$record_owner_info->first_name;
	$bid_info->record_owner_last_name=$record_owner_info->last_name;
	$bid_info->record_owner_login=$record_owner_info->login;
	$bid_info->record_owner_initials=$record_owner_info->initials;
	
	///////////////Do some math, figure out duration////////////////
	$start_date=@strtotime($bid_info->start_date);
	$complete_date=@strtotime($bid_info->complete_date);
	$duration_seconds=$complete_date - $start_date;
	$bid_info->duration=round($duration_seconds / 86400);
	$bid_info->duration_months=$bid_info->duration / 30;
	$bid_info->duration_months=(round($bid_info->duration_months * 100)/100);
	
	
	///////////////Checkboxen/Radio 3 select buttons////////////////
	$bid_info->show_in_bidlist=checkbreak($bid_info->show_in_bidlist);
	$bid_info->show_in_bid_total_calc=checkbreak($bid_info->show_in_bid_total_calc);
	$bid_info->change_order=checkbreak($bid_info->change_order);
	$bid_info->public=checkbreak($bid_info->public);
	$bid_info->umc_gc_prime=checkbreak($bid_info->umc_gc_prime);
	$bid_info->private=checkbreak($bid_info->private);
	$bid_info->gccm=checkbreak($bid_info->gccm);
	
	/////////////// Dates ///////////////////////////////
	$bid_info->bid_date=invali_date($bid_info->bid_date);
	$bid_info->walk_through_date=invali_date($bid_info->walk_through_date);
	$bid_info->create_date=invali_date($bid_info->create_date);
	$bid_info->start_date=invali_date($bid_info->start_date);
	$bid_info->complete_date=invali_date($bid_info->complete_date);
	$bid_info->lead_next_activity=invali_date($bid_info->lead_next_activity);
	
	///////////////Integers//////////////////////////////
	$bid_info->proposal_num=intbreak($bid_info->proposal_num);
	$bid_info->deposit=intbreak($bid_info->deposit,1);
	$bid_info->rom=intbreak($bid_info->rom,1);
	$bid_info->engineer_budget=intbreak($bid_info->engineer_budget,1);
	$bid_info->approx_total_budget=intbreak($bid_info->approx_total_budget,1);
	if ($bid_info->engineer_budget!="") $bid_info->rom=$bid_info->engineer_budget;
	$bid_info->rom_hrs_plumb=intbreak($bid_info->rom_hrs_plumb,1);
	$bid_info->rom_hrs_fit=intbreak($bid_info->rom_hrs_fit,1);
	$bid_info->rom_hrs_sheet=intbreak($bid_info->rom_hrs_sheet,1);
	$bid_info->rom_hrs_mill_rights=intbreak($bid_info->rom_hrs_mill_rights,1);
	$bid_info->rom_hrs_iron=intbreak($bid_info->rom_hrs_iron,1);
	$bid_info->rom_hrs_carpenter=intbreak($bid_info->rom_hrs_carpenter,1);
	$bid_info->rom_hrs_misc=intbreak($bid_info->rom_hrs_misc,1);
	$bid_info->probability_raw=$bid_info->probability;
	$bid_info->probability=intbreak($bid_info->probability);
	$bid_info->bld_square_feet=intbreak($bid_info->bld_square_feet,1);
	$bid_info->bld_stories=intbreak($bid_info->bld_stories,1);
	$bid_info->umc_costs_lms=intbreak($bid_info->umc_costs_lms,1);

	///////////////Time//////////////////////////////////
	$bid_info->walk_through_time=timebreak($bid_info->walk_through_time);
	$bid_info->bid_time=timebreak($bid_info->bid_time);
	

	///////////////TO Assignments////////////////////////
	$to_info=getoneb("select * from contacts where contacts_id = '$bid_info->to_lead'");
	$bid_info->to_lead_initials=$to_info->initials;
	$bid_info->to_lead_display_name=$to_info->display_name;
	$bid_info->to_lead_login_name=$to_info->login_name;
	$bid_info->to_lead_first_name=$to_info->first_name;
	$bid_info->to_lead_last_name=$to_info->last_name;
	
	$to_info=getoneb("select * from contacts where contacts_id = '$bid_info->to_plm'");
	$bid_info->to_plm_initials=$to_info->initials;
	$bid_info->to_plm_display_name=$to_info->display_name;
	$bid_info->to_plm_login_name=$to_info->login_name;
	$bid_info->to_plm_first_name=$to_info->first_name;
	$bid_info->to_plm_last_name=$to_info->last_name;
	
	$to_info=getoneb("select * from contacts where contacts_id = '$bid_info->to_htg'");
	$bid_info->to_htg_initials=$to_info->initials;
	$bid_info->to_htg_display_name=$to_info->display_name;
	$bid_info->to_htg_login_name=$to_info->login_name;
	$bid_info->to_htg_first_name=$to_info->first_name;
	$bid_info->to_htg_last_name=$to_info->last_name;
	
	$to_info=getoneb("select * from contacts where contacts_id = '$bid_info->to_metal'");
	$bid_info->to_metal_initials=$to_info->initials;
	$bid_info->to_metal_display_name=$to_info->display_name;
	$bid_info->to_metal_login_name=$to_info->login_name;
	$bid_info->to_metal_first_name=$to_info->first_name;
	$bid_info->to_metal_last_name=$to_info->last_name;
	
	$to_info=getoneb("select * from contacts where contacts_id = '$bid_info->to_proc'");
	$bid_info->to_proc_initials=$to_info->initials;
	$bid_info->to_proc_display_name=$to_info->display_name;
	$bid_info->to_proc_login_name=$to_info->login_name;
	$bid_info->to_proc_first_name=$to_info->first_name;
	$bid_info->to_proc_last_name=$to_info->last_name;
	
	$to_info=getoneb("select * from contacts where contacts_id = '$bid_info->to_prop'");
	$bid_info->to_prop_initials=$to_info->initials;
	$bid_info->to_prop_display_name=$to_info->display_name;
	$bid_info->to_prop_login_name=$to_info->login_name;
	$bid_info->to_prop_first_name=$to_info->first_name;
	$bid_info->to_prop_last_name=$to_info->last_name;
	
	$to_info=getoneb("select * from contacts where contacts_id = '$bid_info->to_gct'");
	$bid_info->to_gct_initials=$to_info->initials;
	$bid_info->to_gct_display_name=$to_info->display_name;
	$bid_info->to_gct_login_name=$to_info->login_name;
	$bid_info->to_gct_first_name=$to_info->first_name;
	$bid_info->to_gct_last_name=$to_info->last_name;
	
	$to_info=getoneb("select * from contacts where contacts_id = '$bid_info->project_sponsor'");
	$bid_info->project_sponsor_initials=$to_info->initials;
	$bid_info->project_sponsor_display_name=$to_info->display;
	$bid_info->project_sponsor_login_name=$to_info->login_name;
	$bid_info->project_sponsor_first_name=$to_info->first_name;
	$bid_info->project_sponsor_last_name=$to_info->last_name;
	
	$to_info=getoneb("select * from contacts where contacts_id = '$bid_info->owner'");
	$bid_info->owner_display_name=$to_info->display_name;
	$bid_info->owner_address=$to_info->address_1;
	$bid_info->owner_address_city=$to_info->address_city;
	$bid_info->owner_address_state=$to_info->address_state;
	$bid_info->owner_address_zip=$to_info->address_zip;
	
	$to_info=getoneb("select * from contacts where contacts_id = '$bid_info->walk_through_employee'");
	$bid_info->walk_through_employee_display_name=$to_info->display_name;
	$bid_info->walk_through_employee_first_name=$to_info->first_name;
	$bid_info->walk_through_employee_last_name=$to_info->last_name;
	$bid_info->walk_through_employee_initials=$to_info->initials;
	$bid_info->walk_through_employee_email=$to_info->email;
	
	$to_info=getoneb("select * from contacts where contacts_id = '$bid_info->creator'");
	$bid_info->creator_display_name=$to_info->display_name;
	$bid_info->creator_first_name=$to_info->first_name;
	$bid_info->creator_last_name=$to_info->last_name;
	$bid_info->creator_initials=$to_info->initials;
	$bid_info->creator_email=$to_info->email;

	
	//////////////////////////////////////////////////////////////////////
	//	Bid Results Info
	//////////////////////////////////////////////////////////////////////
	$bid_info->results_count=@mysql_num_rows(@mysql_query("select * from bidlist_results where bidlist_id = '$bid_info->bidlist_id'"));
	$bid_info->winning_results_count=@mysql_num_rows(@mysql_query("select * from bidlist_results where bidlist_id = '$bid_info->bidlist_id' and winning_bid = 'Y'"));
	
	// Find the low general
	$gc_low_query="select * from bidlist_bidding_to where bidlist_id = '$bid_info->bidlist_id' and low_bid = 'Y'";
	$gc_low_info=getoneb($gc_low_query);
	if ($gc_low_info) {
		$bid_info->gc_low_set='Y';
		$gc_low_gc_info=getoneb("select * from contacts where contacts_id = '$gc_low_info->general_id'");
		$bid_info->gc_low_display_name=$gc_low_gc_info->display_name;
		}
	$our_bid_info=getoneb("select * from bidlist_results where bidlist_id = '$bid_info->bidlist_id' and company = '968' order by winning_bid desc,bid_amount limit 1");
	$bid_info->our_bid_amount=$our_bid_info->bid_amount;
	$bid_info->our_bid_amount_formatted=format_long_num($bid_info->our_bid_amount);
	if ($bid_info->our_bid_amount!="") $bid_info->our_bid_amount_formatted="\$$bid_info->our_bid_amount_formatted";
	if ($our_bid_info) $bid_info->umc_bid_results='Y';
	else $bid_info->umc_bid_results='N';

	$bid_info->umc_bid_won='?';
	if ($bid_info->winning_results_count>0) $bid_info->umc_bid_won="N";
	if ($our_bid_info->winning_bid=='Y') $bid_info->umc_bid_won='Y';

	$results_won_qry="select * from bidlist_results where bidlist_id = '$bid_info->bidlist_id' and winning_bid = 'Y'";
	$results_won_res=@mysql_query($results_won_qry);
	$results_won_count=@mysql_num_rows($results_won_res);

	if ($results_won_count<1) {
		$bid_info->results_winning_company="";
		}
	if ($results_won_count>1) {
		$bid_info->results_winning_company="<Cherry Pick>";
		$bid_info->results_winning_bid_amount="*";
		}
	if ($results_won_count==1) {
		$results_winning_company_info=@mysql_fetch_object($results_won_res);
		$results_won_company=getoneb("select * from contacts where contacts_id = '$results_winning_company_info->company'");
		$bid_info->results_winning_company=$results_won_company->display_name;
		$bid_info->results_winning_bid_amount=$results_winning_company_info->bid_amount;
		$bid_info->results_winning_bid_amount_formatted=intbreak($bid_info->results_winning_bid_amount,1);
		if ($bid_info->results_winning_bid_amount_formatted!="") $bid_info->results_winning_bid_amount_formatted="\$$bid_info->results_winning_bid_amount_formatted";
		}

	////////////////////////////////////////////////////////////////////////
	// End Bid Results
	////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////
	// Get a text list of the "project type(s)" aka, airport/casino/office
	////////////////////////////////////////////////////////////////////////
	/*
	$pt_info=@mysql_query("select project_type from bidlist_bid_projtypes left join bidlist_projecttypes using (project_type_id) where bidlist_id = '$bid_info->bidlist_id'");
	$bid_info->project_type_list="";
	$pt_seperator="";
	while($pt_row=@mysql_fetch_object($pt_info)) {
		$bid_info->project_type_list=$bid_info->project_type_list . $pt_seperator . $pt_row->project_type;
		$pt_seperator="/";
		}
	*/
	$bid_info->project_type_list=$bid_info->projtype;
	////////////////////////////////////////////////////////////////////////
	// Owner and general stuff?
	////////////////////////////////////////////////////////////////////////

	$job_info=getoneb("select * from jobinfo where jobinfo_id = '$bid_info->jobinfo_id' and jobinfo_id > 0");
	$bid_info->job_num=$job_info->contract_num;
	
	$owner_contact_info=getoneb("select * from contacts where contacts_id = '$bid_info->owner_contact_id'");
	$bid_info->owner_contact_display_name=$owner_contact_info->display_name;
	$bid_info->owner_contact_phone=$owner_contact_info->phone;
	if ($owner_contact_info) $bid_info->owner_contact_set=1;
	
	$gc_qry="select * from bidlist_bidding_to where bidlist_id = '$bid_info->bidlist_id'";
	$gc_res=@mysql_query($gc_qry);
	$gc_count=@mysql_num_rows($gc_res);
	$bid_info->bidding_to_count=$gc_count;

	$bid_info->gc_contact_text="";
	$bid_info->gc_contact_phone_text="";

	if ($gc_count < 1) $bid_info->gc_text="";
	if ($gc_count > 1) $bid_info->gc_text="Multiple";

	if ($gc_count == 1) {
		$gc_row=@mysql_fetch_object($gc_res);
		$gc_info=getoneb("select * from contacts where contacts_id = '$gc_row->general_id'");
		$bid_info->gc_text=$gc_info->display_name;
		$bid_info->gc_contact_text=$gc_row->contact_name;
		$bid_info->gc_contact_phone_text=$gc_row->contact_phone;
		$bid_info->gc_text_list=$bid_info->gc_text;
		} else {
		$bid_info->gc_text_list="";
		$gc_seperator="";
		while($gc_row=@mysql_fetch_object($gc_res)) {
			$gc_info=getoneb("select * from contacts where contacts_id = '$gc_row->general_id'");
			$bid_info->gc_text_list=$bid_info->gc_text_list . $gc_seperator . $gc_info->display_name;
			$gc_seperator=",";
			}
		}
	if ($bid_info->gc_low_set=='Y') $bid_info->gc_text_list=$bid_info->gc_low_display_name;
	
	//////////////////////////////////////////////////////////////////////
	// Proposal Number detection
	//////////////////////////////////////////////////////////////////////
	$up_one_info=getoneb("select proposal_num,bidlist_id from bidlist where proposal_num > $bid_info->proposal_num order by proposal_num limit 1");
	if ($up_one_info) {
		$bid_info->proposal_num_up=$up_one_info->proposal_num;
		$bid_info->proposal_id_up=$up_one_info->bidlist_id;
	} else {
		$bid_info->proposal_num_up=$bid_info->proposal_num;
		$bid_info->proposal_id_up=$bid_info->bidlist_id;
	}

	$up_one_info=getoneb("select proposal_num,bidlist_id from bidlist where proposal_num < $bid_info->proposal_num order by proposal_num desc limit 1");
	if ($up_one_info) {
		$bid_info->proposal_num_down=$up_one_info->proposal_num;
		$bid_info->proposal_id_down=$up_one_info->bidlist_id;
	} else {
		$bid_info->proposal_num_down=$bid_info->proposal_num;
		$bid_info->proposal_id_down=$bid_info->bidlist_id;
	}
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////
	// Get the first lead comment from the log, and return it for use
	// in reports. It now stops the old "lead_lead_notes" field, and 
	// borrows it's old place in the cache.
	//////////////////////////////////////////////////////////////////////
	$lead_lead_comment_info=getoneb("select * from bidlist_log where bidlist_id = $bid_info->bidlist_id and type = 'lead_comments' order by bidlist_log_id desc limit 1");
	$bid_info->lead_lead_notes=$lead_lead_comment_info->comment;

	//////////////////////////////////////////////////////////////////////
	// Get the group id for associations..
	//////////////////////////////////////////////////////////////////////
	$bl_ass_info=getoneb("select * from bidlist_associations where bidlist_id = '$bid_info->bidlist_id'");
	if ($bl_ass_info) $bid_info->bidlist_group=$bl_ass_info->bidlist_group;
	else $bid_info->bidlist_group=$bl_ass_info; // Defined as FALSE
	return $bid_info;
	}


function proposal_num_new ($year="") {
	$this_year=date('Y');
	$year_orig=$year;
	if ($year=="") $year=$this_year;
	$year=ereg_replace("[^0-9]","",$year);
	$len=strlen($year);
	if (($len<4)||($year<999)) {
		$year=$year + 2000;
		echo "<hr>Warning! Fixing screwed up year format in proposal_num_new ($year_orig).<hr>";
		}
	if ($year==2000) {
		echo "WARNING: Screwed up or null year. Just using this year ($this_year)!";
		$year=$this_year;
		}
	// This should fix a bug that cause leads to sometimes turn to duped proposal numbers
	//$info=getone("select max(proposal_num) as max_prop from bidlist where create_date >= '$year-01-01' and create_date <= '$year-12-31'");
	$info=getone("select max(proposal_num) as max_prop from bidlist where proposal_num >= '${year}0000' and proposal_num <= '${year}9999'");
	if (($info->max_prop=="")||($info->max_prop==0)) $info->max_prop="${year}0000";
	$downsize="${year}0000";
	$info->max_prop=$info->max_prop - $downsize;
	$info->max_prop++;
	$info->max_prop=str_pad($info->max_prop,4,"0",STR_PAD_LEFT);
	return "$year$info->max_prop";
	}

function proposal_list ($query,$load_mode="proposal_edit") {
	$res=@mysql_query($query);
	$rows=@mysql_num_rows($res);
	if ($rows==0) {
		echo "<b>No Entries Found...</b>";
		return 0;
		}
	echo "<table border=0 cellspacing=0 cellpadding=3>";
	while($row=@mysql_fetch_object($res)) {
		$prop_info=load_proposal_info($row->bidlist_id);
		$time=$prop_info->bid_timetext;
		if ($time!="") {
			$time=ereg_replace("m$","",$time);
			} else {
			$time="&nbsp;";
			}
		if ($prop_info->prop_num <=1) $prop_info->prop_num="L#&nbsp;$prop_info->bidlist_id";
		echo "
		<tr><td>
			<a href=$pagename?mode=$load_mode&bidlist_id=$prop_info->bidlist_id><font color=blue>$prop_info->prop_num</font></a>
		</td><td bgcolor=\"$prop_info->bid_date_color\">
			$prop_info->bid_datetext
		</td><td align=right bgcolor=\"$prop_info->bid_date_color\">
			$time
		</td><td>
			$prop_info->project
		</td></tr>
		";
		}
	echo "</table>";
	}

function bidrow_pdf_header($page_info) {
	pdf_setcolor($page_info->pdf,"fill","gray","1");
	$page_info=pdf_font($page_info,"Courier-Bold");
	$fontid=pdf_get_value($page_info->pdf,'font');
	$descender=pdf_get_value($page_info->pdf,'descender',$fontid);
    $descenderpts=$descender * $page_info->fontsize;
    
	pdf_rect($page_info->pdf,
	$page_info->sidemargins,
	($page_info->currenty + ($descenderpts)),
	($page_info->startx - ($page_info->sidemargins * 2)),
	(2 * $page_info->fontsize));
	
	pdf_closepath_fill_stroke($page_info->pdf);
	pdf_setcolor($page_info->pdf,"fill","gray","0");
	
	
	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_bid_id);
	pdf_show($page_info->pdf,"Prop/Bid");
	
	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_bid_date);
	pdf_show($page_info->pdf,"Date");
	
	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_bid_time);
	pdf_show($page_info->pdf,"Time");
	
	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_project);
	pdf_show($page_info->pdf,"Project");
	
	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_location);
	pdf_show($page_info->pdf,"Location");

	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_to_lead);
	pdf_show($page_info->pdf,"LEAD");
	
	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_to_plm);
	pdf_show($page_info->pdf,"PLUM");
	
	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_to_htg);
	pdf_show($page_info->pdf,"HEAT");
	
	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_to_metal);
	pdf_show($page_info->pdf,"METL");
	
	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_to_proc);
	pdf_show($page_info->pdf,"PROC");
	
	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_to_prop);
	pdf_show($page_info->pdf,"PROP");

	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_bid_bond);
	pdf_show($page_info->pdf,"BB");
	
	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_gc);
	pdf_show($page_info->pdf,"GC List");
	
	$page_info=pdf_font($page_info);
	$page_info=pdf_crlf($page_info,"","1.3");
	return $page_info;
	}

function bidrow_pdf($page_info,$bid_id) {
	$bid_info=load_proposal_info($bid_id);
	if (($bid_info->status=="Active") || ($bid_info->status=="active"))
		$page_info=pdf_font($page_info,"Courier-Bold");
		else 
		$page_info=pdf_font($page_info,"Courier");
		
	
	pdf_setcolor($page_info->pdf,"fill","gray",".8");

	$fontid=pdf_get_value($page_info->pdf,'font');
	$descender=pdf_get_value($page_info->pdf,'descender',$fontid);
    $descenderpts=$descender * $page_info->fontsize;
    
	pdf_setlinewidth($page_info->pdf,.1);
	pdf_rect($page_info->pdf,$page_info->sidemargins,($page_info->currenty + $descenderpts + $page_info->fontsize + 2),($page_info->startx - ($page_info->sidemargins * 2)),0);

	pdf_rect($page_info->pdf,
	$page_info->sidemargins,
	($page_info->currenty + ($descenderpts)),
	(($page_info->bidlist_x_bid_date - $page_info->bidlist_x_gap - $page_info->bidlist_x_bid_id) + 1),
	$page_info->fontsize);
	                
	pdf_closepath_fill_stroke($page_info->pdf);
	pdf_setcolor($page_info->pdf,"fill","gray","0");
	
	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_bid_id);
	pdf_show($page_info->pdf,"$bid_info->proposal_num");

	$bid_date=ereg_replace("-....$","",$bid_info->bid_date);
	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_bid_date);
	pdf_show($page_info->pdf,"$bid_date");
	

	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_bid_time);
	$page_info=pdf_font($page_info,"Times-Roman");
	
	$bt=$bid_info->bid_time;
	$bt=ereg_replace(":00 "," ",$bt);
	$bt=ereg_replace(" ","",$bt);
	$bt=ereg_replace("m","",$bt);
	pdf_show_truncated($page_info,"$bt",($page_info->bidlist_x_location - $page_info->bidlist_x_gap));
	$page_info=pdf_font($page_info);
	
	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_project);
	$page_info=pdf_font($page_info,"Times-Roman");
	pdf_show_truncated($page_info,"$bid_info->project",($page_info->bidlist_x_location - $page_info->bidlist_x_gap));
	$page_info=pdf_font($page_info);
	
	
	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_location);
	$page_info=pdf_font($page_info,"Times-Roman");
	pdf_show_truncated($page_info,$bid_info->location,($page_info->bidlist_x_to_lead - $page_info->bidlist_x_gap));
	$page_info=pdf_font($page_info);
	

	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_to_lead);
	pdf_show($page_info->pdf,"$bid_info->to_lead_initials");

	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_to_plm);
	pdf_show($page_info->pdf,"$bid_info->to_plm_initials");

	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_to_htg);
	pdf_show($page_info->pdf,"$bid_info->to_htg_initials");

	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_to_metal);
	pdf_show($page_info->pdf,"$bid_info->to_metal_initials");

	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_to_proc);
	pdf_show($page_info->pdf,"$bid_info->to_proc_initials");

	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_to_prop);
	pdf_show($page_info->pdf,"$bid_info->to_prop_initials");

	//$page_info=pdf_set_col($page_info,$page_info->bidlist_x_status);
	//$status=substr($bid_info->status,0,1);
	//pdf_show($page_info->pdf,"$status");

	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_bid_bond);
	pdf_show($page_info->pdf," $bid_info->bid_bond");

	$gc_list_long=false;
	$page_info=pdf_set_col($page_info,$page_info->bidlist_x_gc);
	$page_info=pdf_font($page_info,"Times-Roman");
	$page_info=pdf_fontsize($page_info,8);
	//pdf_show($page_info->pdf,"$bid_info->gc_text_list");
	$moretext=pdf_show_truncated($page_info,"$bid_info->gc_text_list");
	
	//if ($moretext!="") pdf_show_truncated($page_info,"--$moretext--");
	while ($moretext!="") {
		$gc_list_long=true;
		$page_info=pdf_fontsize($page_info);
		pdf_crlf($page_info,"",1.2);
		$page_info=pdf_fontsize($page_info,8);
		$page_info=pdf_set_col($page_info,$page_info->bidlist_x_gc);
		$moretext=pdf_show_truncated($page_info,"$moretext");
		}
	$page_info=pdf_fontsize($page_info);
	$page_info=pdf_font($page_info);

	if ($bid_info->comments!="") {
		pdf_crlf($page_info,"","1.4");
		$page_info=pdf_font($page_info,"Times-Roman");
		$page_info=pdf_set_col($page_info,$page_info->bidlist_x_comments);
		pdf_show($page_info->pdf,"Comments -- $bid_info->comments");
		$page_info=pdf_font($page_info);
		}
	//$page_info=pdf_set_col($page_info,$page_info->bidlist_x_comments);
	//$page_info=pdf_font($page_info,"Times-Roman");
	//pdf_show_truncated($page_info,"$bid_info->comments",($page_info->bidlist_x_status - $page_info->bidlist_x_gap));
	//$page_info=pdf_font($page_info);


	if (($bid_info->status=="Active") || ($bid_info->status=="active"))
		$page_info=pdf_font($page_info);
	
	$page_info=pdf_crlf($page_info,"","1.4");
	return $page_info;
	}

function proposal_search_stats_box($query,$output_title="Search Stats") {
	$results=proposal_search_stats($query);
	
	
	echo "<table border=1 cellspacing=0 cellpadding=3>
	<tr bgcolor=#dddddd><td colspan=2 align=center>
		<b>$output_title</b>
	</td></tr>
	
	<tr><td>
		Total Bids:
	</td><td align=right>
		$results->count
	</td></tr>

	<tr><td>
		Total Bid $:
	</td><td align=right>
		\$$results->total_our_bid_amount_pretty
	</td></tr>

	<tr><td>
		Total Won Y/N/?:
	</td><td align=right>
		$results->umc_won_y/$results->umc_won_n/$results->umc_won_other
	</td></tr>
	
	<tr><td>
		Total Won % Y/N/?:
	</td><td align=right>
		$results->umc_won_y_percentage% / $results->umc_won_n_percentage% / $results->umc_won_other_percentage%
	</td></tr>
	
	<tr><td>
		Total Won $:&nbsp;&nbsp;($results->umc_won_y_dollars_percentage%)
	</td><td align=right>
		\$$results->umc_won_y_dollars_pretty
	</td></tr>

	<tr><td>
		Total Lost $:&nbsp;&nbsp;($results->umc_won_n_dollars_percentage%)
	</td><td align=right>
		\$$results->umc_won_n_dollars_pretty
	</td></tr>

	<tr><td>
		Total ? $:&nbsp;&nbsp;($results->umc_won_other_dollars_percentage%)
	</td><td align=right>
		\$$results->umc_won_other_dollars_pretty
	</td></tr>




	</table>
	";
	}




function proposal_search_stats_box_pdf($pdf,$query,$output_title="<b>Report Statistics</b>",$no_abort=0) {
	$results=proposal_search_stats($query);
	if ($no_abort==0) {
		$pdf->transaction('start');
		$start_page=$pdf->ezPageCount;
		}

	$pdf->ezTableDumpStart();
	$pdf->ezTableDumpAdd("","<b>Bids","Percent","Dollars","% Dollars</b>");
	$pdf->ezTableDumpAdd("<b>Won</b>","$results->umc_won_y","$results->umc_won_y_percentage","$results->umc_won_y_dollars_pretty","$results->umc_won_y_dollars_percentage");
	$pdf->ezTableDumpAdd("<b>Lost</b>","$results->umc_won_n","$results->umc_won_n_percentage","$results->umc_won_n_dollars_pretty","$results->umc_won_n_dollars_percentage");
	$pdf->ezTableDumpAdd("<b>?</b>","$results->umc_won_other","$results->umc_won_other_percentage","$results->umc_won_other_dollars_pretty","$results->umc_won_other_dollars_percentage");

	$pdf->ezTableDumpAdd("","","","","");
	$pdf->ezTableDumpAdd("<b>Total</b>","$results->count","","$results->total_our_bid_amount_pretty");
	
	$pdf->ezTableDumpPrint(1,array('xPos'=>'left','xOrientation'=>'right','title'=>"$output_title",'cols'=>array(
	'0'=>array('width'=>'35'),
	'1'=>array('width'=>'35'),
	'2'=>array('width'=>'50'),
	'3'=>array('width'=>'70'),
	'4'=>array('width'=>'58')
	)));
	
	if ($no_abort==0) {
		if ($start_page!=$pdf->ezPageCount) {
			$pdf->transaction('abort');
			$pdf->ezNewPage();
			$pdf=proposal_search_stats_box_pdf($pdf,$query,$output_title,1);
			} else {
			$pdf->transaction('commit');
			}
		}
	return($pdf);
	}


function proposal_search_sum_box_pdf($pdf,$query,$output_title="<b>Totals</b>",$no_abort=0) {
	$results=proposal_search_stats($query);
	if ($no_abort==0) {
		$pdf->transaction('start');
		$start_page=$pdf->ezPageCount;
		}

	$pdf->ezTableDumpStart();
	$pdf->ezTableDumpAdd("","<b>Bids","Dollars</b>");
	$pdf->ezTableDumpAdd("<b>Total</b>","$results->count","$results->total_our_bid_amount_pretty");
	
	$pdf->ezTableDumpPrint(1,array('xPos'=>'left','xOrientation'=>'right','title'=>"$output_title",'cols'=>array(
	'0'=>array('width'=>'35'),
	'1'=>array('width'=>'35'),
	'2'=>array('width'=>'70')
	)));
	if ($no_abort==0) {
		if ($start_page!=$pdf->ezPageCount) {
			$pdf->transaction('abort');
			$pdf->ezNewPage();
			$pdf=proposal_search_stats_box_pdf($pdf,$query,$output_title,1);
			} else {
			$pdf->transaction('commit');
			}
		}
	return($pdf);
	}



function proposal_search_stats($query) {
	$res=@mysql_query($query);

	//////////////////////////////////////////////////////////
	//	Initialize a bunch of variables
	//////////////////////////////////////////////////////////
	$results->query=$query;
	$results->count=0;
	$results->umc_won_y=0;
	$results->umc_won_n=0;
	$results->umc_won_na=0;
	$results->umc_won_other=0;
	$results->umc_won_y_dollars=0;
	$results->umc_won_n_dollars=0;
	$results->umc_won_other_dollars=0;
	$results->umc_won_na_dollars=0;
	$results->awarded_pending_dollars=0;
	$results->awarded_pending=0;
	$results->projected_additional_dollars=0;
	$results->total_our_bid_amount=0;
	//////////////////////////////////////////////////////////
	
	if (!($res)) echo "ERROR: proposal_search_query failed! Failed query:<hr>$query";
	while($row=@mysql_fetch_object($res)) {
		$bid_info=getone("select * from bidlist_cache where bidlist_id = '$row->bidlist_id'");
		if ($bid_info->status=='No Bid') continue;
		$results->count++;
		
		$results->group_none_our_bid_sum=$results->group_none_our_bid_sum + $bid_info->our_bid;
		
		$results->total_our_bid_amount=$results->total_our_bid_amount + $bid_info->our_bid_amount;
		if ($bid_info->umc_bid_won=='Y') {
			$results->umc_won_y++;
			$results->umc_won_y_dollars=$results->umc_won_y_dollars + $bid_info->our_bid_amount;
			continue;
			}
		if ($bid_info->umc_bid_won=='N') {
			$results->umc_won_n++;
			$results->umc_won_n_dollars=$results->umc_won_n_dollars + $bid_info->our_bid_amount;
			continue;
			}
		if ($bid_info->umc_bid_won=='?') {
			$results->umc_won_other++;
			$results->umc_won_other_dollars=$results->umc_won_other_dollars + $bid_info->our_bid_amount;
			//if ($bid_info->probability > 0) {
			//	$more_dollars=$bid_info->probability * $bid_info->our_bid_amount / 100;
			//	$results->projected_additional_dollars=$results->projected_additional_dollars + @round($more_dollars);
			//	}
			continue;
			}
		continue;

		}
	$results->umc_won_y_percentage=@round ( 100 * $results->umc_won_y / $results->count );
	$results->umc_won_n_percentage=@round ( 100 * $results->umc_won_n / $results->count );
	$results->umc_won_other_percentage=@round ( 100 * $results->umc_won_other / $results->count );
	$results->umc_won_na_percentage=@round ( 100 * $results->umc_won_na / $results->count );
	$results->awarded_pending_percentage=@round ( 100 * $results->awarded_pending / $results->count );
	
	$results->umc_won_y_dollars_percentage=@round ( 100 * $results->umc_won_y_dollars / $results->total_our_bid_amount );
	$results->umc_won_n_dollars_percentage=@round ( 100 * $results->umc_won_n_dollars / $results->total_our_bid_amount );
	$results->umc_won_other_dollars_percentage=@round ( 100 * $results->umc_won_other_dollars / $results->total_our_bid_amount );
	$results->umc_won_na_dollars_percentage=@round ( 100 * $results->umc_won_na_dollars / $results->total_our_bid_amount );
	$results->awarded_pending_dollars_percentage=@round ( 100 * $results->awarded_pending_dollars / $results->total_our_bid_amount );
	
	$results->total_our_bid_amount_pretty=intbreak($results->total_our_bid_amount,1);

	$results->umc_won_y_dollars_pretty=intbreak($results->umc_won_y_dollars,1);
	$results->umc_won_n_dollars_pretty=intbreak($results->umc_won_n_dollars,1);
	$results->umc_won_other_dollars_pretty=intbreak($results->umc_won_other_dollars,1);
	$results->umc_won_na_dollars_pretty=intbreak($results->umc_won_na_dollars,1);
	$results->awarded_pending_dollars_pretty=intbreak($results->awarded_pending_dollars,1);
	return $results;
	}

require_once('reports.inc');
?>
