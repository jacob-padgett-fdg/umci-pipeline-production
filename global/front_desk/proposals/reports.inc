<?

function proposal_search_output_1 ($query,$title="Query Results",$edit_mode="proposal_edit") {
	$admin=$GLOBALS['adminuser'];
	$mode=$GLOBALS['mode'];
	$colspan="10";
	$res=@mysql_query($query);
	$rows=@mysql_num_rows($res);
	if ($rows==0) {
		echo "
		<table border=1 cellspacing=0 cellpadding=3 width=100%>
		<tr><td width=100% bgcolor=#dddddd align=center><h3>$title</h3></td></tr>
		</table>
		<b>No Entries Found...</b>";
		return 0;
		}
	if ($admin) $colspan=$colspan + 1;
	echo "<table border=1 cellspacing=0 cellpadding=0 width=100%>
	<tr><td colspan=$colspan>
		<table border=1 cellspacing=0 cellpadding=3 width=100%>
		<tr><td width=100% bgcolor=#dddddd align=center><h3>$title</h3></td></tr>
		</table>
	</td></tr>

	<tr><td>
	";
	
	if ($mode=="main_leads") echo "<b><a href=$pagename?mode=$mode&new_sort_by=bidlist_id style=\"color: blue\">Lead #</a></b>";
	else echo "<b><a href=$pagename?mode=$mode&new_sort_by=proposal_num style=\"color: blue\">Prop/Bid #</a></b>";
	
	echo"
	</td><td>
		<b><a href=$pagename?mode=$mode&new_sort_by=creator style=\"color: blue\">Init</a></td><td>
		<b><a href=$pagename?mode=$mode&new_sort_by=to_lead_initials style=\"color: blue\">Lead</a></b><font size=-1><br>*=init</font>
	";

	if ($mode!="main_leads") echo "
	</td><td>
		<b><a href=$pagename?mode=$mode&new_sort_by=bid_date_mysql style=\"color: blue\">Bid Date</a></b>
	</td><td>
		<b><a href=$pagename?mode=$mode&new_sort_by=bid_time style=\"color: blue\">Bid Time</a></b>
	</td><td>
	"; 
	else echo "
	</td><td>
		<b><a href=$pagename?mode=$mode&new_sort_by=lead_next_activity_orig style=\"color: blue\">Next Activity</a></b>
	</td><td><b><a href=$pagename?mode=$mode&new_sort_by=lead_ranking style=\"color: blue\">Rank</a></b></td><td>";
	
	echo "
		<b><a href=$pagename?mode=$mode&new_sort_by=project style=\"color: blue\">Project</a></b>
	</td><td>
		<b><a href=$pagename?mode=$mode&new_sort_by=umcgroups_id style=\"color: blue\">Group</a></b>
	</td><td>
		<b><a href=$pagename?mode=$mode&new_sort_by=owner_display_name style=\"color: blue\">Owner</a></b>
	</td><td>
		<b><a href=$pagename?mode=$mode&new_sort_by=gc_text style=\"color: blue\">General</a></b>
	</td><td>
		<b>Our Bid</b>
	";
	
	if ($admin) echo "</td><td><b><a href=$pagename?mode=$mode&new_sort_by=rom_orig style=\"color: blue\">ROM&nbsp;\$</a></b>";
	echo "</td></tr>";
	while($row=@mysql_fetch_object($res)) {
		$prop_info=load_proposal_info($row->bidlist_id);
		//echo "<pre>";print_r($prop_info);echo "</pre>";die();
		$group_info=getoneb("select * from bidlist_umcgroups where umcgroups_id = '$prop_info->umcgroups_id'");
		$time=$prop_info->bid_timetext;
		if ($time!="") {
			$time=ereg_replace("m$","",$time);
			} else {
			$time="&nbsp;";
			}
		if ($prop_info->to_lead_initials=="") $lead_init="*$prop_info->record_owner_initials";
		else $lead_init=$prop_info->to_lead_initials;
		if ($prop_info->prop_num <=1) $prop_info->prop_num="&nbsp;$prop_info->bidlist_id";
		if (($prop_info->status=="Lead")||($prop_info->status=="Dead Lead")) {
			$edit_mode="lead_edit";
			} else { 
			$edit_mode="proposal_edit";
			}
		echo "
		<tr><td>
			<a href=$pagename?mode=$edit_mode&bidlist_id=$prop_info->bidlist_id><font color=blue>$prop_info->prop_num$prop_info->revision</font></a>
		</td><td>
			$prop_info->creator_initials&nbsp;
		</td><td>
			$lead_init&nbsp;
		";
		
		if ($mode!="main_leads") echo "
			</td><td bgcolor=\"$prop_info->bid_date_color\">
				$prop_info->bid_datetext&nbsp;
			</td><td align=right bgcolor=\"$prop_info->bid_date_color\">
				&nbsp;$time
			</td><td>
		";
		
		else echo "</td><td>$prop_info->lead_next_activity&nbsp;</td><td>$prop_info->lead_ranking</td><td>";
		
		echo "
			&nbsp;$prop_info->project
		</td><td>
			&nbsp;$group_info->name
		</td><td>
			&nbsp;$prop_info->owner_display_name
		</td><td>
			&nbsp;$prop_info->gc_text
		</td><td>
			&nbsp;$prop_info->our_bid_amount_formatted";
		if ($admin) { if ($prop_info->rom!="") $prop_info->rom='$' . $prop_info->rom; echo "</td><td>$prop_info->rom&nbsp;"; }
		echo "</td></tr>";
		}
	echo "</table>";
	}

