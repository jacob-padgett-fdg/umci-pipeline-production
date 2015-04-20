#!/usr/local/bin/php
<?

require("global_connect_rw.phtml");
require('querylib.inc');
$site="pipeline.umci.com";
$issue_id=addslashes($argv[1]);
$exclude_user_id=addslashes($argv[2]);
$contacts_view_url="http://$site/global/contacts/contactview.phtml?contacts_id=";

function mail_contact($contact,$issue_info) {
	global $site,$contacts_view_url;
	$index_info=getone("select * from mis_issue_index where issue_id = '$issue_info->issue_id'");
	$contact=addslashes($contact);
	$itisus=getoneb("select * from mis_issue_uslist where contacts_id = '$contact'");
	$rcd=invali_date($issue_info->requested_completion,'/');
	$ecd=invali_date($issue_info->expected_completion,'/');
	$nad=invali_date($issue_info->expected_completion_date,'/');
	$created_date=invali_date($issue_info->issue_create_date);
	
	
	$people_res=@mysql_query("select * from mis_issue_concerned_parties left join contacts using (contacts_id) where issue_history_id = '$issue_info->issue_history_id'");
	$us_people="";
	$them_people="";
	while ($people_row=@mysql_fetch_object($people_res)) {
		if ($people_row->us_or_them=="us") $us_people=$us_people . "<a target=contact_view_win href=$contacts_view_url$people_row->contacts_id><font color=blue>$people_row->display_name</font></a><br>";
		if ($people_row->us_or_them=="them") $them_people=$them_people . "<a target=contact_view_win href=$contacts_view_url$people_row->contacts_id><font color=blue>$people_row->display_name</font></a><br>";
		}
	if ($itisus) {
		$issue_url="http://pipeline.umci.com/global/mis_issue_tracking/?mode=issue_edit&issue_id=$issue_info->issue_id";
		} else {
		$issue_url="http://pipeline.umci.com/global/mis_issue_request/?mode=issue_edit&issue_id=$issue_info->issue_id";
		}
	
	if ($index_info->status=="complete") {
		$action_text="completed";
		//if (!($itisus)) $additional_message="This item will be considered closed unless you follow the link to edit the item and then reject it. We would also appreciate it if when you are completely satisfied that the issue has been resolved to your satisfaction that you log in, and approve the issues completion.<p>";
		$additional_message="This item will be considered closed unless you follow the link to edit the item and then reject it. We also ask that you log in and approve this item when you are satisfied with it's resolution.<p>";
		//else $additional_message="";
		} else {
		$action_text="updated";
		$additional_message="";
		}
	$creator_info=getoneb("select * from contacts where contacts_id = '$issue_info->creator'");
	$subject="IT issue $action_text";
	$message="<b>IT Issue number $issue_info->issue_id has been $action_text</b><p>$additional_message
	
	<a href=\"$issue_url\"><font color=blue>Edit Issue $issue_info->issue_id</font></a>
	
	
	<table border=1 cellspacing=0 cellpadding=4><tr><td bgcolor=#3355ff>
	<table border=1 cellspacing=0 cellpadding=4 bgcolor=white>
	<tr bgcolor=#99aaff><td colspan=2 align=center>
		<b>$issue_info->name (<a href=\"$issue_url\"><font color=blue>$issue_info->issue_id</font></a>)</b>
	</td></tr>
	
	<tr><td width=50%>
		<b>Description</b>
	</td><td width=50%>
		$issue_info->description
	</td></tr>

	<tr><td>
		<b>Issue Creator</b>
	</td><td>
		<a target=contact_view_win href=$contacts_view_url$creator_info->contacts_id><font color=blue>$creator_info->display_name</font></a>
	</td></tr>
	
	<tr><td>
		<b>Issue Created on
	</td><td>
		$created_date
	</td></tr>

	<tr><td>
		<b>Item Status</b>
	</td><td>
		$issue_info->status
	</td></tr>
	
	<tr><td>
		<b>Requested&nbsp;Completion&nbsp;Date</b>
	</td><td>
		$rcd
	</td></tr>
		
	<tr><td>
		<b>Expected Completion Date</b>
	</td><td>
		$ecd
	</td></tr>
		
	<tr><td valign=top>
		<b>Activity/Notes</b> <i><font size=-1>($nad)</font></i>
	</td><td>
		$issue_info->action_required
	</td></tr>

	<tr><td>
		<b>Additional Notes</b>
	</td><td>
		$issue_info->notes&nbsp;
	</td></tr>";
	
	if ($itisus) {
		$message=$message . "
		<tr><td>
			<b>IT Notes</b>
		</td><td>
			$issue_info->it_notes&nbsp;
		</td></tr>
		";
		}
	
	if ($issue_info->us_or_them=="us") $nar=$us_people;
	if ($issue_info->us_or_them=="them") $nar=$them_people;
	
	if ($index_info->status!="complete") {
	$message = $message . "
	<tr><td valign=top>
		<b>Next action responsibility</b>
	</td><td>
		$nar
	</td></tr>";
	}		
	$message = $message . "
	<tr bgcolor=#99aaff><td align=center>
		<b>Assigned IT People</b>
	</td><td align=center>
		<b>Users</b>
	</td></tr>	
	
	<tr><td valign=top>
		$us_people
	</td><td valign=top>
		$them_people
	</td></tr>
	
	</table>
	</td></tr></table>
	";
	
	//echo $message;
	queue_email($contact,$subject,$message,'Y');
	
	}

$query="select * from mis_issue_index,mis_issue_history 
	where 
	mis_issue_index.issue_id = '$issue_id' and 
	mis_issue_index.issue_id = mis_issue_history.issue_id and 
	current_hist_item = 'Y' and email_sent = 'N'";
$issue_info=getoneb($query);


if ($issue_info) {
	$query="select * from mis_issue_concerned_parties,contacts 
		where 
		issue_history_id = '$issue_info->issue_history_id' and 
		mis_issue_concerned_parties.contacts_id = contacts.contacts_id group by contacts.contacts_id";
	$res=@mysql_query($query);
	
	while ($row=@mysql_fetch_object($res)) {
		//echo "<hr>$row->display_name";
		if ($exclude_user_id!=$row->contacts_id) mail_contact($row->contacts_id,$issue_info);
		}
	@mysql_query("update mis_issue_history set email_sent = 'Y' where issue_history_id = '$issue_info->issue_history_id'");
	
	}
?>
