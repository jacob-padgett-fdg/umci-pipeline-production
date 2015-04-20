<?
$write=FALSE;
$guest=FALSE;

if ($application_user_type=="admin") $write=TRUE;
if ($application_user_type=="full") $write=TRUE;
if ($application_user_type=="guest") $guest=TRUE;

function load_javascript_oiclog_doc_box() {
	global $javascript_oiclog_doc_box_loaded,$devel;
	if ($javascript_oiclog_doc_box_loaded=='Y') return 0;
	$javascript_oiclog_doc_box_loaded='Y';
	echo "
	<script>
	function fd_doc_box_refresh(id,options_encoded) {
		ajaxManager('load_page','/global/front_desk$devel/?mode=short_doc_list_with_attachments&options_encoded=' + options_encoded,id);
		}
	</script>
	";	
	return 0;
	}

function oiclog_doclist_doc_row ($options,$row) {
	global $fd_color_5,$global_user_id,$fd_color_4;
	$error=0;
	$removal_error=0;
	$link=getoneb("select * from documents_links where doca = '" . $options['doc_id'] . "' and docb = '$row->doc_id' limit 1");
	if ($options['toggle']=='Y') {
		if ($link) {
			if ($link->file_group_id>0) {
				if (getoneb("select * from webfile_files where file_group_id = '$link->file_group_id' limit 1")) $removal_error=1;
				else $update_query="call doc_link_delete(" . $options['doc_id'] . ",$row->doc_id)";
				} else {
				$update_query="call doc_link_delete(" . $options['doc_id'] . ",$row->doc_id)";
				}
			} else {
			$update_query="call doc_link(" . $options['doc_id'] . ",$row->doc_id,$global_user_id,'')";
			}
	 	if ($removal_error!='1') @mysql_query($update_query);
		$link=getoneb("select * from documents_links where doca = '" . $options['doc_id'] . "' and docb = '$row->doc_id' limit 1");
		}

	$bstart="";
	$bend="";
	$row_bg="#ffffff";
	if ($link) {
		$row_bg="$fd_color_4";
		$bstart="<b>";
		$bend="</b>";
		} 
	if ($removal_error==1) $row_bg=$fd_color_5;
		echo "
		<td style=\"width: 120px; border-top: 1px solid black;\" id=\"1_$row->doc_id\" bgcolor=\"$row_bg\"\>
			<font size=\"-1\"><b>$row->app_reference</b>&nbsp;&nbsp;</font>
		</td><td style=\"border-top: 1px solid black;\" id=\"2_$row->doc_id\" bgcolor=\"$row_bg\">
			<font size=\"-1\">$row->description</font>
		</td>";
	}

function oiclog_doclist_doc_row_afc ($options,$row) {
	global $fd_color_5,$global_user_id,$fd_color_4;
	$error=0;
	$removal_error=0;
	$link=getoneb("select * from documents_links where doca = '" . $options['doc_id'] . "' and docb = '$row->doc_id' limit 1");
	if ($options['toggle']=='Y') {
		if ($link) {
			if ($link->file_group_id>0) {
				if (getoneb("select * from webfile_files where file_group_id = '$link->file_group_id' limit 1")) $removal_error=1;
				else $update_query="call doc_link_delete(" . $options['doc_id'] . ",$row->doc_id)";
				} else {
				$update_query="call doc_link_delete(" . $options['doc_id'] . ",$row->doc_id)";
				}
			} else {
			$update_query="call doc_link(" . $options['doc_id'] . ",$row->doc_id,$global_user_id,'')";
			}
	 	if ($removal_error!='1') @mysql_query($update_query);
		$link=getoneb("select * from documents_links where doca = '" . $options['doc_id'] . "' and docb = '$row->doc_id' limit 1");
		}

	$bstart="";
	$bend="";
	$row_bg="#ffffff";
	if ($link) {
		$row_bg="$fd_color_4";
		$bstart="<b>";
		$bend="</b>";
		} 
	if ($removal_error==1) $row_bg=$fd_color_5;
		echo "
		<td style=\"width: 120px; border-top: 1px solid black;\" id=\"1_$row->doc_id\" bgcolor=\"$row_bg\"\>
			<font size=\"-1\"><b>$row->app_reference</b>&nbsp;&nbsp;</font>
		</td><td style=\"border-top: 1px solid black;\" id=\"2_$row->doc_id\" bgcolor=\"$row_bg\">
			<font size=\"-1\">$row->description</font>
		</td>";
	}


function get_oic_info($doc_id) {
	$doc_info=getoneb("select * from documents where doc_id = '$doc_id'");
	if (!($doc_info)) die("Application error in get oic info. Document ID not detected!");
	return(getoneb("select * from oiclog where oiclog_id = '$doc_info->app_record_id'"));
	}

function oiclog_doc_box ($options="") {
	global $application_name,$devel;
	ajax_load();
	$list_id=rand();
	$options['list_id']=$list_id;
	$options=fd_document_associator_set_default_options($options);
	$options_encoded=base64_encode(serialize($options));
	load_javascript_generic_watch_form_element();
	load_javascript_oiclog_doc_box();
	
	$oic_info=get_oic_info($options['doc_id']);
	
	//echo "<b>$oic_info->status</b><br>"; 
	
	
	if ($oic_info->status=="Approved for construction by PM") {
		$approved=TRUE; 
		} else {
		$approved=FALSE;
		}
	
	echo "<div style=\"max-width: " . $options['div_max_width'] . "; overflow: auto; border:" . $options['div_border'] . "\" id=\"" . $options['list_id'] . "\">";
	include("oiclog_doc_list.phtml");
	echo "
	</div>
	<script>
	";
	if ($options['onchange']!='') {
		echo "document.getElementById('$list_id').onchange=\"" . $options['onchange'] . "\"
		document.getElementById('$list_id').value=1;
		generic_watch_form_element($list_id);
		";
		}
	echo"
	document.getElementById('$list_id').ajax_trigger='never';
	</script>";
	return 0;
	}


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
	global $global_jobinfo_id,$mode,$adminuser,$pagename,$application_name;
	$global_section_name='current_' . $application_name . '_section';
	echo "<form style=\"display: inline;\" name=select_section method=post action=$pagename>Section&nbsp;<input type=hidden name=mode value=\"$mode\">";dropbox("select section as new_application_section,section from ${application_name}_sections where jobinfo_id = '$global_jobinfo_id' and active = 'Y'",$GLOBALS[$global_section_name],1,"submit()");
	if ($adminuser) echo "&nbsp;&nbsp;<a href=$pagename?mode=manage_sections><font color=blue><i>Edit List</i></font></a>";
	echo"</form>";
	}

function comb_webfile_permissions() {
	global $current_oiclog_section, $global_jobinfo_id;
	$section=$current_oiclog_section;
	$list=@mysql_query("select * from oiclog where section = '$section' and jobinfo_id = '$global_jobinfo_id' and document_file_group_id > 0");
	while ($row=@mysql_fetch_object($list)) {
		apply_default_file_group_permissions($row->document_file_group_id,'oiclog-alternate-1');
		}
	}

?>