function proposal_search_output_2 ($query,$title="Query Results",$edit_mode="proposal_edit") {
	$res=@mysql_query($query);
	$rows=@mysql_num_rows($res);
	if ($rows==0) {
		echo "
		<table border=1 cellspacing=0 cellpadding=3 width=100%>
		<tr><td width=100% bgcolor=#dddddd align=center><h3>$title</h3></td></tr>
		</table>
		<b>No Entries Found...</b>";
		return 0;
		}
	echo "<table border=1 cellspacing=0 cellpadding=0 width=100%>
	<tr><td colspan=17>
		<table border=1 cellspacing=0 cellpadding=3 width=100%>
		<tr><td width=100% bgcolor=#dddddd align=center><h3>$title</h3></td></tr>
		</table>
	</td></tr>

	<tr><td>
		<b>Status</b>
	</td><td>
		<b>Prop/Bid</b>
	</td><td>
		<b>Lead</b><font size=-1><br>*=init</font>
	</td><td>
		<b>Project</b>
	</td><td>
		<b>Owner</b>
	</td><td>
		<b>Contact</b>
	</td><td>
		<b>Phone</b>
	</td><td>
			<b>Location</b>
	</td><td>
		<b>Bid Date</b>
	</td><td>
		<b>Amt</b>
	</td><td>
		<b>Prob</b>
	</td><td>
		<b>Awd</b>
	</td><td>
		<b>To Who</b>
	</td><td>
		<b>Job Num</b>
	</td><td>
		<b>Start Date</b>
	</td><td>
		<b>End Date</b>
	</td><td>
		<b>Days</b>
	</td></tr>
	";
	while($row=@mysql_fetch_object($res)) {
		$prop_info=load_proposal_info($row->bidlist_id);
	
		$biddate=ereg_replace("/200","/0",ereg_replace("-","/",$prop_info->bid_date));
		$sdate=ereg_replace("/200","/0",ereg_replace("-","/",$prop_info->start_date));
		$edate=ereg_replace("/200","/0",ereg_replace("-","/",$prop_info->complete_date));
		if ($biddate=="") $biddate="&nbsp;";
		if ($sdate=="") $sdate="&nbsp;";
		if ($edate=="") $edate="&nbsp;";
		
		$owner_short=substr($prop_info->owner_display_name,0,20);
		if ($prop_info->to_lead_initials=="") $lead_init="*$prop_info->record_owner_initials";
		else $lead_init=$prop_info->to_lead_initials;
	
		if ($prop_info->prop_num <=1) $prop_info->prop_num="L#&nbsp;$prop_info->bidlist_id";
		echo "
		<tr><td>
			$prop_info->status
		</td><td>
			<a href=$pagename?mode=$edit_mode&bidlist_id=$prop_info->bidlist_id><font color=blue>$prop_info->prop_num$prop_info->revision</font></a>
		</td><td>
			$lead_init&nbsp;
		</td><td>
			&nbsp;$prop_info->project
		</td><td>
			&nbsp;$owner_short
		</td><td>
			&nbsp;$prop_info->owner_contact_display_name
		</td><td>
			&nbsp;$prop_info->owner_contact_phone
		</td><td>
			&nbsp;$prop_info->location
		</td><td bgcolor=$prop_info->bid_date_color>
			<font size=-1>$biddate</font>
		</td><td align=right>
			&nbsp;$prop_info->our_bid_amount_formatted
		</td><td>
			&nbsp;$prop_info->probability
		</td><td>
			&nbsp;$prop_info->awarded
		</td><td>
			&nbsp;$prop_info->results_winning_company
		</td><td>
			&nbsp;$prop_info->job_num
		</td><td>
			<font size=-1>$sdate</font>
		</td><td>
			<font size=-1>$edate</font>
		</td><td>
			&nbsp;$prop_info->duration
		</td></tr>
		";
		
		$notescount=1;
		$res2=@mysql_query("select * from bidlist_log where bidlist_id = $prop_info->bidlist_id and type = 'proposal_comments' order by bidlist_log_id desc limit 3");
		while ($row2=@mysql_fetch_object($res2)) {
			if ($notescount==3) 
				echo "<tr><td colspan=18 align=center><b>--Additional Notes Truncated--</b></td></tr>";
			else
				echo "<tr><td colspan=3 align=right>Notes:&nbsp;</td><td colspan=15>&nbsp;$row2->comment</td></tr>";
			$notescount++;
			}
		}
	echo "</table>";
	}

