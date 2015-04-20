<?
require_once("viewpoint_libs.inc");

function verify_ts_integrity($timesheet_id) {
	$timesheet_id=addslashes($timesheet_id);
	$ts_info=getone("select * from timesheet_index where timesheet_id = '$timesheet_id'");
	$error_query="select * from timesheet_time where timesheet_id = '$timesheet_id' and timesheet_status != '$ts_info->status' limit 1";
	$ts_errors=getoneb($error_query);
	if ($ts_errors) {
		echo "<font size=+1><b>FATAL ERROR:</b></font> The timesheet with ID $timesheet_id seems to be corrupt! Please contact the database administrator to get this resolved! Payroll should not continue until this item is fixed!!!<p>Debug information:<hr>Timesheet Status is: \"$ts_info->status\" but the query below returned something!!!<p>$error_query<p><b>Suspect Data:</b>";
		tabledump("select * from timesheet_time where timesheet_id = '$timesheet_id' and timesheet_status != '$ts_info->status'");
		die();
		}
	return 0;
	}

function send_safety_email($message) {
	$subject="Safety incident notification from TIMESHEET";
	$res=@mysql_query("select * from timesheet_safety_recipients");
	while ($row=@mysql_fetch_object($res)) {
		queue_email($row->contacts_id,$subject,$message,'Y');
		}
	}

/////////////////////////////////////////////////////////////////////////////////////////
function employee_sick_hours($employee_num) {
	$ms_query="select AvailBal from PREL with (NOLOCK) where PRCo = 1 and Employee = $employee_num and LeaveCode = 30";
	$sick_hrs=ms_getoneb($ms_query);
	if (!($sick_hrs)) return 0;
	return $sick_hrs->AvailBal;
	}

/////////////////////////////////////////////////////////////////////////////////////////
function employee_sick_hours_end_payperiod($employee_num) {
	return 0;
	}


/////////////////////////////////////////////////////////////////////////////////////////
function employee_vacation_hours($employee_num) {
	$ms_query="select AvailBal from PREL with (NOLOCK) where PRCo = 1 and Employee = $employee_num and LeaveCode = 20";
	$vac_hrs=ms_getoneb($ms_query);
	if (!($vac_hrs)) return 0;
	return $vac_hrs->AvailBal;
	}

/////////////////////////////////////////////////////////////////////////////////////////
function timesheet_summary_info($tsid) {
	verify_ts_integrity($tsid);
	$ts_info=new bsclass;
	$ts_info->timesheet_id=$tsid;
	$ts_info->st=0;
	$ts_info->ot=0;
	$ts_info->dt=0;
	$ts_info->v=0;
	$ts_info->s=0;
	$ts_info->h=0;
	$ts_info->hours=0;
	$ts_info->exps=0;
	$ts_info->shift_tag="";
	$alt_shift_tag="<acronym title=\"User has time that isn't shift 1\">*</acronym>";
	
	$emp_num=getone("select employee_num from timesheet_index where timesheet_id = '$tsid'");
	$emp_sick_hrs=employee_sick_hours($emp_num->employee_num);
	$emp_vac_hrs=employee_vacation_hours($emp_num->employee_num);
	$ts_info->avail_sick=$emp_sick_hrs;
	$ts_info->avail_vacation=$emp_vac_hrs;
	
	$query="select * from timesheet_time where timesheet_id = '$tsid'";
	$res=@mysql_query($query);
	
	while($row=@mysql_fetch_object($res)) {
		if ($row->shift!=1) {
			$ts_info->shift_tag=$alt_shift_tag;
			$ts_info->alt_shift=1;
			}
		switch ($row->earnings_code) {
			case (1):	$ts_info->st=$ts_info->st + $row->hours;
						$ts_info->hours=$ts_info->hours + $row->hours;
						break;
						
			case (2):	$ts_info->ot=$ts_info->ot + $row->hours;
						$ts_info->hours=$ts_info->hours + $row->hours;
						break;
						
			case (3):	$ts_info->dt=$ts_info->dt + $row->hours;
						$ts_info->hours=$ts_info->hours + $row->hours;
						break;
						
			case (8):	$ts_info->v=$ts_info->v + $row->hours;
						$ts_info->hours=$ts_info->hours + $row->hours;
						break;
						
			case (9):	$ts_info->s=$ts_info->s + $row->hours;
						$ts_info->hours=$ts_info->hours + $row->hours;
						break;
						
			case (10):	$ts_info->h=$ts_info->h + $row->hours;
						$ts_info->hours=$ts_info->hours + $row->hours;
						break;
			
			case (5):	$ts_info->exps=$ts_info->exps + $row->dollars;
						break;

			case (6):	$ts_info->exps=$ts_info->exps + $row->dollars;
						break;

			case (7):	$ts_info->exps=$ts_info->exps + $row->dollars;
						break;

			}
		}
	return $ts_info;
	}


/////////////////////////////////////////////////////////////////////////////////////////
function next_week_end($date="",$weeks=0) {
	if ($date=="") {
		$date=date("m-d-Y",(strtotime("saturday"))+14400);
		} else { 
		$date=date("m-d-Y",(strtotime("saturday",(strtotime(vali_date($date)))) + 14400));
		}
	$date=date("m-d-Y",strtotime("+$weeks weeks",strtotime(vali_date($date))));
	return($date);
	}

/////////////////////////////////////////////////////////////////////////////////////////
function last_week_end($date="",$weeks=0) {
	if ($date=="") {
		//$date=date("m-d-Y",strtotime("last saturday"));
		$date=date("m-d-Y",(strtotime("last saturday"))+14400);
		} else { 
		//$date=date("m-d-Y",strtotime("last saturday",strtotime(vali_date($date))));
		$date=date("m-d-Y",(strtotime("last saturday",strtotime(vali_date($date))))+14400);
		}
	$date=date("m-d-Y",(strtotime("-$weeks weeks",strtotime(vali_date($date))))+14400);
	return($date);
	}

/////////////////////////////////////////////////////////////////////////////////////////
function next_avail_week_end($employee_num,$date="") {
	if ($date=="") $date=next_week_end();
	while (!(ts_is_avail($date,$employee_num))) {
		$date=next_week_end($date,1);
		}
	return $date;
	}

/////////////////////////////////////////////////////////////////////////////////////////
function last_avail_week_end($employee_num,$date="") {
	if ($date=="") $date=last_week_end();
	while(!(ts_is_avail($date,$employee_num))) {
		$date=last_week_end($date,0);
		}
	return $date;
	}

