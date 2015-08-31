<?
require('header.phtml');
if (($jobinfo_id!="")&&($jobinfo_id!=0)) {
	$jobinfo_id=addslashes($jobinfo_id);
	$job_info=getone("select * from jobinfo where jobinfo_id = '$jobinfo_id'");
	$title="Groups for $job_info->job_name ($job_info->contract_num)";
	} else {
	$title="Global Groups";
	$job_info->jobinfo_id="0";
	}
$query="select * from contacts_groups where jobinfo_id = '$job_info->jobinfo_id' order by description";

echo "
<a href=$pagename?mode=main><font color=blue>Main Menu</font></a>
<form name=job_selection method=get action=$pagename>
<input type=hidden name=mode value=group_management>
<b>Select Job (<i>none for global</i>):</b>&nbsp;";

dropbox("select jobinfo.jobinfo_id,contract_num,' - ',job_name 
from jobinfo,contacts_groups where  
	jobinfo.jobinfo_id = contacts_groups.jobinfo_id and 
	jobinfo.active = 'Y' and 
	jobinfo.jobinfo_id != 0 
	
	group by jobinfo_id
	
	order by contract_num, job_name","$job_info->jobinfo_id","","submit()",2);

echo "
</form>
<a href=\"$pagename?mode=group_edit&jobinfo_id=$job_info->jobinfo_id\"><font color=blue>Add</font></a>
<table border=1 cellspacing=0 cellpadding=5>
<tr><td bgcolor=$umc_standard_blue colspan=4>
	<b>$title</b>
</td></tr>

<tr bgcolor=#eeeeee><td>
	<b>Name</b>
</td><td>
	<b>Login Rqd</b>
</td><td>
	<b>Email Rqd</b>
</td><td>
	&nbsp;
</td></tr>
";



$result=@mysql_query($query);
while ($row=@mysql_fetch_object($result)) {
	if ($row->active=='Y') $textcolor="blue";
	else $textcolor="gray";
	if ($row->current_contact_required=="Y") {
		$delres=@mysql_query("select * from contacts_groups_members right join contacts on (contacts_groups_members.contacts_id = contacts.contacts_id) where group_id = '$row->group_id' and contacts.current = 'N'");
		while ($delrow=@mysql_fetch_object($delres)) {
			@mysql_query("delete from contacts_groups_members where membership_id = '$delrow->membership_id'");
			}
		$members_res=@mysql_query("select * from contacts_groups_members right join contacts on (contacts_groups_members.contacts_id = contacts.contacts_id) where group_id = '$row->group_id' and contacts.current = 'Y'");
		} else {
		$members_res=@mysql_query("select * from contacts_groups_members where group_id = '$row->group_id'");
		}
	$member_count=@mysql_num_rows($members_res);
	echo "
	<tr><td>
		<a href=$pagename?mode=group_edit&group_id=$row->group_id><font color=\"$textcolor\">$row->description</font></a>
	</td><td align=center>
		$row->login_required
	</td><td align=center>
		$row->email_required
	</td><td>
		<a href=$pagename?mode=group_members&group_id=$row->group_id><font color=\"$textcolor\"><i>Members ($member_count)</i></a>
	</td></tr>
	
	";
	}
echo "</table>";

?>