function proposal_search_output_3 ($query,$title="Query Results") {
	global $mode;
	$last_bidlist_query=$query;
	include("show_bidlist_pdf.phtml");
	}


function proposal_search_output_4($query,$title="Query Results",$print_summary=0) {
	proposal_search_stats_box($query);
	}

	
function proposal_search_output_5 ($query,$title="Query Results") {
	$res=@mysql_query($query);
	$rows=@mysql_num_rows($res);
	if ($rows==0) {
		echo "
		<table border=1 cellspacing=0 cellpadding=3 width=100%>
		<tr><td width=100% bgcolor=#dddddd align=center><h3>$title</h3></td></tr>
		</table>
		<b>No Entries Found...</b>";
		return 0;
		}
	echo "<table border=1 cellspacing=0 cellpadding=0 width=100%>
	<tr><td colspan=20>
		<table border=1 cellspacing=0 cellpadding=3 width=100%>
		<tr><td width=100% bgcolor=#dddddd align=center><h3>$title</h3></td></tr>
		</table>
	</td></tr>

	<tr><td>
		<b>Bid Date</b>
	</td><td>
		<b>Prop/Bid</b>
	</td><td>
		<b>Project</b>
	</td><td>
		<b>Lead</b>
	</td><td>
		<b>Cost in Recap</b>
	</td><td>
		<b>Our Bid</b>
	</td><td>
		<b>Awarded<br>Bid Amt $</b>
	</td><td>
		<b>Who was low</b>
	</td><td>
		<b>% Off</b>
	</td><td>
		<b>Dept</b>
	</td><td>
		<b>Prop Type</b>
	</td><td>
		<b>Job#</b>
	</td><td>
		<b>Start</b>
	</td><td>
		<b>End</b>
	</td><td>
		<b>Duration</b>
	</td><td>
		<b>Status</b>
	</td><td>
		<b>Awarded</b>
	</td><td>
		<b>UMC Won</b>
	</td></tr>
	";
	while($row=@mysql_fetch_object($res)) {
		$prop_info=load_proposal_info($row->bidlist_id);
	
		$biddate=ereg_replace("/200","/0",ereg_replace("-","/",$prop_info->bid_date));
		$sdate=ereg_replace("/200","/0",ereg_replace("-","/",$prop_info->start_date));
		$edate=ereg_replace("/200","/0",ereg_replace("-","/",$prop_info->complete_date));
		if ($biddate=="") $biddate="&nbsp;";
		if ($sdate=="") $sdate="&nbsp;";
		if ($edate=="") $edate="&nbsp;";
		
		$owner_short=substr($prop_info->owner_display_name,0,20);
		if ($prop_info->to_lead_initials=="") $lead_init="*$prop_info->record_owner_initials";
		else $lead_init=$prop_info->to_lead_initials;
		if ($prop_info->umc_costs_lms!="") $prop_info->umc_costs_lms="\$&nbsp;$prop_info->umc_costs_lms";
		else $prop_info->umc_costs_lms="&nbsp;";
		if ($prop_info->prop_num <=1) $prop_info->prop_num="L#&nbsp;$prop_info->bidlist_id";

		//if (($prop_info->results_winning_bid_amount > 0) && ($prop_info->our_bid_amount > 0))
		//$off_by=(@round($prop_info->our_bid_amount / $prop_info->results_winning_bid_amount * 100) - 100) . "%";
		//else $off_by="";
		
		
		$umc_group_info=getoneb("select * from bidlist_umcgroups where umcgroups_id = '$prop_info->umcgroups_id'");
		$umc_group_info->name=ereg_replace(" ","&nbsp;",$umc_group_info->name);
		echo "
		<tr><td>
			$prop_info->bid_date
		</td><td>
			<a href=$pagename?mode=proposal_edit&bidlist_id=$prop_info->bidlist_id><font color=blue>$prop_info->prop_num$prop_info->revision</font></a>
		</td><td>
			&nbsp;$prop_info->project
		</td><td>
			$lead_init
		</td><td>
			$prop_info->umc_costs_lms
		</td><td bgcolor=$prop_info->bid_date_color>
			&nbsp;$prop_info->our_bid_amount_formatted
		</td><td align=right>
			&nbsp;$prop_info->results_winning_bid_amount_formatted
		</td><td>
			&nbsp;$prop_info->results_winning_company
		</td><td>
			";
			if ($prop_info->percent_from_pace_applice=='Y') 
				echo "&nbsp;$prop_info->percent_from_pace";
			else
				echo "&nbsp;";
			echo "
		</td><td>
			$umc_group_info->name
		</td><td>
			$prop_info->proposal_type
		</td><td>
			&nbsp;$prop_info->job_num
		</td><td>
			<font size=-1>$sdate</font>
		</td><td>
			<font size=-1>$edate</font>
		</td><td>
			&nbsp;$prop_info->duration
		</td><td>
			&nbsp;$prop_info->status
		</td><td>
			&nbsp;$prop_info->awarded
		</td><td>
			&nbsp;$prop_info->umc_bid_won
		</td></tr>
		";
		}
	echo "</table>";
	}