/////////////////////////////////////////////////////////////////////////////////////////
function ts_is_avail($date,$employee_num) {
	$date=vali_date($date);
	$testvar=getoneb("select timesheet_id from timesheet_index where employee_num = '$employee_num' and week_ending = '$date' limit 1");
	//echo "select timesheet_id from timesheet_index where employee_id = '$employee_num' and week_ending = '$date' limit 1";
	if (!($testvar))
		return true;
		else {
		return false;
		}
	}

/////////////////////////////////////////////////////////////////////////////////////////
function week_start($week_end) {
	return date("m-d-Y",strtotime("-6 days",strtotime(vali_date($week_end))));
	}

/////////////////////////////////////////////////////////////////////////////////////////
function next_day($date) {
	return date("m-d-Y",strtotime("+1 days",strtotime(vali_date($date))));
	}

/////////////////////////////////////////////////////////////////////////////////////////
function previous_day($date) {
	return date("m-d-Y",strtotime("-1 days",strtotime(vali_date($date))));
	}

/////////////////////////////////////////////////////////////////////////////////////////
function prev_day($date) {
	return previous_day($date);
	}

/////////////////////////////////////////////////////////////////////////////////////////
function day_of_week_long($date) {
	return date("l",strtotime(vali_date($date)));
	}

/////////////////////////////////////////////////////////////////////////////////////////
function day_of_week($date) {
	return date("D",strtotime(vali_date($date)));
	}

/////////////////////////////////////////////////////////////////////////////////////////
function day_of_week_num($date) {
	return date("w",strtotime(vali_date($date)));
	}

/////////////////////////////////////////////////////////////////////////////////////////
function date_short_slash($date) {
	return date("m/d",strtotime(vali_date($date)));
	}

/////////////////////////////////////////////////////////////////////////////////////////
function date_short($date) {
	return date("m-d",strtotime(vali_date($date)));
	}

/////////////////////////////////////////////////////////////////////////////////////////
function week_fully_posted($week) {
	return (!(getoneb("select * from timesheet_index where week_ending = '$week' and status != 'posted' and status != 'deleted' limit 1")));
	}

/////////////////////////////////////////////////////////////////////////////////////////
function recent_jobs ($employee_id,$expenses=0) {
$query="select job_num,job_desc,max(timesheet_time_id) as maxtimesheet_time_id from timesheet_time where employee_id = '$employee_id' group by job_num order by maxtimesheet_time_id desc limit 10";
$res=@mysql_query($query);
echo "<tt>";
while($row=@mysql_fetch_object($res)) {
	$row->job_num=ereg_replace("- *$","",$row->job_num);
	$row->job_num=ereg_replace(" ","",$row->job_num);
	echo "<a title=\"$row->job_desc\" href=javascript:document.add_time.job_num.value=\"$row->job_num\";document.add_time.phase.focus();><font color=black>$row->job_num</font></a><br>";
	}
echo "<hr>";
if (!($expenses)) echo "
<a title=DD708 href=javascript:document.add_time.job_num.value='Vacation';document.add_time.phase.value='';document.add_time.hours.focus();><font color=black>Vacation</font></a><br>
<a title=DD709 href=javascript:document.add_time.job_num.value='Sick';document.add_time.phase.value='';document.add_time.hours.focus();><font color=black>Sick</font></a><br>
<a title=DD710 href=javascript:document.add_time.job_num.value='Holiday';document.add_time.phase.value='';document.add_time.hours.focus();><font color=black>Holiday</font></a><br>";

echo "<a title=DD724 href=javascript:document.add_time.job_num.value='Training';document.add_time.phase.value='';document.add_time.hours.focus();><font color=black>Training</font></a><br>
</tt>
";
}

/////////////////////////////////////////////////////////////////////////////////////////
function recent_phases ($employee_id) {
$query="select phase,phase_desc,max(timesheet_time_id) as maxtimesheet_time_id from timesheet_time where employee_id = '$employee_id' group by phase order by maxtimesheet_time_id desc limit 10";
$res=@mysql_query($query);
echo "<tt>";
while($row=@mysql_fetch_object($res)) {
	$row->job_num=ereg_replace("- *$","",$row->phase);
	echo "<a title=\"$row->phase_desc\" href=javascript:document.add_time.phase.value='$row->phase';document.add_time.hours.focus();><font color=black>$row->phase</font></a><br>";
	}
echo "</tt>";

}


