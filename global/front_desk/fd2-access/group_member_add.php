<?

if ($group_id=="") die("Application error: no group_id specified!");
else $group_id=addslashes($group_id);

// This is just to make sure it's a valid group
$group_info=getone("select * from contacts_groups where group_id = '$group_id'");

if ($contacts_id == "") die("Application error: no contact specified!");
else $contacts_id=addslashes($contacts_id);

security_log_email(
$contacts_id,
$group_info->group_id,
$group_info->jobinfo_id,
"contact added to group",
"contact groups"
);
                        


$query="insert into contacts_groups_members set contacts_id = '$contacts_id', group_id = '$group_id'";
@mysql_query($query);

forward("group_members&group_id=$group_id");
?>
