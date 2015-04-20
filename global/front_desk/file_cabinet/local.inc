<?
function keyword_hits_array($kwl) {
	global $direct_hits_array;
	$direct_hits_array=array();
	$where_clause="";
	$where_seperator="";
	$sep="";
	//if ($sep=="") $sep="or";
	$args=explode(" ",$kwl);
	//echo "<pre>";print_r($args);echo "</pre>";
	$count=0;
	while ($item=array_shift($args)) {
		$tsep=$sep;
		$item_final=$item;
		if ($count>0) {
			if ($item[0]=="+") {
				$tsep="and";
				$item_final=substr($item,1);
				}
			if ($item[0]=="-") {
				$tsep="and not";
				$item_final=substr($item,1);
				}
				}
		
		$where_clause = $where_clause . " $tsep keywords like '%$item_final%' ";
		$count++;
		$sep="or";
		}
	$query="select * from file_cabinet_templates where $where_clause";
	$query_res=@mysql_query($query);
	$hits_array=array();
	while ($row=@mysql_fetch_object($query_res)) {
		$direct_hits_array[$row->template_id]=1;
		$hits_array[$row->template_id]=$hits_array[$row->template_id] + 1;
		while ($row->template_id!=$row->folder_parent) {
			$row=getoneb("select * from file_cabinet_templates where template_id = '$row->folder_parent'");
			$hits_array[$row->template_id]=$hits_array[$row->template_id] + 1;
			}
		}
	//echo "<pre>";print_r($direct_hits_array);echo "</pre>";
	//tabledump($query);
	
	//echo "<hr>$query<hr>";
	//echo "<hr>foo<hr>";
	
	return($hits_array);
	}

function file_search($kwl) {
	global $global_jobinfo,$direct_hits_array, $fd_color_4;
	$direct_hits_array=array();
	$where_clause="";
	$where_seperator="";
	$sep="";
	//if ($sep=="") $sep="or";
	$args=explode(" ",$kwl);
	//echo "<pre>";print_r($args);echo "</pre>";
	$count=0;
	while ($item=array_shift($args)) {
		$tsep=$sep;
		$item_final=$item;
		if ($count>0) {
			if ($item[0]=="+") {
				$tsep="and";
				$item_final=substr($item,1);
				}
			if ($item[0]=="-") {
				$tsep="and not";
				$item_final=substr($item,1);
				}
			}
		
		$where_clause = $where_clause . " $tsep description like '%$item_final%' ";
		$count++;
		$sep="or";
		}
	$query="select * from file_cabinet where jobinfo_id = '$global_jobinfo->jobinfo_id' and ($where_clause)";
	//tabledump($query);echo "<hr>$query<hr>";
	
	$res=@mysql_query($query);
	$count=@mysql_num_rows($res);
	if ($count<1) {
		echo "<b>No files found matching search criteria!</b>";
		} else {
		while ($row=@mysql_fetch_object($res)) {
			//$files_info=load_file_group_info($row->file_group_id);
			$link=webfile_paperclip_view($row->file_group_id);
			$tree_path=get_tree_path_main($row->tree_id,'Y');
			$creator_info=getoneb("select * from contacts where contacts_id = '$row->creator'");
			$date=invali_date($row->create_date,'/');
			echo "
			<div style=\"border: 1px solid black; background: $fd_color_4;\"><ul>$tree_path</ul></div>
			<table border=0 cellspacing=0 cellpadding=8>
			<tr><td>
				<a href=$pagename?mode=file_cabinet_edit&file_cabinet_id=$row->file_cabinet_id><font color=blue>$row->description
			</td><td>
				$creator_info->display_name
			</td><td>
				$date
			</td><td>
				$link
			</td></tr>
			</table>
			";
			}
		
		}
	
	return 0;
	
	$query_res=@mysql_query($query);
	$hits_array=array();
	while ($row=@mysql_fetch_object($query_res)) {
		$direct_hits_array[$row->template_id]=1;
		$hits_array[$row->template_id]=$hits_array[$row->template_id] + 1;
		while ($row->template_id!=$row->folder_parent) {
			$row=getoneb("select * from file_cabinet_templates where template_id = '$row->folder_parent'");
			$hits_array[$row->template_id]=$hits_array[$row->template_id] + 1;
			}
		}
	//echo "<pre>";print_r($direct_hits_array);echo "</pre>";
	//tabledump($query);
	
	//echo "<hr>$query<hr>";
	//echo "<hr>foo<hr>";
	
	return($hits_array);
	}


