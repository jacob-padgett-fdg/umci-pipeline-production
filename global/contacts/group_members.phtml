<?
if ($group_id=="") die("Application error: not group id specified!");
else $group_id=addslashes($group_id);

$group_info=getone("select * from contacts_groups where group_id = '$group_id'");

$lgreq="";
$emailreq="";
if ($group_info->login_required=='Y') $lgreq=" and login_name != ''";
if ($group_info->email_required=='Y') $emailreq=" and email != ''";
if ($group_info->umc_employee_required=='Y') $empreq=" and umc_emp = 'Y'";
if ($group_info->current_contact_required=='Y') $curreq=" and contacts.current = 'Y'";

$jobinfo=getoneb("select * from jobinfo where jobinfo_id = '$group_info->jobinfo_id'");


if ($jobinfo) 
	$title="<b>$jobinfo->contract_num/$group_info->description members<br><i>($jobinfo->job_name)</i></b>";
	else 
	$title="<b>$group_info->description members</b>";
echo "
<a href=$pagename?mode=group_management&jobinfo_id=$group_info->jobinfo_id><font color=blue>Return to groups</font></a>
<form name=group_member_add method=post action=$pagename>
<input type=hidden name=mode value=group_member_add>
<input type=hidden name=group_id value=\"$group_id\">
<b>Add:&nbsp;</b>";
//contactsbox("select * from contacts where 1 = 1 $lgreq $emailreq $empreq $curreq","","contacts_id","opener.document.group_member_add.submit()",2);
contact("select * from contacts where 1 = 1 $lgreq $emailreq $empreq $curreq","","contacts_id","group_member_add.submit()");
echo"</form>
<table border=1 cellspacing=0 cellpadding=5>
<tr><td colspan=2 bgcolor=\"$umc_standard_blue\" align=center>
	$title
</td></tr>

<form name=fooberry method=post action=$pagename>
";

$query="select * from contacts_groups_members,contacts where contacts_groups_members.group_id = '$group_id' and contacts_groups_members.contacts_id = contacts.contacts_id $curreq order by display_name";
$res=@mysql_query($query);
while ($row=@mysql_fetch_object($res)) {
	echo "
	<tr><td>";
		contactsbox("","$row->contacts_id","foo$row->membership_id","",1);echo"
	</td><td>
		<a href=$pagname?mode=group_member_remove&membership_id=$row->membership_id><font color=blue><i>Remove</i></font></a>
	</td></tr>";
	}
echo "</form></table>";
?>