/////////////////////////////////////////////////////////////////////////////////////////
function print_day($contacts_id,$date,$showblank="1",$return_to="",$noedit="0") {
$day_info=date_info($date);
$contact_info=getone("select * from contacts where contacts_id = '$contacts_id'");
$query="select * from timesheet_time,contacts where employee_id = '$contacts_id' and workday = '$day_info->date_mysql' and contacts_id = creator order by timesheet_time_id";
$res=@mysql_query($query);
$sum_query="select sum(hours) as total_hours from timesheet_time where employee_id = '$contacts_id' and workday = '$day_info->date_mysql' order by timesheet_time_id";
$day_sum=getoneb($sum_query);
$padded_day=str_pad($day_info->day_of_week_long,9);
$padded_day=ereg_replace(" ","&nbsp;",$padded_day);
if ( (mysql_num_rows($res)==0) && ($showblank==0) ) return 0;
$addurl="$pagename?mode=timesheet_edit&timesheet_mode_new=timesheet_showday&showday=$day_info->date&return_to_set=$return_to";

echo "<tt>
<table border=0 cellpadding=0 cellspacing=0 width=800px>
<tr><td bgcolor=#dddddd border=1>
	<table border=1 cellpadding=1 cellspacing=0 width=200px><tr><td  align=center width=100px><tt width=200px>
	"; if ($noedit==0) echo "<a href=$addurl>";
	echo "<font color=black>$padded_day&nbsp;($day_info->date_slashes)</font>
	"; if ($noedit==0) echo "</a>";
	echo "</tt></td></tr></table>
	</td><td width=700px valign=bottom>
	<sub><hr valign=bottom></sub>
</td></tr>
</table>
";

echo "<table border=0 cellpadding=0 width=800px>";
if (mysql_num_rows($res)>0) {
	echo "
	<tr><td>
		<tt><b>Job&nbsp;#</b></tt>
	</td><td>
		<tt><b>Phase</b></tt>
	</td><td>
		<tt><b>Description</b></tt>
	</td><td>
		<tt><b>Input</b></tt>
	</td><td>
		<tt><b>Hours</b></tt>
	</td><td>
		<tt><b>Sh</b></tt>
	</td></tr>
	";
	}

	$day_sum->sum_st=0;
	$day_sum->sum_ot=0;
	$day_sum->sum_dt=0;
	$day_sum->sum_v=0;
	$day_sum->sum_s=0;
	$day_sum->sum_h=0;
	$day_sum->subs="0.00";
	$day_sum->park="0.00";
	$day_sum->trav="0.00";
	$day_sum->exps="0.00";
while($row=@mysql_fetch_object($res)) {
	$row->job_num=ereg_replace("- *$","",$row->job_num);
	$row->job_num=ereg_replace("- *$","",$row->job_num);
	$ectype="ERROR!";
	switch ($row->earnings_code) {
		case 1:
			$ectype="ST";$day_sum->sum_st=$day_sum->sum_st + $row->hours;break;
		case 2:
			$ectype="OT";$day_sum->sum_ot=$day_sum->sum_ot + $row->hours;break;
		case 3:
			$ectype="DT";$day_sum->sum_dt=$day_sum->sum_dt + $row->hours;break;
		case 8:
			$ectype="V&nbsp;";$day_sum->sum_v=$day_sum->sum_v + $row->hours;break;
		case 9:
			$ectype="S&nbsp;";$day_sum->sum_s=$day_sum->sum_s + $row->hours;break;
		case 10:
			$ectype="H&nbsp;";$day_sum->sum_h=$day_sum->sum_h + $row->hours;break;
		case 5:
			$row->phase_desc="Subsistence";
			$ectype="$";$row->hours=$row->dollars;
			$day_sum->subs=$day_sum->subs + $row->dollars;
			$day_sum->exps=$day_sum->exps + $row->dollars;
			break;
		case 6:
			$row->phase_desc="Parking";
			$ectype="$";$row->hours=$row->dollars;
			$day_sum->park=$day_sum->park + $row->dollars;
			$day_sum->exps=$day_sum->exps + $row->dollars;
			break;
		case 7:
			$row->phase_desc="Travel";
			$ectype="$";$row->hours=$row->dollars;
			$day_sum->trav=$day_sum->trav + $row->dollars;
			$day_sum->exps=$day_sum->exps + $row->dollars;
			break;
		}
	$row_description="$row->job_desc - $row->phase_desc";
	$row_description=substr($row_description,0,59);
	
	echo "
	</tt></tt></tt></tt></tt>
	<tr valign=top><td>
		<tt>$row->job_num</tt>
	</td><td>
		<tt>$row->phase</tt>
	</td><td>
		<font size=-1>$row_description</font>
	</td><td>
		<tt>$row->login_name</tt>
	</td><td align=right>
		<tt>$ectype&nbsp;$row->hours</tt>
	</td><td align=center>
		<tt>$row->shift</tt>

	";
	echo "</td><td>";
	if ($row->comments!="") echo "<a href=javascript:shownotes($row->timesheet_time_id)>N</a>";
	if ($noedit==0) {
		if ($ectype=='$') echo "</td><td><tt><a href=javascript:dte('$row->timesheet_time_id');><font size=-1 color=blue><i>Delete</i></font></a></tt>";
		else echo "</td><td><tt><a href=javascript:ete('$row->timesheet_time_id');><font size=-1 color=blue><i>Edit</i></font></a></tt>";
		}
	echo "</td></tr>";
	
	}
$day_sum->total_hours++;
$day_sum->total_hours--;

$day_sum->sum_st++;$day_sum->sum_st--;
$day_sum->sum_ot++;$day_sum->sum_ot--;
$day_sum->sum_dt++;$day_sum->sum_dt--;
$day_sum->sum_v++;$day_sum->sum_v--;
$day_sum->sum_s++;$day_sum->sum_s--;
$day_sum->sum_h++;$day_sum->sum_h--;

$day_sum->subs=sprintf('%.2f',$day_sum->subs);
$day_sum->park=sprintf('%.2f',$day_sum->park);
$day_sum->trav=sprintf('%.2f',$day_sum->trav);
$day_sum->exps=sprintf('%.2f',$day_sum->exps);

if ($day_sum->subs!=0) echo "<tr><td colspan=4 width=100% align=right><tt>Subsistence:</tt></td><td align=right><tt>\$$day_sum->subs</tt></td></tr>";
if ($day_sum->park!=0) echo "<tr><td colspan=4 width=100% align=right><tt>Parking:</tt></td><td align=right><tt>\$$day_sum->park</tt></td></tr>";
if ($day_sum->trav!=0) echo "<tr><td colspan=4 width=100% align=right><tt>Travel:</tt></td><td align=right><tt>\$$day_sum->trav</tt></td></tr>";
if ($day_sum->exps!=0) echo "<tr><td colspan=4 width=100% align=right><tt>Total&nbsp;S/P/T:</tt></td><td align=right><tt>\$$day_sum->exps</tt></td></tr>";

if ($day_sum->sum_st!=0) echo "<tr><td colspan=4 width=100% align=right><tt>Straight&nbsp;Time&nbsp;Hours:</tt></td><td align=right><tt>$day_sum->sum_st</tt></td></tr>";
if ($day_sum->sum_ot!=0) echo "<tr><td colspan=4 width=100% align=right><tt>Overtime&nbsp;Hours:</tt></td><td align=right><tt>$day_sum->sum_ot</tt></td></tr>";
if ($day_sum->sum_dt!=0) echo "<tr><td colspan=4 width=100% align=right><tt>Doubletime&nbsp;Hours:</tt></td><td align=right><tt>$day_sum->sum_dt</tt></td></tr>";
if ($day_sum->sum_v!=0) echo "<tr><td colspan=4 width=100% align=right><tt>Vacation&nbsp;Hours:</tt></td><td align=right><tt>$day_sum->sum_v</tt></td></tr>";
if ($day_sum->sum_s!=0) echo "<tr><td colspan=4 width=100% align=right><tt>Sick&nbsp;Hours:</tt></td><td align=right><tt>$day_sum->sum_s</tt></td></tr>";
if ($day_sum->sum_h!=0) echo "<tr><td colspan=4 width=100% align=right><tt>Holiday&nbsp;Hours:</tt></td><td align=right><tt>$day_sum->sum_h</tt></td></tr>";
 

echo "
<tr><td width=100% colspan=4 align=right><tt>Total&nbsp;Hours:</tt></td><td align=right><tt>$day_sum->total_hours</tt></td></tr>
</table></tt>";

return($day_sum);

}

