<?

function contact_table($query,$edit=1,$title="") {
	global $fd_color_4;
	if ($query=="") $query="select * from contacts order by alphasort";
	
	$res=@mysql_query($query);
	if (!($res)) {
		echo "<br><b>ERROR: Contact table printing failed.</b><br>";
		return FALSE;
		}
	echo"<table border=1 cellspacing=0 cellpadding=2 width=600>";
	
if ($title!="") echo "<tr><td colspan=3 align=center bgcolor=$fd_color_4><b>$title</b></td></tr>";
echo "
	<tr bgcolor=#eeeeee><td>
		<b>Contact</b>
	</td><td>
		<b>Type</b>
	</td><td>
		<b>E-Mail</b>
	</td></tr>
	";
	
	while($row=@mysql_fetch_object($res)) {
		if ($edit==1) $urltext="$pagename?mode=contacts_edit&contacts_id=$row->contacts_id";
		else $urltext="$pagename?mode=contacts_view&contacts_id=$row->contacts_id";
		
		if ($row->current=='Y') $cellbg="#ffffff";
		else $cellbg="#dddddd";
		
		
		//$type="contact";
		//if ($row->is_company=='Y') $type="company";
		$type=$row->contact_type;
		if (($row->umc_emp=='Y')&&($row->contact_type=="contact")) $type="umc&nbsp;employee";
		if ($row->display_name=="") $row->display_name="none";
		$creator=getoneb("select * from contacts where contacts_id = '$row->record_creator'");
		echo "<tr bgcolor='$cellbg'><td>
				<a href=$urltext><font color=blue>$row->display_name</font></a>
			</td><td>
				<b title=\"Created by: $creator->display_name\">$type</b>
			</td><td>
				<font size=-1>$row->email&nbsp;</font>
			</td></tr>";
		}
	echo "</table>";	
	}







?>
