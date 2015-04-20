<?
if ($timesheet_id_new!="") {
	session_register('current_timesheet_id');
	$current_timesheet_id=escapeshellcmd($timesheet_id_new);
	}
verify_ts_integrity($current_timesheet_id);
$timesheet_info=getone("select * from timesheet_index where timesheet_id = '$current_timesheet_id'");

if ($force_timesheet_status_new=='Y') $timesheet_info->status="new";

if ($return_to_set != "") {
	session_register('return_to_once');
	$return_to_once=$return_to_set;
	}

if ($current_user->contacts_id!=$timesheet_info->employee_id) {
	echo "Error: timesheet's user id is not the same as current user's id! This error was probably caused
	by trying to edit more than employees timesheet at the same time (using multiple windows). Please only
	modify one employee's timesheet at a time. If this is a recurring error, please contact your system
	administrator.<p>
	<a href=$pagename?mode=main><font color=blue>Return to Main Menu</font></a><br>";
	exit;
	}
$week_end=invali_date($timesheet_info->week_ending);
$week_start=week_start($week_end);

$current_day=$week_start;
$tomorrow=next_day($current_day);

$counter=0;
while ($current_day != (next_day($week_end))) {
	$week_info[$counter]=new bsclass;
	$week_info[$counter]=date_info($current_day);
	$current_day=next_day($current_day);
	$counter++;
	}

?>