/////////////////////////////////////////////////////////////////////////////////////////
function print_job($contacts_id,$job_num,$start_time,$end_time,$return_to="",$noedit=0) {

$start_info=date_info($start_time);
$end_info=date_info($end_time);
$job_num=escapeshellcmd($job_num);
$job_num_text=ereg_replace("- *$","",$job_num);

$contact_info=getone("select * from contacts where contacts_id = '$contacts_id'");

$query="select * from timesheet_time,contacts where employee_id = '$contacts_id' and job_num = '$job_num' and workday >= '$start_info->date_mysql' and workday <= '$end_info->date_mysql' and contacts_id = creator order by timesheet_time_id";
$res=@mysql_query($query);

$sum_query="select sum(hours) as total_hours from timesheet_time where employee_id = '$contacts_id' and job_num = '$job_num' and workday >= '$start_info->date_mysql' and workday <= '$end_info->date_mysql'";
$job_sum=getoneb($sum_query);

	$job_sum->sum_st=0;
	$job_sum->sum_ot=0;
	$job_sum->sum_dt=0;
	$job_sum->sum_v=0;
	$job_sum->sum_s=0;
	$job_sum->sum_h=0;
	$job_sum->subs="0.00";
	$job_sum->park="0.00";
	$job_sum->trav="0.00";
	$job_sum->exps="0.00";
echo "<tr><td colspan=7><hr></td></tr>";
while($row=@mysql_fetch_object($res)) {
	$row->job_num=ereg_replace("- *$","",$row->job_num);
	$row->job_num=ereg_replace("- *$","",$row->job_num);
	$workdate=ereg_replace("-","/",invali_date($row->workday));
	$ectype="ERROR!";
	switch ($row->earnings_code) {
		case 1:
			$ectype="ST";$job_sum->sum_st=$job_sum->sum_st + $row->hours;break;
		case 2:
			$ectype="OT";$job_sum->sum_ot=$job_sum->sum_ot + $row->hours;break;
		case 3:
			$ectype="DT";$job_sum->sum_dt=$job_sum->sum_dt + $row->hours;break;
		case 8:
			$ectype="V&nbsp;";$job_sum->sum_v=$job_sum->sum_v + $row->hours;break;
		case 9:
			$ectype="S&nbsp;";$job_sum->sum_s=$job_sum->sum_s + $row->hours;break;
		case 10:
			$ectype="H&nbsp;";$job_sum->sum_h=$job_sum->sum_h + $row->hours;break;
		case 5:
			$row->phase_desc="Subsistence";
			$ectype="$";$row->hours=$row->dollars;
			$job_sum->subs=$_sum->subs + $row->dollars;
			$job_sum->exps=$job_sum->exps + $row->dollars;
			break;
		case 6:
			$row->phase_desc="Parking";
			$ectype="$";$row->hours=$row->dollars;
			$job_sum->park=$job_sum->park + $row->dollars;
			$job_sum->exps=$job_sum->exps + $row->dollars;
			break;
		case 7:
			$row->phase_desc="Travel";
			$ectype="$";$row->hours=$row->dollars;
			$job_sum->trav=$job_sum->trav + $row->dollars;
			$job_sum->exps=$job_sum->exps + $row->dollars;
			break;
		}
	$row_description="$row->job_desc - $row->phase_desc";
	$row_description=substr($row_description,0,45);

		
	echo "
	<tr valign=top><td>
		<tt><b>$row->job_num&nbsp;</b></tt>
	</td><td>
		<tt>$row->phase&nbsp;</tt>
	</td><td>
		<tt>$workdate&nbsp</tt>
	</td><td>
		<font size=-2>$row_description</font>
	</td><td>
		<tt>$row->login_name</tt>
	</td><td align=right>
		<tt>$ectype&nbsp;$row->hours</tt>
	</td><td align=right>
		<tt>$row->shift</tt>
	";
	echo "</td><td>";
	if ($row->comments!="") echo "<a href=javascript:shownotes($row->timesheet_time_id)>N</a>";
	if ($noedit==0) {
		if ($ectype=='$')
		echo "</td><td><tt><a href=javascript:dte('$row->timesheet_time_id');><font size=-1 color=blue><i>Delete</i></font></a></tt>";
		else
		echo "</td><td><tt><a href=javascript:ete('$row->timesheet_time_id');><font size=-1 color=blue><i>Edit</i></font></a></tt>";
	}
	echo "</td></tr>";
	
	}
$job_sum->total_hours++;
$job_sum->total_hours--;

$job_sum->sum_st++;$job_sum->sum_st--;
$job_sum->sum_ot++;$job_sum->sum_ot--;
$job_sum->sum_dt++;$job_sum->sum_dt--;
$job_sum->sum_v++;$job_sum->sum_v--;
$job_sum->sum_s++;$job_sum->sum_s--;
$job_sum->sum_h++;$job_sum->sum_h--;

$job_sum->subs=sprintf('%.2f',$job_sum->subs);
$job_sum->park=sprintf('%.2f',$job_sum->park);
$job_sum->trav=sprintf('%.2f',$job_sum->trav);
$job_sum->exps=sprintf('%.2f',$job_sum->exps);

if ($job_sum->subs!=0) echo "<tr><td colspan=5 width=100% align=right><tt>Subsistence:</tt></td><td align=right><tt>\$$job_sum->subs</tt></td></tr>";
if ($job_sum->park!=0) echo "<tr><td colspan=5 width=100% align=right><tt>Parking:</tt></td><td align=right><tt>\$$job_sum->park</tt></td></tr>";
if ($job_sum->trav!=0) echo "<tr><td colspan=5 width=100% align=right><tt>Travel:</tt></td><td align=right><tt>\$$job_sum->trav</tt></td></tr>";
if ($job_sum->exps!=0) echo "<tr><td colspan=5 width=100% align=right><tt>Total&nbsp;S/P/T:</tt></td><td align=right><tt>\$$job_sum->exps</tt></td></tr>";

if ($job_sum->sum_st!=0) echo "<tr><td colspan=5 width=100% align=right><tt>Straight&nbsp;Time&nbsp;Hours:</tt></td><td align=right><tt><b>$job_sum->sum_st</b></tt></td></tr>";
if ($job_sum->sum_ot!=0) echo "<tr><td colspan=5 width=100% align=right><tt>Overtime&nbsp;Hours:</tt></td><td align=right><tt><b>$job_sum->sum_ot</b></tt></td></tr>";
if ($job_sum->sum_dt!=0) echo "<tr><td colspan=5 width=100% align=right><tt>Doubletime&nbsp;Hours:</tt></td><td align=right><tt><b>$job_sum->sum_dt</b></tt></td></tr>";
if ($job_sum->sum_v!=0) echo "<tr><td colspan=5 width=100% align=right><tt>Vacation&nbsp;Hours:</tt></td><td align=right><tt><b>$job_sum->sum_v</b></tt></td></tr>";
if ($job_sum->sum_s!=0) echo "<tr><td colspan=5 width=100% align=right><tt>Sick&nbsp;Hours:</tt></td><td align=right><tt><b>$job_sum->sum_s</b></tt></td></tr>";
if ($job_sum->sum_h!=0) echo "<tr><td colspan=5 width=100% align=right><tt>Holiday&nbsp;Hours:</tt></td><td align=right><tt><b>$job_sum->sum_h</b></tt></td></tr>";
 
return($job_sum);

}