function proposal_search_output_5_pdf ($query,$pdf,$title="Query Results") {
	$res=@mysql_query($query);
	$rows=@mysql_num_rows($res);
	
	$pdf->ezTableDumpStart();
	if ($rows==0) {
		$pdf->ezTableDumpAdd("No matching records found...");
		$width=$pdf->ez['pageWidth'] - ($pdf->ez['leftMargin'] + $pdf->ez['rightMargin']);
		$pdf->ezTableDumpPrint(1,array('fontSize'=>'7','colGap'=>'5','width'=>"$width"));
		return ($pdf);
		}

	$pdf->ezTableDumpAdd("<b>Bid Date","Prop/Bid","Project","Lead","Direct Cost",
	"Our Bid","Awarded\nBid Amt","Who won","% Off","Dept","Prop Type","Job#",
	"Start","End","Duration","Status","Awarded","UMC Won</b>");

	while($row=@mysql_fetch_object($res)) {
		$prop_info=load_proposal_info($row->bidlist_id);
		$biddate=ereg_replace("/200","/0",ereg_replace("-","/",$prop_info->bid_date));
		$sdate=ereg_replace("/200","/0",ereg_replace("-","/",$prop_info->start_date));
		$edate=ereg_replace("/200","/0",ereg_replace("-","/",$prop_info->complete_date));
		$owner_short=substr($prop_info->owner_display_name,0,20);
		if ($prop_info->to_lead_initials=="") $lead_init="*$prop_info->record_owner_initials";
		else $lead_init=$prop_info->to_lead_initials;
		//if ($prop_info->umc_costs_lms!="") $prop_info->umc_costs_lms="\$ $prop_info->umc_costs_lms";
		if ($prop_info->prop_num <=1) $prop_info->prop_num="L# $prop_info->bidlist_id";
		$umc_group_info=getoneb("select * from bidlist_umcgroups where umcgroups_id = '$prop_info->umcgroups_id'");
		if ($prop_info->percent_from_pace_applies=='Y') $percent_from_pace="$prop_info->percent_from_pace";
		else $percent_from_pace="";
		
		$prop_link="<c:alink:http://pipeline.umci.com/global/proposals/?mode=proposal_edit&bidlist_id=$prop_info->bidlist_id>$prop_info->prop_num$prop_info->revision</c:alink>";
		$bid_info=getoneb("select * from bidlist_results where company = 968 and bidlist_id = '$prop_info->bidlist_id' limit 1");
		if ($bid_info->direct_cost!="") $bid_info->direct_cost="$" . format_long_decimal($bid_info->direct_cost);
		$pdf->ezTableDumpAdd($prop_info->bid_date,
			
			$prop_link,$prop_info->project,
			$lead_init,$bid_info->direct_cost,$prop_info->our_bid_amount_formatted,
			$prop_info->results_winning_bid_amount_formatted,$prop_info->results_winning_company,
			$percent_from_pace,$umc_group_info->name,$prop_info->proposal_type,
			$prop_info->job_num,$sdate,$edate,$prop_info->duration,
			$prop_info->status,$prop_info->awarded,$prop_info->umc_bid_won);
		}
	$width=$pdf->ez['pageWidth'] - ($pdf->ez['leftMargin'] + $pdf->ez['rightMargin']);
	$options_array=array(
	'splitRows'=>1,
	'fontSize'=>'8',
	'width'=>"$width",
	'cols'=>array(
	'0'=>array('width'=>'55'),
	'1'=>array('width'=>'53'),
	'2'=>array('width'=>'220'),
	'3'=>array('width'=>'30'),
	'4'=>array('width'=>'60'),
	'5'=>array('width'=>'55'),
	'6'=>array('width'=>'55'),
	'7'=>array('width'=>'100'),
	'8'=>array('width'=>'40'),
	'9'=>array('width'=>'70'),
	'10'=>array('width'=>'66'),
	'11'=>array('width'=>'35'),
	'12'=>array('width'=>'55'),
	'13'=>array('width'=>'55'),
	'14'=>array('width'=>'43'),
	'15'=>array('width'=>'55'),
	'16'=>array('width'=>'45'),
	'17'=>array('width'=>'48')
	));
	
	$pdf->ezTableDumpPrint(1,$options_array);
	$pdf=proposal_search_stats_box_pdf($pdf,$query);
	return ($pdf);
	}

