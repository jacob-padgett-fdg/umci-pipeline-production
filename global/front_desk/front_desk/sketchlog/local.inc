<?

$write=FALSE;
$guest=FALSE;

if ($application_user_type=="admin") $write=TRUE;
if ($application_user_type=="full") $write=TRUE;
if ($application_user_type=="guest") $guest=TRUE;




function record_history ($sketchlog_id) {
	global $global_user_id;
	$global_user_id=addslashes($global_user_id);
	$sketchlog_id=addslashes($sketchlog_id);
	@mysql_query("insert into sketchlog_history set sketchlog_id = '$sketchlog_id',contacts_id = '$global_user_id'");
	}
function update_rev($sketchlog_id) {
	$sketch_rev=getoneb("select * from sketchlog_revs where sketchlog_id = '$sketchlog_id' order by revision desc limit 1");
	$query="update sketchlog set 
	revision = '$sketch_rev->revision',
	current_rev_date = '$sketch_rev->rev_date',
	sent_to_field_date = '$sketch_rev->sent_to_field_date'
	where sketchlog_id = '$sketchlog_id'";
	@mysql_query($query);
	}
function verify_basic_sections() {
	global $global_jobinfo_id;
	$jobinfo_id=$global_jobinfo_id;
	$has_section=getoneb("select * from sketchlog_sections where jobinfo_id = '$jobinfo_id' limit 1");
	if ($has_section) return 0;
	$query="insert into sketchlog_sections set jobinfo_id = '$jobinfo_id', section = 'default', default_section = 'Y'";
	$res=@mysql_query($query);
	if (!($res)) die("Error adding default category!<hr>$query");
	$query2="update sketchlog set section = 'default' where jobinfo_id = '$jobinfo_id'";
	$res2=@mysql_query($query2);
	if (!($res2)) die("Error moving job items to default category!!<hr>$query2");
	return 0;
	}

function select_a_section() {
	global $global_jobinfo_id,$current_sketchlog_section,$new_sketchlog_section;
	verify_basic_sections();
	
	if ($new_sketchlog_section!="") $current_sketchlog_section=addslashes($new_sketchlog_section);
	$section_info=getoneb("select * from sketchlog_sections where jobinfo_id = '$global_jobinfo_id' and section = '$current_sketchlog_section' limit 1");
	if ($section_info) return 0;
	session_register('current_sketchlog_section');
	$section_info=getoneb("select * from sketchlog_sections where jobinfo_id = '$global_jobinfo_id' and default_section = 'Y' limit 1");
	$current_sketchlog_section="$section_info->section";
	return ($section_info);
	}

function select_section_box() {
	global $global_jobinfo_id,$current_sketchlog_section,$mode,$adminuser,$pagename;
	
	echo "<form style=\"display: inline;\" name=select_section method=post action=$pagename>Section&nbsp;<input type=hidden name=mode value=\"$mode\">";dropbox("select section as new_sketchlog_section,section from sketchlog_sections where jobinfo_id = '$global_jobinfo_id' and active = 'Y'","$current_sketchlog_section",1,"submit()");
	if ($adminuser) echo "&nbsp;&nbsp;<a href=$pagename?mode=manage_sections><font color=blue><i>Edit List</i></font></a>";
	echo"</form>";
	}
?>