/////////////////////////////////////////////////////////////////////////////////////////
function print_job2($contacts_id,$job_num,$start_time,$end_time,$return_to="",$noedit=0) {

$start_info=date_info($start_time);
$end_info=date_info($end_time);
$job_num=escapeshellcmd($job_num);
$job_num_text=ereg_replace("- *$","",$job_num);

$contact_info=getone("select * from contacts where contacts_id = '$contacts_id'");

$query="select * from timesheet_time,contacts where employee_id = '$contacts_id' and job_num = '$job_num' and workday >= '$start_info->date_mysql' and workday <= '$end_info->date_mysql' and contacts_id = creator order by timesheet_time_id";
$res=@mysql_query($query);

$sum_query="select sum(hours) as total_hours from timesheet_time where employee_id = '$contacts_id' and job_num = '$job_num' and workday >= '$start_info->date_mysql' and workday <= '$end_info->date_mysql'";
//$sum_query="select sum(hours) as total_hours from timesheet_time where employee_id = '$contacts_id' and workday = '$day_info->date_mysql' order by timesheet_time_id";
$job_sum=getoneb($sum_query);

echo "<tt>
<table border=0 cellpadding=0 cellspacing=0 width=100%>
<tr><td bgcolor=#dddddd border=1>
	<table border=1 cellpadding=1 cellspacing=0><tr><td><tt>
		<font color=black>$job_num_text</font></tt></td></tr></table>
	</td><td width=100% valign=bottom>
	<sub><hr valign=bottom></sub>
</td></tr>
</table>
";

echo "<table border=0 cellpadding=0>";
if (mysql_num_rows($res)>0) {
	echo "
	<tr><td>
		<tt><b>Job&nbsp;#</b></tt>
	</td><td>
		<tt><b>Phase</b></tt>
	</td><td>
		<tt><b>Date</b></tt>
	</td><td>
		<tt><b>Description</b></tt>
	</td><td>
		<tt><b>Input</b></tt>
	</td><td>
		<tt><b>Hours</b></tt>
	</td><td>
		<tt><b>Sh</b></tt>
	</td></tr>
	";
	}

	$day_sum->sum_st=0;
	$day_sum->sum_ot=0;
	$day_sum->sum_dt=0;
	$day_sum->sum_v=0;
	$day_sum->sum_s=0;
	$day_sum->sum_h=0;
	$day_sum->subs="0.00";
	$day_sum->park="0.00";
	$day_sum->trav="0.00";
	$day_sum->exps="0.00";
while($row=@mysql_fetch_object($res)) {
	$row->job_num=ereg_replace("- *$","",$row->job_num);
	$row->job_num=ereg_replace("- *$","",$row->job_num);
	$workdate=invali_date($row->workday);
	$ectype="ERROR!";
	switch ($row->earnings_code) {
		case 1:
			$ectype="ST";$day_sum->sum_st=$day_sum->sum_st + $row->hours;break;
		case 2:
			$ectype="OT";$day_sum->sum_ot=$day_sum->sum_ot + $row->hours;break;
		case 3:
			$ectype="DT";$day_sum->sum_dt=$day_sum->sum_dt + $row->hours;break;
		case 8:
			$ectype="V&nbsp;";$day_sum->sum_v=$day_sum->sum_v + $row->hours;break;
		case 9:
			$ectype="S&nbsp;";$day_sum->sum_s=$day_sum->sum_s + $row->hours;break;
		case 10:
			$ectype="H&nbsp;";$day_sum->sum_h=$day_sum->sum_h + $row->hours;break;
		case 5:
			$row->phase_desc="Subsistence";
			$ectype="$";$row->hours=$row->dollars;
			$day_sum->subs=$day_sum->subs + $row->dollars;
			$day_sum->exps=$day_sum->exps + $row->dollars;
			break;
		case 6:
			$row->phase_desc="Parking";
			$ectype="$";$row->hours=$row->dollars;
			$day_sum->park=$day_sum->park + $row->dollars;
			$day_sum->exps=$day_sum->exps + $row->dollars;
			break;
		case 7:
			$row->phase_desc="Travel";
			$ectype="$";$row->hours=$row->dollars;
			$day_sum->trav=$day_sum->trav + $row->dollars;
			$day_sum->exps=$day_sum->exps + $row->dollars;
			break;
		}
	$row_description="$row->job_desc - $row->phase_desc";
	$row_description=substr($row_description,0,59);
	
	echo "
	</tt></tt></tt></tt></tt>
	<tr valign=top><td>
		<tt>$row->job_num</tt>
	</td><td>
		<tt>$row->phase</tt>
	</td><td>
		<tt>$workdate</tt>
	</td><td>
		<font size=-1>$row_description</font>
	</td><td>
		<tt>$row->login_name</tt>
	</td><td align=right>
		<tt>$ectype&nbsp;$row->hours</tt>
	</td><td align=center>
		<tt>$row->shift</tt>
	";
	echo "</td><td>";
	if ($row->comments!="") echo "<a href=javascript:shownotes($row->timesheet_time_id)>N</a>";
	if ($noedit==0) {
		if ($ectype=='$') echo "</td><td><tt><a href=javascript:dte('$row->timesheet_time_id');><font size=-1 color=blue><i>Delete</i></font></a></tt>";
		else echo "</td><td><tt><a href=javascript:ete('$row->timesheet_time_id');><font size=-1 color=blue><i>Edit</i></font></a></tt>";
		}
	echo "</td></tr>";
	
	}
$day_sum->total_hours++;
$day_sum->total_hours--;

$day_sum->sum_st++;$day_sum->sum_st--;
$day_sum->sum_ot++;$day_sum->sum_ot--;
$day_sum->sum_dt++;$day_sum->sum_dt--;
$day_sum->sum_v++;$day_sum->sum_v--;
$day_sum->sum_s++;$day_sum->sum_s--;
$day_sum->sum_h++;$day_sum->sum_h--;

$day_sum->subs=sprintf('%.2f',$day_sum->subs);
$day_sum->park=sprintf('%.2f',$day_sum->park);
$day_sum->trav=sprintf('%.2f',$day_sum->trav);
$day_sum->exps=sprintf('%.2f',$day_sum->exps);

if ($day_sum->subs!=0) echo "<tr><td colspan=4 width=100% align=right><tt>Subsistence:</tt></td><td align=right><tt>\$$day_sum->subs</tt></td></tr>";
if ($day_sum->park!=0) echo "<tr><td colspan=4 width=100% align=right><tt>Parking:</tt></td><td align=right><tt>\$$day_sum->park</tt></td></tr>";
if ($day_sum->trav!=0) echo "<tr><td colspan=4 width=100% align=right><tt>Travel:</tt></td><td align=right><tt>\$$day_sum->trav</tt></td></tr>";
if ($day_sum->exps!=0) echo "<tr><td colspan=4 width=100% align=right><tt>Total&nbsp;S/P/T:</tt></td><td align=right><tt>\$$day_sum->exps</tt></td></tr>";

if ($day_sum->sum_st!=0) echo "<tr><td colspan=4 width=100% align=right><tt>Straight&nbsp;Time&nbsp;Hours:</tt></td><td align=right><tt>$day_sum->sum_st</tt></td></tr>";
if ($day_sum->sum_ot!=0) echo "<tr><td colspan=4 width=100% align=right><tt>Overtime&nbsp;Hours:</tt></td><td align=right><tt>$day_sum->sum_ot</tt></td></tr>";
if ($day_sum->sum_dt!=0) echo "<tr><td colspan=4 width=100% align=right><tt>Doubletime&nbsp;Hours:</tt></td><td align=right><tt>$day_sum->sum_dt</tt></td></tr>";
if ($day_sum->sum_v!=0) echo "<tr><td colspan=4 width=100% align=right><tt>Vacation&nbsp;Hours:</tt></td><td align=right><tt>$day_sum->sum_v</tt></td></tr>";
if ($day_sum->sum_s!=0) echo "<tr><td colspan=4 width=100% align=right><tt>Sick&nbsp;Hours:</tt></td><td align=right><tt>$day_sum->sum_s</tt></td></tr>";
if ($day_sum->sum_h!=0) echo "<tr><td colspan=4 width=100% align=right><tt>Holiday&nbsp;Hours:</tt></td><td align=right><tt>$day_sum->sum_h</tt></td></tr>";
 

//echo "<tr><td width=100% colspan=4 align=right><tt>Total&nbsp;Hours:</tt></td><td align=right><tt>$day_sum->total_hours</tt></td></tr>";

echo "</table></tt>";

return($day_sum);

}