function get_root($jobinfo_id="") {
	global $global_jobinfo_id;
	if ($jobinfo_id=="") $jobinfo_id=$global_jobinfo_id;
	return (getoneb("select * from file_cabinet_tree where jobinfo_id = '$jobinfo_id' and parent_id = tree_id"));
	}

function get_tree_path_mini($tree_id) {
	global $fd_color_3, $devel, $ul_style,$doc_id,$doc_edit_id;
	//$ul_style="padding: 0; margin: 0; margin-left: 8px; list-style-type: none;";
	$tree_info=getoneb("select * from file_cabinet_tree where tree_id = '$tree_id'");
	$tree_path_link="";
	$tree_temp_info=$tree_info;
	$depth=1;
	while ($tree_temp_info->tree_id!=$tree_temp_info->parent_id) {
		$indent=$depth * 5;
		
		$tree_path_link="<ul onclick=\"fc_sh_tree('$doc_id','$doc_edit_id','$tree_temp_info->tree_id')\" style=\"$ul_style\"><img src=/global/file_cabinet$devel/down.png> $tree_temp_info->folder_name" . $tree_path_link;
		$tree_temp_info=getoneb("select * from file_cabinet_tree where tree_id = '$tree_temp_info->parent_id'");
		$depth++;
		}
	$tree_path_link="<img onclick=\"fc_sh_tree('$doc_id','$doc_edit_id','$tree_temp_info->tree_id')\" src=/global/file_cabinet$devel/down.png>$global_jobinfo->contract_num: /" . $tree_path_link;
	while ($depth > 1 ) {
		echo "</ul>";
		$depth=$depth - 1;
		}
		
	$tree_path_link="<div onclick=\"add_new_document('$doc_id','$doc_edit_id','$tree_info->tree_id')\" style=\"cursor: hand; cursor: pointer; padding: 0px; font-size: 9px; border: 1px solid black; color: blue; background: $fd_color_3; float: right;\">&nbsp;Create&nbsp;</div>" . $tree_path_link;
	return ($tree_path_link);
	}


function more_folders_there($tree_id) {
	$tree_info=getoneb("select * from file_cabinet_tree where tree_id = '$tree_id'");
	$query="select file_cabinet_templates.template_id,file_cabinet_templates.folder_name as folder_name
	from file_cabinet_templates left join file_cabinet_tree on 
	(file_cabinet_templates.template_id = file_cabinet_tree.template_id and 
	file_cabinet_tree.jobinfo_id = '$tree_info->jobinfo_id') 
	where folder_parent = '$tree_info->template_id' 
	and folder_parent != file_cabinet_templates.template_id and 
	file_cabinet_tree.folder_name is null 
	limit 1";
	return(getoneb($query));
	}



/////////////////////////////////////////////////////////////////////////////
//	Recursive function to create a "folder" if needed.. this is really the
//	only reasonable way to solve this that comes to mind..
/////////////////////////////////////////////////////////////////////////////
function gimme_da_works_baby($jobinfo_id) {
	$res=@mysql_query("select * from file_cabinet_templates where active = 'Y'");
	while ($row=@mysql_fetch_object($res)) {
		//echo "<hr>$row->template_id";
		$result=verify_folder_activated($row->template_id,$jobinfo_id);
		if (!($result)) die("Error creating all folders.. Aborting!");
		}
	return (TRUE);
	}

function gimme_da_root_folders($jobinfo_id) {
	global $template_root;
	$res=@mysql_query("select * from file_cabinet_templates where active = 'Y' and folder_parent = '$template_root'");
	while ($row=@mysql_fetch_object($res)) {
		$result=verify_folder_activated($row->template_id,$jobinfo_id);
		if (!($result)) die("Error creating root folders.. Aborting!");
		}
	return (TRUE);
	}


