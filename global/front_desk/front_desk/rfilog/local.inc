<?
$write=FALSE;
$guest=FALSE;

if ($application_user_type=="admin") $write=TRUE;
if ($application_user_type=="full") $write=TRUE;
if ($application_user_type=="guest") $guest=TRUE;

function verify_basic_sections() {
	global $global_jobinfo_id;
	$jobinfo_id=$global_jobinfo_id;
	$has_section=getoneb("select * from rfi_sections where jobinfo_id = '$jobinfo_id' limit 1");
	if ($has_section) return 0;
	$query="insert into rfi_sections set jobinfo_id = '$jobinfo_id', section = 'default', default_section = 'Y'";
	$res=@mysql_query($query);
	if (!($res)) die("Error adding default category!<hr>$query");
	$query2="update rfilog set section = 'default' where jobinfo_id = '$jobinfo_id'";
	$res2=@mysql_query($query2);
	if (!($res2)) die("Error moving job items to default category!!<hr>$query2");
	return 0;
	}

function select_a_section() {
	global $global_jobinfo_id,$current_rfilog_section,$new_rfilog_section;
	verify_basic_sections();
	
	if ($new_rfilog_section!="") $current_rfilog_section=addslashes($new_rfilog_section);
	$section_info=getoneb("select * from rfi_sections where jobinfo_id = '$global_jobinfo_id' and section = '$current_rfilog_section' limit 1");
	if ($section_info) return 0;
	session_register('current_rfilog_section');
	$section_info=getoneb("select * from rfi_sections where jobinfo_id = '$global_jobinfo_id' and default_section = 'Y' limit 1");
	$current_rfilog_section="$section_info->section";
	return ($section_info);
	}

function select_section_box() {
	global $global_jobinfo_id,$current_rfilog_section,$mode,$adminuser,$pagename;
	
	echo "<form style=\"display: inline;\" name=select_section method=post action=$pagename>Section&nbsp;<input type=hidden name=mode value=\"$mode\">";dropbox("select section as new_rfilog_section,section from rfi_sections where jobinfo_id = '$global_jobinfo_id' and active = 'Y'","$current_rfilog_section",1,"submit()");
	if ($adminuser) echo "&nbsp;&nbsp;<a href=$pagename?mode=manage_sections><font color=blue><i>Edit List</i></font></a>";
	echo"</form>";
	}
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// Below this line is where beta items for adding to querylib.inc
// are being tested. 
//
// These need to be moved to the main libarary soon, once they're stable.
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////





function send_email_now($from_id,$to_id,$doc_id,$subject,$message,$attachment_data="",$attachment_name="",$attachment_mime_type="application/octet-stream") {
	global $global_user_id;
	$bad_from=0;
	$bad_to=0;
	$intended_from=addslashes($intended_from);
	$intended_to=addslashes($intended_to);
	$attachment=0;
	$emergency_contact=2;
	$doc_id=addslashes($doc_id);
	$semi_rand = md5(time()) . $from_id . $to_id; 
	$mime_boundary = "==Multipart_Boundary_x{$semi_rand}x";  
	$subject=addslashes($subject);
	$message=addslashes($message);
	
	$attachment=0;
	if ($attachment_data!="") {
		$attachment=1;
		$attachment_data=base64_encode($attachment_data);
		if ($attachment_name=="") $attachment_name="file.bin";
		}
	$from_id=addslashes($from_id);
	$from_info=getoneb("select * from contacts where contacts_id = '$from_id'");
	if ($from_info->email=="") {
		$from_id=addslashes($global_user_id);
		$from_info=getoneb("select * from contacts where contacts_id = '$from_id'");
		if ($from_info->email =="") {
			$from_info->email="webmaster@umci.com";
			$from_info->display_name="UMC Web Server";
			$bad_from=1;
			}
		}
	$to_id=addslashes($to_id);
	$to_info=getoneb("select * from contacts where contacts_id = '$to_id'");
	if ($to_info->email=="") {
		$to_info=$from_info;
		$bad_to=1;
		}
	$full_message="\r\n--$mime_boundary\r\nContent-Type: text/html; us-ascii\r\nFrom: \"$from_info->display_name\" <$from_info->email>\r\nSubject: $subject\r\n\r\n$message\r\n\r\n--$mime_boundary\r\nContent-Type: $attachment_mime_type;\r\nContent-Transfer-Encoding: base64\r\nContent-Disposition: attachment; filename=$attachment_name\r\n\r\n" . $attachment_data . "\r\n--${mime_boundary}--\r\n";
	mail("\"$to_info->display_name\" <$to_info->email>",$subject , $full_message , "From: \"$from_info->display_name\" <$from_info->email>\r\nContent-type: multipart/mixed;\r\n  boundary=\"$mime_boundary\"");
	}
?>