/////////////////////////////////////////////////////////////////////////////////////////
function print_tab($employee_id,$date,$tabtext="",$highlight_date="") {
	$day_info=date_info($date);
	if ($highlight_date=="") $highlight_date=date("m-d-Y");
	if ($day_info->date==$highlight_date) $bgcolor="#dddddd";
		else $bgcolor="#ffffff";
	if ($tabtext=="") $tabtext=substr($day_info->day_of_week,0,1);
	$day_sum=getoneb("select sum(hours) as total_hours from timesheet_time where employee_id = '$employee_id' and workday = '$day_info->date_mysql'");
	$day_sum->total_hours++;
	$day_sum->total_hours--;
	echo "<td><table border=1 cellpadding=3 cellspacing=0><tr><td bgcolor=$bgcolor>
	<a title=\"$day_info->date_short\" href=$pagename?mode=timesheet_edit&timesheet_mode_new=timesheet_showday&showday=$day_info->date><font color=black>
	<tt>$tabtext&nbsp;($day_sum->total_hours)</tt>
	</font></a>
	</td></tr></table><td valign=bottom><sub>_</sub></td>";
	}

/////////////////////////////////////////////////////////////////////////////////////////
function print_tab_summary($employee_id,$week_end_date,$tabtext="",$highlight_date="") {
	global $timesheet_info;
	$day_info=date_info($week_end_date);
	if ($highlight_date=="summary") $bgcolor="#dddddd";
		else {
			if ($timesheet_info->status=='new') $bgcolor="yellow";
			else $bgcolor="#ffffff";
			}
	if ($tabtext=="") 
		{
		$tabtext="Totals/Review";
		//if (($highlight_date!="summary")&&($timesheet_info->status == 'new')) $bgcolor="yellow";
		}
	$sum_query="select sum(hours) as total_hours from timesheet_time where employee_id = '$employee_id' and workday >= '$day_info->week_start_mysql' and workday <= '$day_info->week_end_mysql'";
	$weeks_hours=getone($sum_query);
	if ($weeks_hours->total_hours=="") $weeks_hours->total_hours="0";
	echo "<td><table border=1 cellpadding=3 cellspacing=0><tr><td bgcolor=$bgcolor>
	<a href=$pagename?mode=timesheet_edit&timesheet_mode_new=timesheet_summary><font color=black>
	<tt>$tabtext:&nbsp;$weeks_hours->total_hours</tt>
	</font></a>
	</td></tr></table><td valign=bottom><sub>_</sub></td>";
	}

/////////////////////////////////////////////////////////////////////////////////////////
/*
function date_info($date="") {
if ($date=="") $date=date("m-d-Y");
$vali_date=vali_date($date);

$day=new bsclass;
$day->unixtime=strtotime($vali_date);
$day->unixtime2=$day->unixtime + 14400;
$day->day_of_week=date("D",$day->unixtime2);
$day->day_of_week_long=date("l",$day->unixtime2);
$day->date=$date;
$day->date_slashes=ereg_replace("-","/",$date);
$day->day_of_week_num=date("w",$day->unixtime2);

$day->week_end_unixtime=strtotime("saturday",$day->unixtime2);
$day->week_end_unixtime2=$day->week_end_unixtime + 14400;
$day->week_end=date("m-d-Y",$day->week_end_unixtime2);
$day->week_end_slashes=date("m/d/Y",$day->week_end_unixtime2);
$day->week_end_mysql=date("Y-m-d",$day->week_end_unixtime2);
$day->next_week_end_unixtime2=$day->week_end_unixtime2 + 604800;
$day->next_week_end=date("m-d-Y",$day->next_week_end_unixtime2);
$day->next_week_end_mysql=date("Y-m-d",$day->next_week_end_unixtime2);
$day->last_week_end_unixtime2=$day->week_end_unixtime2 - 604800;
$day->last_week_end=date("m-d-Y",$day->last_week_end_unixtime2);
$day->last_week_end_mysql=date("Y-m-d",$day->last_week_end_unixtime2);

$day->week_start_unixtime=strtotime("-6 days",$day->week_end_unixtime2);
$day->week_start_unixtime2=$day->week_start_unixtime + 14400;
$day->week_start=date("m-d-Y",$day->week_start_unixtime2);
$day->week_start_slashes=date("m/d/Y",$day->week_start_unixtime2);
$day->week_start_mysql=date("Y-m-d",$day->week_start_unixtime2);
$day->next_week_start_unixtime2=$day->week_start_unixtime2 + 604800;
$day->next_week_start=date("m-d-Y",$day->next_week_start_unixtime2);
$day->next_week_start_mysql=date("Y-m-d",$day->next_week_start_unixtime2);
$day->last_week_start_unixtime2=$day->week_start_unixtime2 - 604800;
$day->last_week_start=date("m-d-Y",$day->last_week_start_unixtime2);
$day->last_week_start_mysql=date("Y-m-d",$day->last_week_start_unixtime2);

$day->date_mysql=date("Y-m-d",$day->unixtime2);
$day->date_short=date("m-d",$day->unixtime2);
$day->date_short_slash=date("m/d",$day->unixtime2);
$day->next_unixtime=strtotime("+1 days",$day->unixtime2);
$day->next_unixtime2=$day->next_unixtime + 14400;
$day->next=date("m-d-Y",$day->next_unixtime2);
$day->previous_unixtime=strtotime("-1 days",$day->unixtime2);
$day->previous_unixtime2=$day->previous_unixtime + 14400;
$day->previous=date("m-d-Y",$day->previous_unixtime2);
$day->prev=$day->previous;

return $day;
}
*/