function verify_folder_activated($template_id,$jobinfo_id,$depth=99) {
	global $global_user_id;
	//echo "boo ($depth/$template_id/$jobinfo_id)<br>";
	$tree_info=getoneb("select * from file_cabinet_tree where template_id = '$template_id' and jobinfo_id = '$jobinfo_id' limit 1");
	//echo "is it there?<pre>";
	//print_r($tree_info);
	//echo "</pre>";
	if ($tree_info) return($tree_info);
	//echo "it's not there...";
	if ($depth<1) return (FALSE);
	$template_info=getoneb("select * from file_cabinet_templates where template_id = '$template_id'");
	if (!($template_info)) return (FALSE);
	if ($template_info->folder_parent==$template_info->template_id) {
		// This is the root folder, and we better just create the damn thing or this thing
		// is gonna go spinning outa control into oblivion (sp?)
		//echo "make da root?";
		create_root($jobinfo_id);
		return(get_root($jobinfo_id));
		die("How did I get here?");
		}
	//echo "ran into the end and ";return (FALSE);
	$parent_tree_info=verify_folder_activated($template_info->folder_parent,$jobinfo_id,($depth - 1));
	if (!($parent_tree_info)) {
		//echo "verify for ($template_id) returned false";
		return (FALSE);
		}
	
	$query="insert into file_cabinet_tree set 
	jobinfo_id = '$parent_tree_info->jobinfo_id',
	parent_id = '$parent_tree_info->tree_id',
	template_id = '$template_info->template_id',
	sort_priority = '$template_info->sort_priority',
	file_count = 0,
	folder_name = '$template_info->folder_name',
	folder_description = '$template_info->folder_description',
	folder_creator = '$global_user_id',
	correspondence_children = '$template_info->for_correspondence'";
	
	//echo "<hr>$query<hr>";
	
	@mysql_query($query);
	$id=@mysql_insert_id();
	//$tree_info=getoneb("select * from file_cabinet_tree where template_id = '$template_id' and jobinfo_id = '$jobinfo_id' limit 1");
	$tree_info=getoneb("select * from file_cabinet_tree where tree_id = '$id'");
	if ($parent_tree_info->correspondence_children=="Y") {
		$query="insert into file_cabinet_tree set 
		jobinfo_id = '$parent_tree_info->jobinfo_id',
		parent_id = '$id',
		template_id = '$template_info->template_id',
		sort_priority = '$template_info->sort_priority',
		file_count = 0,
		folder_name = 'In',
		folder_description = '$template_info->folder_description - In',
		folder_creator = '$global_user_id',
		correspondence_children = 'N'";
		$new_in_res=@mysql_query($query);
		if (!($new_in_res)) die("<hr>$query<hr>");
		$query="insert into file_cabinet_tree set 
		jobinfo_id = '$parent_tree_info->jobinfo_id',
		parent_id = '$id',
		template_id = '$template_info->template_id',
		sort_priority = '$template_info->sort_priority',
		file_count = 0,
		folder_name = 'Out',
		folder_description = '$template_info->folder_description - Out',
		folder_creator = '$global_user_id',
		correspondence_children = 'N'";
		$new_out_res=@mysql_query($query);
		if (!($new_out_res)) die("<hr>$query<hr>");
		}
	return($tree_info);
	}




