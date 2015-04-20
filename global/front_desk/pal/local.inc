<?


$write=FALSE;
$guest=FALSE;

//die($global_user->umc_emp);
//die(print_r($global_user_info->umc_emp));
if ($global_user->umc_emp!='Y') $guest=TRUE;
//if ($application_user_type=="admin") $write=TRUE;
//if ($application_user_type=="full") $write=TRUE;
//if ($application_user_type=="guest") $guest=TRUE;



function show_pal_list($query) {
	global $fd_color_4,$pal_status,$pal_assigned,$guest;
	pal_javascripts();
	$res=@mysql_query($query);
	echo "
	<table id='sub_application_main_table' border=1 cellspacing=0 cellpadding=4 border=0>
	<thead>
	<tr class=floatheader bgcolor=$fd_color_4><td>
		<b>PAL #</b>
	</td><td>
		<b>Subject</b>
	</td><td>
		<b>Document</b>
	</td><td>
		<b>Document Description</b>
	</td><td>
		<b>Status</b>
	</td><td>
		<b>Priority</b>
	</td><td>
		<b>Assignment</b>
	</td><td>
		<b>Documents</b>
	</td><td>
		<b>Due</b>
	</td><td>
		<b>Completed</b>
	</td><td>
		<b>Created By</b>
	</td><td>
		<b>Created</b>
	</td></tr>
	</thead>
	<tbody "; if ($global_browser!='IE') echo "class=floatheader"; echo ">
	";

	while ($row=@mysql_fetch_object($res)) {
		if ($guest) {
			$applies_to_me=getoneb("select * from pal_buddies where pal_id = '$row->pal_id' and ( creator_id = '$global_user_id' or owner_id = '$global_user_id') limit 1");
			if (!($applies_to_me)) continue;
			}
		
		$bgcolor="#ffffff";
		if ($row->status=='complete') $bgcolor="#dddddd";
		$creator=getoneb("select * from contacts where contacts_id = '$row->creator_id'");
		$row->due_date=invali_date($row->due_date);
		$row->pal_sign_off_date=invali_date($row->pal_sign_off_date);
		
		$assignments="";
		$ass_res=@mysql_query("select * from pal_assignments,contacts where pal_id = '$row->pal_id' and contacts.contacts_id = pal_assignments.owner_id");
		while ($ass_row=@mysql_fetch_object($ass_res)) {
			$append="";
			if ($ass_row->signed_off=='Y') $append="&nbsp;(done)";
			$assignments=$assignments . "$ass_row->display_name$append<br>";
			}
		$raw_pal_info=getoneb("select * from pal where pal_id = '$row->pal_id'");
		$row_doc_info=getoneb("select * from documents where doc_type = 'pal' and app_record_id = '$row->pal_id'");
		$documents=fd_text_doc_list($row_doc_info->doc_id);
		$create_date=invali_date($raw_pal_info->create_date);
		
		echo "
			<tr bgcolor=\"$bgcolor\"><td>
				<a href=\"$pagename?mode=pal_edit&pal_id=$row->pal_id&pal_status=$pal_status&pal_assigned=$pal_assigned\"><font color=blue>$row->pal_num</font></a>
			</td><td>
				$row->pal_short_description
			</td><td>
				$row->doc_type - $row->app_reference
			</td><td>
				$row->description
			</td><td>
				$row->status
			</td><td>
				$row->priority
			</td><td>
				$assignments
			</td><td>
				$documents
			</td><td>
				$row->due_date
			</td><td>
				$row->pal_sign_off_date
			</td><td>
				$creator->display_name
			</td><td>
				$create_date
			</td></tr>";
		}
	echo "</td></tr>
	<tr height=100%><td colspan=12>&nbsp;</td></tr>
	</table>";
	}









function verify_basic_sections() {
	global $global_jobinfo_id;
	$jobinfo_id=$global_jobinfo_id;
	$has_section=getoneb("select * from pal_sections where jobinfo_id = '$jobinfo_id' limit 1");
	if ($has_section) return 0;
	$query="insert into pal_sections set jobinfo_id = '$jobinfo_id', section = 'default', default_section = 'Y'";
	$res=@mysql_query($query);
	if (!($res)) die("Error adding default category!<hr>$query");
	$query2="update pal set section = 'default' where jobinfo_id = '$jobinfo_id'";
	$res2=@mysql_query($query2);
	if (!($res2)) die("Error moving job items to default category!!<hr>$query2");
	return 0;
	}

function select_a_section() {
	global $global_jobinfo_id,$current_pal_section,$new_pal_section;
	verify_basic_sections();
	
	if ($new_pal_section!="") $current_pal_section=addslashes($new_pal_section);
	$section_info=getoneb("select * from pal_sections where jobinfo_id = '$global_jobinfo_id' and section = '$current_pal_section' limit 1");
	if ($section_info) return 0;
	session_register('current_pal_section');
	$section_info=getoneb("select * from pal_sections where jobinfo_id = '$global_jobinfo_id' and default_section = 'Y' limit 1");
	$current_pal_section="$section_info->section";
	return ($section_info);
	}

function select_section_box() {
	global $global_jobinfo_id,$current_pal_section,$mode,$adminuser,$pagename;
	
	echo "<form style=\"display: inline;\" name=select_section method=post action=$pagename>Section&nbsp;<input type=hidden name=mode value=\"$mode\">";dropbox("select section as new_pal_section,section from pal_sections where jobinfo_id = '$global_jobinfo_id' and active = 'Y'","$current_pal_section",1,"submit()");
	if ($adminuser) echo "&nbsp;&nbsp;<a href=$pagename?mode=manage_sections><font color=blue><i>Edit List</i></font></a>";
	echo"</form>";
	}
?>