function proposal_search_output_9_pdf ($query,$pdf,$title="Query Results") {
	$res=@mysql_query($query);
	$rows=@mysql_num_rows($res);
	
	$pdf->ezTableDumpStart();
	if ($rows==0) {
		$pdf->ezTableDumpAdd("No matching records found...");
		$width=$pdf->ez['pageWidth'] - ($pdf->ez['leftMargin'] + $pdf->ez['rightMargin']);
		$pdf->ezTableDumpPrint(1,array('fontSize'=>'7','colGap'=>'5','width'=>"$width"));
		return ($pdf);
		}

	$pdf->ezTableDumpAdd("<b>Prop/Bid","Project","Prob %","Prop Type",
	"Start","Finish","Days","Hours Plumb","Hours Fit","Hours S/M","Job#",
	"Start","End","Duration","Status","Awd","UMC","Our Bid</b>");
	
	while($row=@mysql_fetch_object($res)) {
		$prop_info=load_proposal_info($row->bidlist_id);
		if ($prop_info->status=="Almost Complete") $status_text="Almst Cmpl";
		else $status_text=$prop_info->status;
		//$biddate=ereg_replace("/200","/0",ereg_replace("-","/",$prop_info->bid_date));
		$sdate=ereg_replace("/200","/0",ereg_replace("-","/",$prop_info->start_date));
		$edate=ereg_replace("/200","/0",ereg_replace("-","/",$prop_info->complete_date));
		//$owner_short=substr($prop_info->owner_display_name,0,20);
		//if ($prop_info->umc_costs_lms!="") $prop_info->umc_costs_lms="\$ $prop_info->umc_costs_lms";
		if ($prop_info->prop_num <=1) $prop_info->prop_num="L# $prop_info->bidlist_id";
		//$umc_group_info=getoneb("select * from bidlist_umcgroups where umcgroups_id = '$prop_info->umcgroups_id'");
		//if ($prop_info->percent_from_pace_applies=='Y') $percent_from_pace="$prop_info->percent_from_pace";
		//else $percent_from_pace="";
		
		$prop_link="<c:alink:http://pipeline.umci.com/global/proposals/?mode=proposal_edit&bidlist_id=$prop_info->bidlist_id>$prop_info->prop_num$prop_info->revision</c:alink>";
		
		$pdf->ezTableDumpAdd(
			$prop_link,$prop_info->project,
			$prop_info->probability,$prop_info->proposal_type,$sdate,
			$edate,$prop_info->duration,
			$prop_info->rom_hrs_plumb,$prop_info->rom_hrs_fit,$prop_info->rom_hrs_sheet,
			$prop_info->job_num,$sdate,$edate,$prop_info->duration,
			$status_text,$prop_info->awarded,$prop_info->umc_bid_won,
			$prop_info->our_bid_amount);
		}
	$width=$pdf->ez['pageWidth'] - ($pdf->ez['leftMargin'] + $pdf->ez['rightMargin']);
	$options_array=array(
	'splitRows'=>1,
	'fontSize'=>'8',
	'width'=>"$width",
	'cols'=>array(
	'0'=>array('width'=>'53'),
	'1'=>array('width'=>'220'),
	'2'=>array('width'=>'30'),
	'3'=>array('width'=>'60'),
	'4'=>array('width'=>'42'),
	'5'=>array('width'=>'42'),
	'6'=>array('width'=>'30'),
	'7'=>array('width'=>'36'),
	'8'=>array('width'=>'36'),
	'9'=>array('width'=>'36'),
	'10'=>array('width'=>'35'),
	'11'=>array('width'=>'55'),
	'12'=>array('width'=>'55'),
	'13'=>array('width'=>'43'),
	'14'=>array('width'=>'55'),
	'15'=>array('width'=>'28'),
	'16'=>array('width'=>'29'),
	'17'=>array('width'=>'43')
	));
	
	$pdf->ezTableDumpPrint(1,$options_array);
	$pdf=proposal_search_sum_box_pdf($pdf,$query);
	return ($pdf);
	}

