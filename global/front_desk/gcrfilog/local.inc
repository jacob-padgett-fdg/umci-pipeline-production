<?

$write=FALSE;
$guest=FALSE;

if ($application_user_type=="admin") $write=TRUE;
if ($application_user_type=="full") $write=TRUE;
if ($application_user_type=="guest") $guest=TRUE;

/*
function sync_to_rfilog($gcrfi_id,$stage=0) {
	$gcrfi_info=getoneb("select * from gcrfilog where gcrfi_id = '$gcrfi_id'");
	$rfi_info=get_my_rfi_info($gcrfi_id);
	//echo "$gcrfi_info->gcrfi_num / $rfi_info->rfi_num";
	if (!($gcrfi_info)) return sync_error();
	if (!($rfi_info)) return sync_error;
	$res=sync_share_only($rfi_info,$gcrfi_info);
	if (!($res)) sync_error();
	}

function sync_error() { 
	echo "<script>alert('There was an error synchronizing data between related documents. Please check this out manually.');</script>";
	//die("crap!");
	return FALSE;
	}

*/








/*
function sync_share_only($rfi_info,$gcrfi_info) {
	global $rfi_sync_field_error;
	if (!($rfi_info)) return sync_error();
	if (!($gcrfi_info)) return sync_error();

	//////////////////////////////////////////////////////////////
	// Sync 'reply' (response)
	//////////////////////////////////////////////////////////////
	$ud=FALSE;
	if ($rfi_info->reply!=$gcrfi_info->reply) {
		if ($rfi_info->reply=="") {
			$gcrfi_info->reply=addslashes($gcrfi_info->reply);
			@mysql_query("update rfilog set reply = '$gcrfi_info->reply' 
						where rfi_id = '$rfi_info->rfi_id'");
			$ud=TRUE;
			}
		if ($gcrfi_info->reply=="") {
			$rfi_info->reply=addslashes($rfi_info->reply);
			@mysql_query("update gcrfilog set reply = '$rfi_info->reply' 
						where gcrfi_id = '$gcrfi_info->gcrfi_id'");
			$ud=TRUE;
			}
		if ($ud!=TRUE) { $rfi_sync_field_error='response'; return sync_error(); }
		}
	//////////////////////////////////////////////////////////////
	// Sync 'reply_from' (response from)
	//////////////////////////////////////////////////////////////
	$ud=FALSE;
	if ($rfi_info->reply_from!=$gcrfi_info->reply_from) {
		if ($rfi_info->reply_from==0) {
			$gcrfi_info->reply_from=addslashes($gcrfi_info->reply_from);
			@mysql_query("update rfilog set reply_from = '$gcrfi_info->reply_from' 
						where rfi_id = '$rfi_info->rfi_id'");
			//echo "hellooo.....";exit;
			$ud=TRUE;
			}
		if ($gcrfi_info->reply_from==0) {
			$rfi_info->reply_from=addslashes($rfi_info->reply_from);
			@mysql_query("update gcrfilog set reply_from = '$rfi_info->reply_from' 
						where gcrfi_id = '$gcrfi_info->gcrfi_id'");
			$ud=TRUE;
			}
		if ($ud!=TRUE) { $rfi_sync_field_error='response_from'; return sync_error(); }
		}
	//////////////////////////////////////////////////////////////
	// Sync 'reply_date' (response date)
	//////////////////////////////////////////////////////////////
	$ud=FALSE;
	if ($rfi_info->reply_date!=$gcrfi_info->reply_date) {
		if ($rfi_info->reply_date=='0000-00-00') {
			$gcrfi_info->reply_date=addslashes($gcrfi_info->reply_date);
			@mysql_query("update rfilog set reply_date = '$gcrfi_info->reply_date' 
						where rfi_id = '$rfi_info->rfi_id'");
			//echo "hellooo.....";exit;
			$ud=TRUE;
			}
		if ($gcrfi_info->reply_date=='0000-00-00') {
			$rfi_info->reply_date=addslashes($rfi_info->reply_date);
			@mysql_query("update gcrfilog set reply_date = '$rfi_info->reply_date' 
						where gcrfi_id = '$gcrfi_info->gcrfi_id'");
			$ud=TRUE;
			}
		if ($ud!=TRUE) { $rfi_sync_field_error='response_date'; return sync_error(); }
		}
	//////////////////////////////////////////////////////////////
	// Sync 'question'
	//////////////////////////////////////////////////////////////
	$ud=FALSE;
	if ($rfi_info->question!=$gcrfi_info->question) {
		if ($rfi_info->question=="") {
			$gcrfi_info->question=addslashes($gcrfi_info->question);
			@mysql_query("update rfilog set question = '$gcrfi_info->question' 
						where rfi_id = '$rfi_info->rfi_id'");
			$ud=TRUE;
			}
		if ($gcrfi_info->question=="") {
			$rfi_info->question=addslashes($rfi_info->question);
			@mysql_query("update gcrfilog set question = '$rfi_info->question' 
						where gcrfi_id = '$gcrfi_info->gcrfi_id'");
			$ud=TRUE;
			}
		if ($ud!=TRUE) { $rfi_sync_field_error='question'; return sync_error(); }
		}
	//////////////////////////////////////////////////////////////
	// Sync 'solution' (recommended solution)
	//////////////////////////////////////////////////////////////
	$ud=FALSE;
	if ($rfi_info->solution!=$gcrfi_info->solution) {
		if ($rfi_info->solution=="") {
			$gcrfi_info->solution=addslashes($gcrfi_info->solution);
			@mysql_query("update rfilog set solution = '$gcrfi_info->solution' 
						where rfi_id = '$rfi_info->rfi_id'");
			$ud=TRUE;
			}
		if ($gcrfi_info->solution=="") {
			$rfi_info->solution=addslashes($rfi_info->solution);
			@mysql_query("update gcrfilog set solution = '$rfi_info->solution' 
						where gcrfi_id = '$gcrfi_info->gcrfi_id'");
			$ud=TRUE;
			}
		if ($ud!=TRUE) { $rfi_sync_field_error='solution'; return sync_error(); }
		}
	
	
	
	return TRUE;
	}



function get_my_rfi_info($gcrfi_id) {
	// This returns *RFI* info when given a gcrfi_id
	// This info will be used to compare data between the two records
	$my_doc_info=getoneb("select * from documents where doc_type = 'gcrfi' and app_record_id = '$gcrfi_id'");
	if (!($my_doc_info)) return sync_error();
	$rfi_link_info=getoneb("select * from documents_links right join documents on (docb = doc_id) where doca = '$my_doc_info->doc_id' and documents.doc_type = 'rfi'");
	//tabledump("select * from documents_links right join documents on (docb = doc_id) where doca = '$my_doc_info->doc_id' and documents.doc_type = 'rfi'");
	$gcrfi_link_info=getoneb("select * from documents_links right join documents on (docb = doc_id) where doca = '$rfi_link_info->doc_id' and documents.doc_type = 'gcrfi'");
	if (!($gcrfi_link_info)) return sync_error();
	$rfi_info=getoneb("select * from rfilog where rfi_id = '$rfi_link_info->app_record_id'");
	
	
	return ($rfi_info);
	}

*/
function verify_basic_sections() {
	global $global_jobinfo_id;
	$jobinfo_id=$global_jobinfo_id;
	$has_section=getoneb("select * from gcrfi_sections where jobinfo_id = '$jobinfo_id' limit 1");
	if ($has_section) return 0;
	$query="insert into gcrfi_sections set jobinfo_id = '$jobinfo_id', section = 'default', default_section = 'Y'";
	$res=@mysql_query($query);
	if (!($res)) die("Error adding default category!<hr>$query");
	$query2="update gcrfilog set section = 'default' where jobinfo_id = '$jobinfo_id'";
	$res2=@mysql_query($query2);
	if (!($res2)) die("Error moving job items to default category!!<hr>$query2");
	return 0;
	}

function select_a_section() {
	global $global_jobinfo_id,$current_gcrfilog_section,$new_gcrfilog_section;
	verify_basic_sections();
	
	if ($new_gcrfilog_section!="") $current_gcrfilog_section=addslashes($new_gcrfilog_section);
	$section_info=getoneb("select * from gcrfi_sections where jobinfo_id = '$global_jobinfo_id' and section = '$current_gcrfilog_section' limit 1");
	if ($section_info) return 0;
	session_register('current_gcrfilog_section');
	$section_info=getoneb("select * from gcrfi_sections where jobinfo_id = '$global_jobinfo_id' and default_section = 'Y' limit 1");
	$current_gcrfilog_section="$section_info->section";
	return ($section_info);
	}

function select_section_box() {
	global $global_jobinfo_id,$current_gcrfilog_section,$mode,$adminuser,$pagename;
	
	echo "<form style=\"display: inline;\" name=select_section method=post action=$pagename>Section&nbsp;<input type=hidden name=mode value=\"$mode\">";dropbox("select section as new_gcrfilog_section,section from gcrfi_sections where jobinfo_id = '$global_jobinfo_id' and active = 'Y'","$current_gcrfilog_section",1,"submit()");
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