/////////////////////////////////////////////////////////////////////////////////////////
function query_underlings($employee_num) {
	$query="select * from HRRM with (NOLOCK) where HRCo = 1 and udsupervisor = $employee_num and ActiveYN = 'Y' order by LastName, FirstName";
	return (@mssql_query($query));
}

function query_underlingsb($contacts_id) {
	$query="
	select * from timesheet_time,timesheet_pm_job_audit_list where 
		
		timesheet_time.job_num like concat(timesheet_pm_job_audit_list.job_prefix,'%')
		and pm_id = '$contacts_id'
		and (timesheet_status = 'New' or timesheet_status = 'Complete' or timesheet_status = 'Processing')
		and employee_group = 1
		group by employee_num
		";
	//tabledump($query);
	return (@mysql_query($query));
}

/////////////////////////////////////////////////////////////////////////////////////////
function dump_underlings($employee_num) {
	$query="select * from HRRM with (NOLOCK) where HRCo = 1 and udsupervisor = $employee_num and ActiveYN = 'Y' order by LastName, FirstName";
	//echo "<hr>$query<hr>";
	ms_tabledump($query);
}

/////////////////////////////////////////////////////////////////////////////////////////
function i_have_underlings($employee_num) {
	$pm_info=getoneb("select * from contacts where employee_num = '$employee_num'");
	$query="select * from timesheet_pm_job_audit_list where pm_id = $pm_info->contacts_id";
	if (getoneb($query)) return TRUE;
	$query="select * from HRRM with (NOLOCK) where HRCo = 1 and udsupervisor = $employee_num and ActiveYN = 'Y' order by LastName, FirstName";
	$res=@mssql_query($query);
	if ((mssql_num_rows($res))>0) return TRUE;
	else return FALSE;
}

function find_supervisor($employee_num) {
	$query="select udsupervisor from HRRM with (NOLOCK) where HRCo = 1 and ActiveYN = 'Y' and PREmp = '$employee_num'";
	$res=ms_getoneb($query);
	return getoneb("select * from contacts where employee_num = '$res->udsupervisor'");
	}

function completed_timesheet_email($supervisor,$employee) {
	$message_text="

Hello $supervisor->first_name $supervisor->last_name,

	This message is to inform you that $employee->first_name $employee->last_name has just
completed a timesheet. You should proceed to the timesheet page 
to either approve or reject their timesheet now.

https://pipeline.umci.com/global/timesheet/

";	
	//echo $message_text;	
	
	mail ( $supervisor->email,"Timesheet for $employee->first_name $employee->last_name ($employee->employee_num) complete", $message_text, "From: Web Timesheet Notification <webmaster@umci.com>");
	
	}

/////////////////////////////////////////////////////////////////////////////////////////
function is_valid_viewpoint_job($job_num) {
$job_num=strtoupper(escapeshellcmd($job_num));
$job_num_p1=str_pad(ereg_replace("-.*","",$job_num),5,"0",STR_PAD_LEFT);
if ($job_num_p1=="00000") return FALSE;
$job_num_p2=ereg_replace(".*-","",$job_num); 

if ($job_num_p1==$job_num_p2) $job_num_p2="";
if ($job_num_p2=="") {
	$job_str_query1="$job_num_p1-";
} else {
	$job_num_p2=str_pad($job_num_p2,3," ",STR_PAD_LEFT);
	$job_str_query1="$job_num_p1-$job_num_p2";
	$job_num_p2=ereg_replace(" ","0",$job_num_p2);
	$job_str_query2="$job_num_p1-$job_num_p2";
	if ($job_str_query1==$job_str_query2) $job_str_query2="";
}

$job_query1="select * from JCJM with (NOLOCK) where JobStatus = 1 and JCCo = 1 and Job = '$job_str_query1'";
$job_res=ms_getoneb($job_query1);


if ($job_res) {
	$job_res->job=$job_str_query1;		
	$job_res->job_query=$job_query1;
	$job_res->Description=escapeshellcmd($job_res->Description);
	} else {
	if ($job_str_query2!="") {
		$job_query2="select * from JCJM with (NOLOCK) where JobStatus = 1 and JCCo = 1 and Job = '$job_str_query2'";
		$job_res=ms_getoneb($job_query2);
		if ($job_res) {
			$job_res->job=$job_str_query2;
			$job_res->job_query=$job_query2;
			$job_res->Description=escapeshellcmd($job_res->Description);
			}
		}
	}
return $job_res;
}

/////////////////////////////////////////////////////////////////////////////////////////
function is_viewpoint_job($job_num) {
$job_num=strtoupper(escapeshellcmd($job_num));
$job_num_p1=str_pad(ereg_replace("-.*","",$job_num),5,"0",STR_PAD_LEFT);
if ($job_num_p1=="00000") return FALSE;
$job_num_p2=ereg_replace(".*-","",$job_num); 

if ($job_num_p1==$job_num_p2) $job_num_p2="";
if ($job_num_p2=="") {
	$job_str_query1="$job_num_p1-";
} else {
	$job_num_p2=str_pad($job_num_p2,3," ",STR_PAD_LEFT);
	$job_str_query1="$job_num_p1-$job_num_p2";
	$job_num_p2=ereg_replace(" ","0",$job_num_p2);
	$job_str_query2="$job_num_p1-$job_num_p2";
	if ($job_str_query1==$job_str_query2) $job_str_query2="";
}
$job_query1="select * from JCJM with (NOLOCK) where JCCo = 1 and Job = '$job_str_query1'";
$job_res=ms_getoneb($job_query1);
if ($job_res) {
	$job_res->job=$job_str_query1;		
	$job_res->job_query=$job_query1;
	$job_res->Description=escapeshellcmd($job_res->Description);
	} else {
	if ($job_str_query2!="") {
		$job_query2="select * from JCJM with (NOLOCK) where JCCo = 1 and Job = '$job_str_query2'";
		$job_res=ms_getoneb($job_query2);
		if ($job_res) {
			$job_res->job=$job_str_query2;
			$job_res->job_query=$job_query2;
			$job_res->Description=escapeshellcmd($job_res->Description);
			}
		}
	}
return $job_res;
}