function proposal_search_output_6 ($query,$title="Query Results") {
	$res=@mysql_query($query);
	
	echo "<table border=1 cellspacing=0 cellpadding=3>
	<tr bgcolor=#dddddd><td colspan=11 align=center><b>$title</b></td></tr>
	
	<tr bgcolor=#dddddd><td>
		Lead ID
	</td><td>
		Project Name:
	</td><td>
		Next Activity
	</td><td>
		Ranking
	</td><td>
		Lead
	</td><td>
		Start
	</td><td>
		Project Type
	</td><td>
		Total Value
	</td><td>
		Mech Value
	</td><td>
		GC
	</td><td>
		Notes
	</td></tr>
	";
	
	
	while ($row=@mysql_fetch_object($res)) {
		$bid_info=load_proposal_info($row->bidlist_id);
		//if ($bid_info->joblist_id < 1) continue;
		$arch_info=getoneb("select * from contacts where contacts_id = '$bid_info->architect'");
		echo "
		<tr><td>
			$bid_info->bidlist_id
		</td><td>
			$bid_info->project
		</td><td>
			$bid_info->lead_next_activity&nbsp;
		</td><td>
			$bid_info->lead_ranking&nbsp;
		</td><td>
			$bid_info->to_lead_initials&nbsp;
		</td><td>
			$bid_info->start_date&nbsp;
		</td><td>
			$bid_info->project_type_list&nbsp;
		</td><td>
			$bid_info->approx_total_budget&nbsp;
		</td><td>
			$bid_info->rom&nbsp;
		</td><td>
			$bid_info->gc_text_list&nbsp;
		</td><td>
			$bid_info->lead_lead_notes&nbsp;
		</td></tr>
		";
		}
	echo "</table>";
	}

