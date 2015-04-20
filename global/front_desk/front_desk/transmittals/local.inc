<?
function verify_basic_sections() {
	global $global_jobinfo_id;
	$jobinfo_id=$global_jobinfo_id;
	$has_section=getoneb("select * from transmittals_sections where jobinfo_id = '$jobinfo_id' limit 1");
	if ($has_section) return 0;
	$query="insert into transmittals_sections set jobinfo_id = '$jobinfo_id', section = 'default', default_section = 'Y'";
	$res=@mysql_query($query);
	if (!($res)) die("Error adding default category!<hr>$query");
	$query2="update transmittals set section = 'default' where jobinfo_id = '$jobinfo_id'";
	$res2=@mysql_query($query2);
	if (!($res2)) die("Error moving job items to default category!!<hr>$query2");
	return 0;
	}

function select_a_section() {
	global $global_jobinfo_id,$current_transmittals_section,$new_transmittals_section;
	verify_basic_sections();
	
	if ($new_transmittals_section!="") $current_transmittals_section=addslashes($new_transmittals_section);
	$section_info=getoneb("select * from transmittals_sections where jobinfo_id = '$global_jobinfo_id' and section = '$current_transmittals_section' limit 1");
	if ($section_info) return 0;
	session_register('current_transmittals_section');
	$section_info=getoneb("select * from transmittals_sections where jobinfo_id = '$global_jobinfo_id' and default_section = 'Y' limit 1");
	$current_transmittals_section="$section_info->section";
	return ($section_info);
	}

function select_section_box() {
	global $global_jobinfo_id,$current_transmittals_section,$mode,$adminuser,$pagename;
	
	echo "<form style=\"display: inline;\" name=select_section method=post action=$pagename>Section&nbsp;<input type=hidden name=mode value=\"$mode\">";dropbox("select section as new_transmittals_section,section from transmittals_sections where jobinfo_id = '$global_jobinfo_id' and active = 'Y'","$current_transmittals_section",1,"submit()");
	if ($adminuser) echo "&nbsp;&nbsp;<a href=$pagename?mode=manage_sections><font color=blue><i>Edit List</i></font></a>";
	echo"</form>";
	}
?>