/////////////////////////////////////////////////////////////////////////////////////////
function is_valid_viewpoint_spt_phase($phase,$job_num){
	$phase=strtoupper(escapeshellcmd($phase));
	$phase=ereg_replace("-.*","",$phase);
	$phase=str_pad($phase,5,"0",STR_PAD_LEFT);
	
	$phases_locked=ms_getoneb("select * from JCJM with (NOLOCK) where JCCo = 1 and Job = '$job_num' and LockPhases = 'Y'");
	if ($phases_locked) {
		$query="select * from JCJP with (NOLOCK) INNER JOIN JCCH with (NOLOCK) on JCJP.JCCo = JCCH.JCCo and JCJP.Job = JCCH.Job and JCJP.Phase = JCCH.Phase where JCJP.Job = '$job_num' and JCJP.Phase LIKE '$phase- %' and JCJP.JCCo = 1 and JCJP.ActiveYN = 'Y' and JCCH.CostType = '8' and JCJP.Phase not like 'YYYYY%'";
		} else {
		$query="select * from JCPM with (NOLOCK) left outer join JCPC with (NOLOCK) on JCPM.PhaseGroup = JCPC.PhaseGroup and JCPM.Phase = JCPC.Phase 
		        where JCPC.CostType = 8 and JCPM.PhaseGroup = 1 and JCPM.Phase not like 'YYYY%' and JCPC.Phase LIKE '$phase- %'";
		}
	$phase_ret=ms_getoneb($query);
	if ($phase_ret) {
		$phase_ret->phase=$phase;
		$phase_ret->query=$query;
		$phase_ret->Description=escapeshellcmd($phase_ret->Description);
		}

return($phase_ret);
}

/////////////////////////////////////////////////////////////////////////////////////////
function is_valid_viewpoint_labor_phase($phase,$job_num){
	$phase=strtoupper(escapeshellcmd($phase));
	$phase_chunk=explode("-",$phase);
	
	$phase_front=ereg_replace(" ","",$phase_chunk[0]);
	$phase_mid=ereg_replace(" ","",$phase_chunk[1]);
	
	$phase_front=str_pad($phase_front,5,"0",STR_PAD_LEFT);
	$phase_mid=str_pad($phase_mid,3," ",STR_PAD_RIGHT);
	
	$phases_locked=ms_getoneb("select * from JCJM with (NOLOCK) where JCCo = 1 and Job = '$job_num' and LockPhases = 'Y'");

	$phase="$phase_front-$phase_mid-";
	$pretty_phase=ereg_replace("[- ]*$","",$phase);
	
	if ($phases_locked) {
		$query="select * from JCJP with (NOLOCK) INNER JOIN JCCH with (NOLOCK) on JCJP.JCCo = JCCH.JCCo and JCJP.Job = JCCH.Job and JCJP.Phase = JCCH.Phase where JCJP.Job = '$job_num' and JCJP.Phase LIKE '$phase%' and JCJP.JCCo = 1 and JCJP.ActiveYN = 'Y' and JCCH.CostType = '1'";
		$phase_ret=ms_getoneb($query);
		} else {
		$query="select * from JCPC with (NOLOCK),JCPM with (NOLOCK) where JCPC.Phase = JCPM.Phase and JCPC.CostType = 1 and JCPC.PhaseGroup = 1 and JCPM.PhaseGroup = 1 and JCPC.Phase LIKE '$phase%'";
		$phase_ret=ms_getoneb($query);
		}

	if ($phase_ret) {
		$phase_ret->phase=$pretty_phase;
		$phase_ret->query=$query;
		$phase_ret->Description=escapeshellcmd($phase_ret->Description);
		}

return($phase_ret);
}

/////////////////////////////////////////////////////////////////////////////////////////
function show_viewpoint_job_phases($job_num) {
	

	$phases_locked=ms_getoneb("select * from JCJM with (NOLOCK) where JCCo = 1 and Job = '$job_num' and LockPhases = 'Y'");
	
	if ($phases_locked) {
		$query="select Phase,Description from JCJP with (NOLOCK) where Job = '$job_num' and JCCo = 1 and ActiveYN = 'Y'";
		} else {
		$query="select * from JCPC with (NOLOCK),JCPM with (NOLOCK) where JCPC.JCCo = JCPM.JCCo and JCPC.JCCo = 1 and JCPC.Phase = JCPM.Phase and JCPC.CostType = 1 and JCPC.PhaseGroup = 1 and JCPM.PhaseGroup = 1";
		}

	$res=@mssql_query($query);

	echo "<table border=0><tr bgcolor=#dddddd><td>Phase Code</td><td>Description</td></tr>";
	while($row=@mssql_fetch_object($res)) {
		$row->Phase=ereg_replace("-.*$","",$row->Phase);
		echo "<tr><td align=center>$row->Phase</td><td>$row->Description</td></tr>";
		}
	echo "</table>";
}

/////////////////////////////////////////////////////////////////////////////////////////
function verify_timesheet_hours($hours){
	$hours1=escapeshellcmd(round($hours,2));
	$hours2=floor($hours);
	if ($hours1==$hours2) return($hours1);
	$diff=$hours1 - $hours2;
	$diff="$diff";
	$prod=$diff * 100;
	$prod="$prod";
	$modextra=$prod % 25;
if ((($prod)%25)=="0")
	return $hours1;
	else
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////
function verify_timesheet_shift($shift) {
	if (($shift==1)||($shift==2)||($shift==3))
		return $shift;
		else
		return FALSE;
	}

$today_info=date_info();


$suid_check=getoneb("select * from timesheet_types where contacts_id = '$global_user->contacts_id' and contact_type = 'suid'");
$billing_check=getoneb("select * from timesheet_types where contacts_id = '$global_user->contacts_id' and contact_type = 'billing'");
$payroll_check=getoneb("select * from timesheet_types where contacts_id = '$global_user->contacts_id' and contact_type = 'payroll'");
$admin_check=getoneb("select * from timesheet_types where contacts_id = '$global_user->contacts_id' and contact_type = 'admin'");
$timesheet_data_entry_check=getoneb("select * from timesheet_types where contacts_id = '$global_user->contacts_id' and contact_type = 'timesheet_data_entry'");

?>
