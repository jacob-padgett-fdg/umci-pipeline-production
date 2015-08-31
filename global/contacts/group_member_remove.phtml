<?
if ($membership_id=="") die("Application error! Nothing to delete. Please contact your system administrator!");
else $membership_id=addslashes($membership_id);
$membership_info=getone("select * from contacts_groups_members where membership_id = '$membership_id'");

$group_info=getoneb("select * from contacts_groups where group_id = '$membership_info->group_id'");

security_log_email(
$membership_info->contacts_id,
$membership_info->group_id,
$group_info->jobinfo_id,
"contact removed from group",
"contact groups"
);


@mysql_query("delete from contacts_groups_members where membership_id = '$membership_info->membership_id'");
forward("group_members&group_id=$membership_info->group_id");
?>
