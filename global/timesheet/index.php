<?
require("settings.cfg");
require("global-auth.inc");
//if ($global_user->contacts_id==2) $global_user->employee_num="8494";
//if ($global_user->contacts_id==2) $global_user->employee_num="9532";
require("timesheet_libs.inc");

//if ($global_contacts_id='353'||$global_contacts_id=='4517'||$global_contacts_id='2'||$global_contacts_id='1') die("testing!!!");

if ($global_contacts_id='353'||$global_contacts_id=='4517'||$global_contacts_id='2'||$global_contacts_id='1') include("viewpoint_connect_ro_pr.phtml");
else include("viewpoint_connect_ro.phtml");

require("../front_desk/reports/report_lib.inc");

if ($current_user_id=="") {
	session_register('current_user_id');
	$current_user_id=$global_user->contacts_id;
	}
$current_user=getoneb("select * from contacts where contacts_id = '$current_user_id'");

if (($current_user->employee_num <=1)||(($current_user->employee_group!='3')&&($current_user->employee_group!='1'))) {
	include("employee_number_acquire.phtml");
	$current_user=getoneb("select * from contacts where contacts_id = '$current_user_id'");
	}

//if ($current_user->employee_num <=1) {
//	include("employee_number_required.phtml");
//	exit;
//	}

if ($current_user_id != $last_user_id) {
	session_register('last_user_id');
	
	if ($current_user->employee_group=='3') {
		$exempt=ms_getoneb("select HRPC.Type from HRPC with (NOLOCK) inner join HRRM with (NOLOCK) on HRPC.PositionCode = HRRM.PositionCode where HRPC.HRCo = 1 and HRRM.HRCo = 1 and HRRM.PREmp = $current_user->employee_num");
		if (!($exempt)) die ("ERROR: Error detecting exemption status. No record found. <p>Someone in HR probably needs to set HR Resource Master -> Info -> Position Code to something available on the \"F4\" menu.");
		if ($exempt->Type=="N") $tquery="update contacts set overtime_exempt = 'N' where contacts_id = '$current_user_id'";
		if ($exempt->Type=="E") $tquery="update contacts set overtime_exempt = 'Y' where contacts_id = '$current_user_id'";
		if (($exempt->Type!="N")&&($exempt->Type!="E")) die ("ERROR: Error determining overtime exemption status. Please contact your system administrator.<p>");
	} else {
		$tquery="update contacts set overtime_exempt = 'N' where contacts_id = '$current_user_id'";
	}
	$tres=@mysql_query($tquery);
	if (!($tres)) die("ERROR: Error updating overtime exemption status. Please contact your system administrator.<p>Failed Query:<hr>$tquery");
	$last_user_id=$current_user_id;
	}

if (($current_employee_group!='1') && ($current_employee_group!='3')) {
	session_register('current_employee_group');
	$current_employee_group="3";
	}

if (isset($mode)) {
	if (is_readable("$mode.phtml")) { 
		include("$mode.phtml");  } else { include ("badmode.phtml"); }
	} else {
	include ("main.phtml");
	}
?>