function proposal_search_output_7 ($query,$title="Query Results") {
	$res=@mysql_query($query);
	
	echo "<table border=1 cellspacing=0 cellpadding=3>
	<tr bgcolor=#dddddd><td colspan=6 align=center><b>$title</b></td></tr>
	
	<tr bgcolor=#dddddd><td>
		Lead ID
	</td><td>
		Project Name
	</td><td>
		Next Activity
	</td><td>
		Ranking
	</td><td>
		Lead
	</td><td>
		Start Date
	</td><td>
		Use
	</td><td>
		ROM
	</td><td>
			
	</td></tr>
	";
	
	
	while ($row=@mysql_fetch_object($res)) {
		$bid_info=load_proposal_info($row->bidlist_id);
		//if ($bid_info->joblist_id < 1) continue;
		$arch_info=getoneb("select * from contacts where contacts_id = '$bid_info->architect'");
		echo "
		<tr><td>
			$bid_info->project
		</td><td>
			$bid_info->rom (ROM)
		</td><td>
			$bid_info->gc_text_list&nbsp;
		</td><td>
			$arch_info->display_name&nbsp;
		</td><td>
			$bid_info->start_date&nbsp;
		</td><td>
			$bid_info->end_date&nbsp;
		</td></tr>
		";
		}
	
	
	
	echo "</table>";
	}

function proposal_search_output_8 ($query,$title="Follow Up Report",$userid="0") {
	if ($title=="") $title="Follow Up Report";
	
	if ($userid>0) {
		$contact_info=getoneb("select * from contacts where contacts_id = '$userid'");
		$title=$title . " for $contact_info->display_name";
		}

	$res=@mysql_query($query);
	$rowcount=@mysql_num_rows($res);
	if ($rowcount < 1) {
		echo "<b>No items requiring follow up found...</b>";
		return 0;
		}
	echo "<table border=1 cellspacing=0 cellpadding=5>
	<tr bgcolor=#eeeeee><td colspan=8 align=center><h3>$title</h3></td></tr>
	
	<tr bgcolor=#eeeeee><td>
		<b>Prop Num</b>
	</td><td>
		<b>Bid Date</b>
	</td><td>
		<b>Job Name</b>
	</td><td>
		Price
	</td><td>
		<b>Proposal Type</b>
	</td><td>
		<b>GC</b>
	</td><td colspan=2>
		<b>Comments</b>
	</td></tr>
	";
	
	while($row=@mysql_fetch_object($res)) {
		$bid_info=load_proposal_info($row->bidlist_id);
		echo "
		<tr><td>
			<a href=$pagename?mode=proposal_edit&bidlist_id=$bid_info->bidlist_id target=prop_look_win><font color=blue>$bid_info->proposal_num$bid_info->revision</font></a>
		</td><td>
			$bid_info->bid_date
		</td><td>
			$bid_info->project
		</td><td>
			$bid_info->our_bid_amount_formatted&nbsp;
		</td><td>
			$bid_info->proposal_type
		</td><td>
			$bid_info->gc_text&nbsp;
		</td><td>
			";
			$comres=@mysql_query("select left(comment,50) as comment from bidlist_log where bidlist_id = '$bid_info->bidlist_id' and type = 'proposal_comments' order by bidlist_log_id desc limit 1");
			$comcount=@mysql_num_rows($comres);
			if ($comcount>0) {
				$comrow=@mysql_fetch_object($comres);
				echo "$comrow->comment";
				} else {
				echo "&nbsp;";
				}
			echo"
		</td><td>
			<a href=$pagename?mode=proposal_edit_add_comment_wizard&bidlist_id=$bid_info->bidlist_id&return_to_contact=$userid><font color=blue size=-1><i>Add</i></font></a>
		</td></tr>
		";
		}
}
?>
