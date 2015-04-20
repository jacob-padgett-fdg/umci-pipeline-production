<?
$write=FALSE;
$guest=FALSE;

if ($application_user_type=="admin") $write=TRUE;
if ($application_user_type=="full") $write=TRUE;
if ($application_user_type=="guest") $guest=TRUE;




function verify_basic_sections() {
	global $global_jobinfo_id,$application_name;
	$jobinfo_id=$global_jobinfo_id;
	$has_section=getoneb("select * from ${application_name}_sections where jobinfo_id = '$jobinfo_id' limit 1");
	if ($has_section) return 0;
	$query="insert into ${application_name}_sections set jobinfo_id = '$jobinfo_id', section = 'default', default_section = 'Y'";
	$res=@mysql_query($query);
	if (!($res)) die("Error adding default category!<hr>$query");
	$query2="update ${application_name} set section = 'default' where jobinfo_id = '$jobinfo_id'";
	$res2=@mysql_query($query2);
	if (!($res2)) die("Error moving job items to default category!!<hr>$query2");
	return 0;
	}

function select_a_section() {
	global $global_jobinfo_id,$new_application_section,$application_name;
	verify_basic_sections();
	$global_section_name='current_' . $application_name . '_section';
	if ($new_application_section!="") $GLOBALS[$global_section_name]=addslashes($new_application_section);
	$section_info=getoneb("select * from ${application_name}_sections where jobinfo_id = '$global_jobinfo_id' and section = '" . $GLOBALS[$global_section_name] . "' limit 1");
	if ($section_info) return 0;
	session_register($global_section_name);
	$section_info=getoneb("select * from ${application_name}_sections where jobinfo_id = '$global_jobinfo_id' and default_section = 'Y' limit 1");
	$GLOBALS[$global_section_name]="$section_info->section";
	return ($section_info);
	}

function select_section_box() {
	global $global_jobinfo_id,$mode,$adminuser,$pagename,$application_name,$global_jobinfo,$global_user;
	$global_section_name='current_' . $application_name . '_section';
	echo "<form style=\"display: inline;\" name=select_section method=post action=$pagename>Section&nbsp;<input type=hidden name=mode value=\"$mode\">";dropbox("select section as new_application_section,section from ${application_name}_sections where jobinfo_id = '$global_jobinfo_id' and active = 'Y'",$GLOBALS[$global_section_name],1,"submit()");
	if (($global_user->contacts_id==$global_jobinfo->project_manager_id)||(member_of_global_group_named('IT'))) echo "&nbsp;&nbsp;<a href=$pagename?mode=manage_sections><font color=blue><i>Edit List</i></font></a>";
	echo"</form>";
	}
?>
