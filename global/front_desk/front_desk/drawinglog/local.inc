<?

$write=FALSE;
$guest=FALSE;

if ($application_user_type=="admin") $write=TRUE;
if ($application_user_type=="full") $write=TRUE;
if ($application_user_type=="guest") $guest=TRUE;

function set_drawing_not_current($drawing_id) {
	$drawing_id=addslashes($drawing_id);
	@mysql_query("update drawinglog set drawing_is_current = 'N' where drawing_id = '$drawing_id'");
	}

function set_drawing_current($drawing_id) {
	$drawing_id=addslashes($drawing_id);
	@mysql_query("update drawinglog set drawing_is_current = 'Y' where drawing_id = '$drawing_id'");
	}


function record_history ($drawing_id) {
	global $global_user_id;
	$global_user_id=addslashes($global_user_id);
	$drawing_id=addslashes($drawing_id);
	@mysql_query("insert into drawinglog_history set drawing_id = '$drawing_id',contacts_id = '$global_user_id'");
	}
function update_rev($drawing_id) {
	$dwg_rev=getoneb("select * from drawinglog_revs where drawing_id = '$drawing_id' order by revision desc,rev_date desc limit 1");
	$query="update drawinglog set 
	revision = '$dwg_rev->revision',
	current_rev_date = '$dwg_rev->rev_date',
	sent_to_field_date = '$dwg_rev->sent_to_field_date'
	where drawing_id = '$drawing_id'";
	@mysql_query($query);
	}

function verify_basic_sections() {
	global $global_jobinfo_id;
	$jobinfo_id=$global_jobinfo_id;
	$has_section=getoneb("select * from drawinglog_sections where jobinfo_id = '$jobinfo_id' limit 1");
	if ($has_section) return 0;
	//echo "<script>alert('no section');</script>";
	$query="insert into drawinglog_sections set jobinfo_id = '$jobinfo_id', section = 'default', default_section = 'Y'";
	$res=@mysql_query($query);
	if (!($res)) die("Error adding default category!<hr>$query");
	$query2="update drawinglog set section = 'default' where jobinfo_id = '$jobinfo_id'";
	$res2=@mysql_query($query2);
	if (!($res2)) die("Error moving job items to default category!!<hr>$query2");
	return 0;
	}

function select_a_section() {
	global $global_jobinfo_id,$current_drawinglog_section,$new_drawinglog_section;
	verify_basic_sections();
	
	if ($new_drawinglog_section!="") $current_drawinglog_section=addslashes($new_drawinglog_section);
	$section_info=getoneb("select * from drawinglog_sections where jobinfo_id = '$global_jobinfo_id' and section = '$current_drawinglog_section' limit 1");
	if ($section_info) return 0;
	session_register('current_drawinglog_section');
	$section_info=getoneb("select * from drawinglog_sections where jobinfo_id = '$global_jobinfo_id' and default_section = 'Y' limit 1");
	$current_drawinglog_section="$section_info->section";
	return ($section_info);
	}

function select_section_box() {
	global $global_jobinfo_id,$current_drawinglog_section,$mode,$adminuser,$pagename;
	
	echo "<form style=\"display: inline;\" name=select_section method=post action=$pagename>Section&nbsp;<input type=hidden name=mode value=\"$mode\">";dropbox("select section as new_drawinglog_section,section from drawinglog_sections where jobinfo_id = '$global_jobinfo_id' and active = 'Y'","$current_drawinglog_section",1,"submit()");
	if ($adminuser) echo "&nbsp;&nbsp;<a href=$pagename?mode=manage_sections><font color=blue><i>Edit List</i></font></a>";
	echo"</form>";
	}
?>