function get_tree_path_main($tree_id,$hide_sub_folders='N') {
	global $fd_color_3, $devel, $ul_style,$pagename;
	$tree_info=getoneb("select * from file_cabinet_tree where tree_id = '$tree_id'");
	$tree_path_link="";
	$tree_temp_info=$tree_info;
	$depth=1;
	
	$dires=@mysql_query("select * from file_cabinet_tree where jobinfo_id = '$tree_info->jobinfo_id' and parent_id = '$tree_info->tree_id' and tree_id != parent_id order by sort_priority,folder_name");
	$tree_path_link="<ul style=\"$ul_style\">";
	if ($hide_sub_folders=='N') {
		while ($dirow=@mysql_fetch_object($dires)) {
			$tree_path_link=$tree_path_link . "<a href=\"$pagename?mode=main&current_folder=$dirow->tree_id\"><font color=blue>
			<img src=/global/file_cabinet$devel/right.png> $dirow->folder_name <font style=\"background: blue; font-weight: bold;\" color=white>($dirow->file_count)</font></font></a>";
			
			if ($dirow->free_for_all=='Y') {
				$ptemp_info=getoneb("select * from file_cabinet_tree where tree_id = '$dirow->parent_id'");
				if ($ptemp_info->free_for_all=='Y') $tree_path_link=$tree_path_link . "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=$pagename?mode=file_cabinet_tree_edit&tree_id=$dirow->tree_id><i>edit</i></a>";
				}
			$tree_path_link=$tree_path_link . "<br>";
			}
			$tree_path_link=$tree_path_link . "</ul>";
			
			if (more_folders_there($tree_info->tree_id)) {
			$tree_path_link="&nbsp;&nbsp;&nbsp;<a href=$pagename?mode=add_folder&parent_tree_id=$tree_id><font color=red>Add Folder</font></a>" . $tree_path_link ;
			}
		}
	while ($tree_temp_info->tree_id!=$tree_temp_info->parent_id) {
		$indent=$depth * 5;
		$tree_path_link="<ul style=\"$ul_style\">
		<a href=\"$pagename?mode=main&current_folder=$tree_temp_info->tree_id\"><font color=blue>
		<img src=/global/file_cabinet$devel/down.png> $tree_temp_info->folder_name</font></a>" . $tree_path_link;
		$tree_temp_info=getoneb("select * from file_cabinet_tree where tree_id = '$tree_temp_info->parent_id'");
		$depth++;
		}
	$tree_path_link="<a href=\"$pagename?mode=main&current_folder=$tree_temp_info->tree_id\"><font color=blue><img onclick=\"fc_sh_tree('$doc_id','$doc_edit_id','$tree_temp_info->tree_id')\" src=/global/file_cabinet$devel/down.png>$global_jobinfo->contract_num: /</font></a>" . $tree_path_link;
	while ($depth > 1 ) {
		echo "</ul>";
		$depth=$depth - 1;
		}
		
	return ($tree_path_link);
	}


function create_root($jobinfo_id="") {
	global $global_jobinfo_id,$global_jobinfo,$global_user_id,$template_root;
	//die("insert into file_cabinet_tree set jobinfo_id = '$global_jobinfo_id', template_id = '$template_root', sort_priority = '1', folder_name = 'root', folder_description = 'root folder for $global_jobinfo->contract_num', folder_creator = '$global_user_id', correspondence_children = 'N'");
	if ($jobinfo_id=="") $jobinfo_id=$global_jobinfo_id;
	
	$jobinfo=getoneb("select * from jobinfo where jobinfo_id = '$jobinfo_id'");
	$res=@mysql_query("insert into file_cabinet_tree set jobinfo_id = '$jobinfo_id', template_id = '$template_root', sort_priority = '1', folder_name = 'root', folder_description = 'root folder for $jobinfo->contract_num', folder_creator = '$global_user_id', correspondence_children = 'N'");
	if (!($res)) return (FALSE);
	$insert_id=@mysql_insert_id();
	$res=@mysql_query("update file_cabinet_tree set parent_id = '$insert_id' where tree_id = '$insert_id'");
	if (!($res)) return (FALSE);
	return (TRUE);
	}
	
function update_directory_counts ($tree_id) {
	$tree_info=getoneb("select * from file_cabinet_tree where tree_id = '$tree_id'");
	$subfolder_count=getoneb("select sum(file_count) as files from file_cabinet_tree where parent_id = '$tree_info->tree_id' and tree_id != '$tree_info->tree_id'");
	if ($subfolder_count->files=="") $sufolder_count->files="0";
	$folder_count=getoneb("select sum(1) as files from file_cabinet where tree_id = '$tree_info->tree_id'");
	if ($folder_count->files=="") $folder_count->files="0";
	$total=$folder_count->files + $subfolder_count->files;
	$query="update file_cabinet_tree set file_count = '$total' where tree_id = '$tree_info->tree_id'";
	//die("<hr>$query<hr>");
	@mysql_query($query);
	
	$parent_info=getoneb("select * from file_cabinet_tree where tree_id = '$tree_info->parent_id'");
	if (($parent_info)&&($parent_info->tree_id!=$tree_info->tree_id)) update_directory_counts($parent_info->tree_id);
	}




?>
