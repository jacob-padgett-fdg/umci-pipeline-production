<?

if ($storage_root=="") {
    // GPH MARK - absolute path
    include("/data/web/pipeline/global/webfile/storage_settings.cfg");
}

function detect_seperator_page($file) {
	if ($files_id=="") die("no file id.. nothing to do");
	else $files_id=addslashes($files_id);
	$file_info=getone("select * from webfile_files where files_id = '$files_id'");
	$older_version=getoneb("select * from webfile_files where file_group_id = 0 and replaced_by = '$file_info->files_id'");
	$download_link=webfile_file_download($file_info->files_id);
	$full_preview_link=webfile_image_show($file_info->files_id);
	if ($onchange!="") $onchange_decode=base64_decode($onchange);
	
	
	echo "<p align=right>$done_link</p>";
	
	$aurl="$pagename?mode=webfile_permissions_toggle&files_id=$file_info->files_id&return_to=$return_to_this_page";
	
	$imgurl=webfile_medium_image_icon($file_info->files_id);
	$viewurl=webfile_view_link($file_info->files_id);
	
	echo "
	<script>
	function apply_changes() {
		$onchange_decode;
		document.location=\"$return_to_decode\";
		}
	</script>
	
	";
	
	if (($file_info->owner==$global_user_id)||($adminuser)) {
		echo "
		<script>
		function webdelfile(files_id) {
			if (confirm(\"Permenantly delete file from database?\")) {
				open('$pagename?mode=delete_file_popup&files_id=' + files_id,'webfile_del_pu','width=20,height=20');
				}
			}
		</script>
		";
		}

	//$bgcolor="#8caede";
	$bgcolor=$umc_standard_blue;
	$bluebgcolor=$bgcolor;
	$p_version_rows="";
	if ($file_info->file_group_id==0) {
		$newer_version=getoneb("select * from webfile_files where files_id = '$file_info->replaced_by'");
		$bgcolor="#888888";
	
		echo "<table border=1 cellpadding=2 cellspacing=0 width=100%>
		<tr bgcolor=\"$bluebgcolor\"><td colspan=4 align=center><b>Newer versions</b></td></tr>";
		while ($newer_version) {
			$image_url=webfile_tiny_image_icon($newer_version->files_id);
			$tools_link="$pagename?mode=webfile_file_tools&files_id=$newer_version->files_id&return_to=$return_to_this_page";
			$p_version_rows="
			<tr><td>
				<table border=1 style=\"border:outset\"cellspacing=0 cellpadding=0><tr><td><a href=\"$tools_link\"><img src=$image_url border=0></a></td></tr></table>
			</td><td>
				$newer_version->file_name&nbsp;
			</td><td>
				$newer_version->upload_date&nbsp;
			</td><td>
				$newer_version->file_size&nbsp;
			</td></tr>
			" . $p_version_rows;
			$newer_version=getoneb("select * from webfile_files where files_id = '$newer_version->replaced_by'");
			}
		echo "$p_version_rows</table>";
		}
	
	echo"	
	<table border=1 cellspacing=0 cellpadding=3 width=100%>
	<form enctype=\"multipart/form-data\" name=file_info method=post action=$pagename>
	<input type=hidden name=files_id value=\"$file_info->files_id\">
	<input type=hidden name=return_to value=\"$return_to_this_page\">
	<input type=hidden name=mode value=webfile_file_update>
	<input type=hidden name=onchange value=\"$onchange\">
	<input type=hidden name=\"MAX_FILE_SIZE\" value=\"20000000000\">
	
	<tr bgcolor='$bgcolor'><td rowspan=2>
		<img style=\"max-width: 150;\"src=$imgurl>
	</td><td colspan=2 align=center>
		<b><i>$file_info->file_name</i></b>
	</td></tr>
	
	
	<tr><td colspan=2 valign=top bgcolor=#eeeeee>
	<table border=0 cellspacing=0 cellpadding=1>
	<tr><td>
		<b>Size:</b>
	<td><td width=100%>
		$file_info->file_size
	<td><td>
			<a href=\"$viewurl\" target=\"webfile_view_$file_info->files_id\"><font color=blue>View&nbsp;File</font></a>
	</td></tr>
		<tr><td>
		<b>Type:</b>
	<td><td>
		<font size=-1>$file_info->mime_type</font>
	<td><td>
		<a href=$download_link><font color=blue>Save&nbsp;File</font></a><br>
	</td></tr>
		<tr><td>
		<b>Date:</b>
	<td><td>
		";$uldate=invali_date($file_info->upload_date);echo"
		$uldate
	<td><td>
		<a href=$full_preview_link><font color=blue>Large&nbsp;Preview</font></a>
	</td></tr>
		";
	if ($file_info->page_count > 0) {
		$pctext="<b>Pages:</b>";
		$pccount=$file_info->page_count;
		} else {
		$pctext="";
		$pccount="";
		}
	
	echo"
	<tr><td>
		$pctext
	<td><td>
		$pccount
	<td><td>
		";if ($adminuser && (!($older_version))) echo "<a href=javascript:webdelfile($file_info->files_id)><font color=blue>Delete</font></a>";echo"
	</td></tr>
	
	</table>
	</td></tr>
	
	";
	
	if ($writable) {
		echo "
		<tr><td>
			<b>Description</b>
		</td><td colspan=2>
			<input type=text size=30 name=description value=\"$file_info->description\" onchange=submit();>
		</td></tr>
		
		<tr><td>
			<b>New Version</b>
		</td><td colspan=2>
			<input type=file size=30 name=upload_file_1 value=\"Select\" onchange=submit();>
		</td></tr>
		";
		}
	
	
	if (($file_info->owner==$global_user_id)||($adminuser)) {
	
	echo "
		<tr><td>
			<b>Permissions</b>
		</td><td align=center>
			<b>Read</b>
		</td><td align=center>
			<b>Write</b>
		</td></tr>
			
		<tr><td>
			<b>Public</b>
		</td><td align=center>
			<a href=\"$aurl&permission=access_public\"><font color=blue>
			$file_info->access_public
			</font></a>
		</td><td align=center>
			<a href=\"$aurl&permission=write_public\"><font color=blue>
			$file_info->write_public
			</font></a>
		</td></tr>
		
		<tr><td>
			<b>UMC Network</b>
		</td><td align=center>
			<a href=\"$aurl&permission=access_intranet\"><font color=blue>
			$file_info->access_intranet
			</font></a>
		</td><td align=center>
			<a href=\"$aurl&permission=write_intranet\"><font color=blue>
			$file_info->write_intranet
			</font></a>
		</td></tr>
		
		<tr><td>
			<b>Users</b>
		</td><td align=center>
			<a href=\"$aurl&permission=access_any_user\"><font color=blue>
			$file_info->access_any_user
			</font></a>
		</td><td align=center>
			<a href=\"$aurl&permission=write_any_user\"><font color=blue>
			$file_info->write_any_user
			</font></a>
		</td></tr>
		
		<tr><td>
			<b>Employees</b>
		</td><td align=center>
			<a href=\"$aurl&permission=access_employees\"><font color=blue>
			$file_info->access_employees
			</font></a>
		</td><td align=center>
			<a href=\"$aurl&permission=write_employees\"><font color=blue>
			$file_info->write_employees
			</font></a>
		</td></tr>
		";
		}		
			
	//if ($readable) echo "READ";
	//if ($writable) echo "WRITE";
	
	echo "</table></form>";
		
	if ($older_version) {
		echo "<table border=1 cellpadding=2 cellspacing=0 width=100%>
		<tr bgcolor=#eeeeee><td colspan=4 align=center><b>Older versions</b></td></tr>";
		while ($older_version) {
			$image_url=webfile_tiny_image_icon($older_version->files_id);
			$tools_link="$pagename?mode=webfile_file_tools&files_id=$older_version->files_id&return_to=$return_to_this_page";
			echo "
			<tr><td>
				<table border=1 style=\"border:outset\"cellspacing=0 cellpadding=0><tr><td><a href=\"$tools_link\"><img src=$image_url border=0></a></td></tr></table>
			</td><td>
				$older_version->file_name&nbsp;
			</td><td>
				$older_version->upload_date&nbsp;
			</td><td>
				$older_version->file_size&nbsp;
			</td></tr>
			";
			$older_version=getoneb("select * from webfile_files where file_group_id = 0 and replaced_by = '$older_version->files_id'");
			}
		echo "</table>";
		}
	}


function prep_for_new_files($file_info="") {
	if (isset($file_info)&&($file_info=="")||($file_info==0)) {
		$file_info=new_file_info();
	}
	if (!(is_dir($file_info->file_directory_full_path))) {
		if (!(mkdir($file_info->file_directory_full_path,0777)))
			die("Application Error! Could not create destination directory! Please contact your system administrator!");
		@chown($file_info->file_directory_full_path,"www-data");
		@chgrp($file_info->file_directory_full_path,"www-data");
		}
	}


function copy_webfile_group($file_group_id,$application="",$description="",$target_file_group_id="") {
	// Copies all the files from a group to a new or existing group..
	if ($target_file_group_id!="") $new_group->file_group_id=addslashes($target_file_group_id);
	else $new_group=create_file_group($description,$application);
	
	//die($target_file_group_id);
	$file_group_id=addslashes($file_group_id);
	
	//$source_group_info=getoneb("select * from webfile_files where file_group_id = '$file_group_id'");
	//$new_group=create_file_group($description,$application);
	$file_list_res=@mysql_query("select * from webfile_files where file_group_id = '$file_group_id'");
	//tabledump("select * from webfile_files where file_group_id = '$file_group_id'");die();
	while($file_row=@mysql_fetch_object($file_list_res)) {
		copy_webfile_files($file_row->files_id,$new_group->file_group_id,$application);
		}
	return($new_group->file_group_id);
	}

function copy_webfile_files($files_id,$file_group_id,$application="") {
	// Copies a webfile file to another file group.. 
	$storage_root=$GLOBALS['storage_root'];
	
	$files_id=addslashes($files_id);
	$file_group_id=addslashes($file_group_id);
	$files_info=getoneb("select * from webfile_files where files_id = '$files_id'");
	//$temp_name=$temp_file = tempnam(sys_get_temp_dir(), 'webfile_copy_file_-_');
	$file=file_get_contents($storage_root . $files_info->file_path);
	$return=add_file_from_var_contents($file_group_id,$file,$files_info->file_name);
	apply_default_file_group_permissions($file_group_id,$application);
	//return(add_file_from_var_contents($file_group_id,$file,$files_info->file_name));
	
	return($return);
	}

function copy_webfile_file($source_files_id,$target_files_id,$application="") {
	// Copies a webfile_file to another webfile_file (replaces the existing
	$storage_root=$GLOBALS['storage_root'];
	$source_info=getoneb("select * from webfile_files where files_id = '$source_files_id'");
	$target_info=getoneb("select * from webfile_files where files_id = '$target_files_id'");
	if(!($source_info)) return FALSE;
	if(!($target_info)) return FALSE;
	//$files_id=addslashes($files_id);
	//$file_group_id=addslashes($file_group_id);
	//$files_info=getoneb("select * from webfile_files where files_id = '$files_id'");
	//$temp_name=$temp_file = tempnam(sys_get_temp_dir(), 'webfile_copy_file_-_');
	$file=file_get_contents($storage_root . $source_info->file_path);
	$return=add_file_from_var_contents($target_info->file_group_id,$file,$source_info->file_name);
	$copy_info=getoneb("select * from webfile_files where files_id = '$return'");
	@mysql_query("update webfile_files set file_group_id = '0', replaced_by = '$copy_info->files_id' where files_id = '$target_info->files_id'");
	apply_default_file_group_permissions($target_info->file_group_id,$application);
	return($return);
	}


function delete_webfile_file($files_id,$single_rev=0) {
	if ($files_id<1) return FALSE;
	if ($files_id=="") return FALSE;
	$storage_root=$GLOBALS['storage_root'];
	$file_info=getoneb("select * from webfile_files where files_id = '$files_id'");
	if (!($file_info)) return $file_info;
	
	$old_version=getoneb("select * from webfile_files where replaced_by = '$file_info->files_id' and file_group_id = 0");
	
	if (($old_version)&&($single_rev==0)) {
		// Normal recursive delete here.. 
		delete_webfile_file($old_version->files_id);
		}
	$delete_path=$storage_root . $file_info->file_path;
	$delete_path=escapeshellcmd($delete_path);
	$delete_file=$storage_root . $file_info->info_path;
	$delete_file=escapeshellcmd($delete_file);
	system("rm -f \"$delete_file\"");
	system("rm -rf \"$delete_path\"*");
	@mysql_query("delete from webfile_files where files_id = '$file_info->files_id'");
	@mysql_query("update webfile_files set 
	replaced_by = '$file_info->replaced_by',
	file_group_id = '$file_info->file_group_id'
	where 
	replaced_by = '$files_id' and 
	file_group_id = 0");
	return TRUE;
	}
	
function delete_webfile_group($file_group_id) {
	if ($file_group_id<1) return FALSE;
	if ($file_group_id=="") return FALSE;
	$file_group_info=load_file_group_info($file_group_id);
	if (!($file_group_info)) return FALSE;
	$list=@mysql_query("select * from webfile_files where file_group_id = '$file_group_info->file_group_id'");
	while ($row=@mysql_fetch_object($list)) {
		delete_webfile_file($row->files_id);
		}
	@mysql_query("delete from webfile_groups where file_group_id = '$file_group_info->file_group_id'");
	}



function new_file_info() {
	global $storage_root,$current_storage_path;
	$file_info = (object) array();
	$file_info->month=date("Ym");
	$file_info->file_directory=$current_storage_path . $file_info->month;
	$file_info->file_directory_full_path=$storage_root . $file_info->file_directory;
	$max_count_info=getoneb("select max(sequence_num) as sequence_num from webfile_files where file_directory = '$file_info->file_directory' and record_type = 'file' group by file_directory limit 1");
	if ($max_count_info) {
		$file_info->sequence_num=$max_count_info->sequence_num + 1;
		} else { 
		$file_info->sequence_num='1';
		}
	$file_info->file_path_orig=$file_info->file_directory_full_path . "/$file_info->sequence_num" . ".bin";
	$file_info->file_path=$file_info->file_directory . "/$file_info->sequence_num" . ".bin";
	$file_info->info_path=$file_info->file_directory . "/$file_info->sequence_num" . ".nfo";
	return $file_info;
	}

function create_file_group($description="",$application="") {
	global $global_user_id;
	$application=addslashes($application);
	if ($description!="") $description=addslashes($description);
	$query="insert into webfile_groups set description = '$description', creator = '$global_user_id', application_name = '$application' ,create_date=now()";
	$res=@mysql_query($query);
	if (!($res)) die("Error creating file group!");
	$file_group_id=@mysql_insert_id();
	$file_group_info=load_file_group_info($file_group_id);
	
	return ($file_group_info);
	}

function post_file_creation_checkup($files_id) {
	$storage_root=$GLOBALS['storage_root'];
	$file_info=getone("select * from webfile_files where files_id = '$files_id'");
	if($file_info->mime_type=='application/pdf') {
		$page_count=`pdftk $storage_root$file_info->file_path dump_data | grep Number | cut -f2 -d" "`;
		@mysql_query("update webfile_files set page_count = '$page_count' where files_id = '$files_id'");
		}
	return($files_id);
	}

function add_file_from_filesystem($file_group_id,$filename,$mime_type="") {
	$file=file_get_contents($filename);
	return(add_file_from_var_contents($file_group_id,$file,$filename));
	}

function add_file_from_var_contents($file_group_id,$file_contents,$file_name='unknown',$mime_type="") {
	$global_user_id=$GLOBALS['global_user_id'];
	$sr=$GLOBALS['storage_root'];
	$file_name=addslashes($file_name);
	$file_size=strlen($file_contents);
	$file_info=new_file_info();
	prep_for_new_files($file_info);
	$known_mime_types = array(
		"image/jpg",
		"image/png",
		"image/tiff",
		"image/gif",
		"image/bmp",
		"application/pdf",
	);

	if(!in_array(strtolower($mime_type), $known_mime_types)){
		$mime_type = mime_type_from_filename(trim($file_name));
	}
	//$file_info='foo';
	$query="insert into webfile_files set
	file_group_id = '$file_group_id',
	mime_type = '$mime_type',
	owner = '$global_user_id',
	upload_date = now(),
	file_path_orig = '$file_info->file_path_orig',
	file_path = '$file_info->file_path',
	info_path = '$file_info->info_path',
	file_directory = '$file_info->file_directory',
	file_name = '$file_name',
	file_size = '$file_size',
	sequence_num = '$file_info->sequence_num'";
	@mysql_query($query);
	$fp=fopen($file_info->file_path_orig,wb);
	fwrite($fp,$file_contents);
	fclose($fp);
	chmod($sr . $file_info->file_path,0666);
	$file_info->files_id=@mysql_insert_id();
	post_file_creation_checkup($file_info->files_id);
	return($file_info->files_id);
	}

function mime_type_from_filename ($filename) {
	$filename=strtolower($filename);
		$extension=ereg_replace("^.*\.","",$filename);
		$extension=ereg_replace("^.*\.","",$extension);
		$extension=ereg_replace("^.*\.","",$extension);
		$extension=ereg_replace("^.*\.","",$extension);
		$extension=ereg_replace("^.*\.","",$extension);
	switch ($extension) {
		case "jpg":
		case "jpeg":	return "image/jpg";
						break;;
		case "png":		return "image/png";
						break;;
		case "tif":
		case "tiff":	return "image/tiff";
						break;;
		case "gif":		return "image/gif";
						break;;
		case "bmp":		return "image/bmp";
						break;;
		case "pdf":		return "application/pdf";
						break;;
		default:		return "application/octet-stream";
						break;;
		}
	die("How did the code get here? Please contact your system administrator!");
	}
?>
