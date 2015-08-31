<?
if ($group_id!="") {
	$group_id=addslashes($group_id);
	$query_start="update contacts_groups set ";
	$query_end=" where group_id = '$group_id'";
	} else {
	$query_start="insert into contacts_groups set ";
	$query_end="";
	}
if ($jobinfo_id!="") $jobinfo_id=addslashes($jobinfo_id);
$description=addslashes($description);
$login_required=checkfix($login_required);
$email_required=checkfix($email_required);
$umc_employee_required=checkfix($umc_employee_required);
$current_contact_required=checkfix($current_contact_required);
$active=checkfix($active);

$query_middle="
jobinfo_id = '$jobinfo_id',
description = '$description',
login_required = '$login_required',
email_required = '$email_required',
umc_employee_required = '$umc_employee_required',
current_contact_required = '$current_contact_required',
active = '$active'
";
$query = $query_start . $query_middle . $query_end;
//echo "<hr>$query<hr>";
$res=@mysql_query($query);
if(!($res)) die("Update failure - failed query:<hr>$query");

forward("group_management&jobinfo_id=$jobinfo_id");
?>
