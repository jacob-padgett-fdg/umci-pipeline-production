<?

/*

Example queries:

	// Give me the last issuance with a drawing attached, and the rest of the basics
	
	select * from cnstdwglog_files 
	
	right join cnstdwglog_issuances on (cnstdwglog_files.issuance_id = cnstdwglog_issuances.issuance_id)
	right join webfile_groups on ( cnstdwglog_files.file_group_id = webfile_groups.file_group_id )
	right join webfile_files on ( webfile_groups.file_group_id = webfile_files.file_group_id )
	where cnstdwglog_id = '$cnstdwglog_info->cnstdwglog_id'
				
	order by 
	cnstdwglog_issuances.issuance_type,
	cnstdwglog_issuances.issuance_date desc,
	cnstdwglog_issuances.sort_priority desc,
	cnstdwglog_issuances.name desc,
	cnstdwglog_issuances.issuance_id desc
	limit 1

	// give me all the actual drawing issuances for a drawing starting with the last issuance and 
	// working back to the original issue from there.
	select * from cnstdwglog_files 
	
	right join cnstdwglog_issuances on (cnstdwglog_files.issuance_id = cnstdwglog_issuances.issuance_id)
	right join webfile_groups on ( cnstdwglog_files.file_group_id = webfile_groups.file_group_id )
	right join webfile_files on ( webfile_groups.file_group_id = webfile_files.file_group_id )
	where cnstdwglog_id = '$cnstdwglog_info->cnstdwglog_id'
				
	order by 
	cnstdwglog_issuances.issuance_type,
	cnstdwglog_issuances.issuance_date desc,
	cnstdwglog_issuances.sort_priority desc,
	cnstdwglog_issuances.name desc,
	cnstdwglog_issuances.issuance_id desc

	// Give me all the issuance for a drawing in order and include attachment info
	select * from cnstdwglog_files 
	
	right join cnstdwglog_issuances on (cnstdwglog_files.issuance_id = cnstdwglog_issuances.issuance_id)
	right join webfile_groups on ( cnstdwglog_files.file_group_id = webfile_groups.file_group_id )
	right join webfile_files on ( webfile_groups.file_group_id = webfile_files.file_group_id )
	where cnstdwglog_id = '$cnstdwglog_info->cnstdwglog_id'
				
	order by 
	cnstdwglog_issuances.issuance_type desc,
	cnstdwglog_issuances.issuance_date,
	cnstdwglog_issuances.sort_priority,
	cnstdwglog_issuances.name,
	cnstdwglog_issuances.issuance_id

















*/
















$write=FALSE;
$guest=FALSE;

if (pm_for_this_job()) $application_user_type="admin";
if ($application_user_type=="admin") $write=TRUE;
if ($application_user_type=="full") $write=TRUE;
if ($application_user_type=="guest") $guest=TRUE;

function current_section() {
	global $application_name;
	$global_section_name='current_' . $application_name . '_section';
	return($GLOBALS[$global_section_name]);
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
    echo "<form class=\"noprint\" style=\"display: inline;\" name=select_section method=post action=$pagename>Section&nbsp;<input type=hidden name=mode value=\"$mode\">";dropbox("select section as new_application_section,section from ${application_name}_sections where jobinfo_id = '$global_jobinfo_id' and active = 'Y'",$GLOBALS[$global_section_name],1,"submit()");
	if ($adminuser) echo "&nbsp;&nbsp;<a class=\"noprint\" href=$pagename?mode=manage_sections><font class=\"noprint\" color=blue><i>Edit List</i></font></a>";
	echo"</form>";
	}

//require ("fill_all_gaps.inc");
//require ("check_addenda.inc");
require ("exported_functions.inc");

function current_issuance() {
	global $global_jobinfo_id;
	$issuance_info=getoneb("select * from cnstdwglog_issuances where jobinfo_id = '$global_jobinfo_id' order by issuance_type,sort_priority desc,issuance_id desc,name desc limit 1");
	return ($issuance_info->issuance_id);
	}

/*
function check_addenda($cnstdwglog_id) {
	global $current_cnstdwglog_section;
	$addenda=array();
	$drawing_info=getoneb("select * from cnstdwglog where cnstdwglog_id = '$cnstdwglog_id'");
	if ($drawing_info) $current_cnstdwglog_section=$drawing_info->section;
	else return FALSE;
	//echo "naaaaaaaaaaaaaaaaaaaacccccccccccchhhhhhhhhhhhhhhhhhhhhhhoooooooooooooo";
	$issuance_list=issuances();
	reset($issuance_list);
	//echo "<pre>";print_r($issuance_list);echo "</pre>";
	$current_id=current($issuance_list);
	$current_name=next($issuance_list);
	$current_type=next($issuance_list);
	$current_current=next($issuance_list);
	//$current_id=current($issuance_list);
	//$current_name=next($issuance_list);
	//$current_type=next($issuance_list);
	//$current_current=next($issuance_list);
	while ($current_id) {
		//echo "<hr>$current_id";
		
		////////////////////////////
		// First check known addenda
		// and make sure they exist
		// (but do it later)
		////////////////////////////
		// Check the addenda array contents
		////////////////////////////
		//echo "<hr><pre>";
		//print_r(array_keys($addenda));
		//echo "</pre>";
		$previous_items=array_keys($addenda);
		//print_r($previous_items);echo "<hr>";
		while ($previous_item=array_pop($previous_items)) {
			if ($previous_item!="") {
				$type=$addenda[$previous_item]['type'];
				$status=$addenda[$previous_item]['status'];
				$value=$addenda[$previous_item]['value'];
				$description=$addenda[$previous_item]['description'];
				if ($status=='Y') {
					if ($type=='document') {
						$keyfield='doc_id';
						} else {
						$keyfield='parent_addenda_id';
						}
					$exists=getoneb("select * from cnstdwglog_addenda left join cnstdwglog_files on (cnstdwglog_addenda.files_id = cnstdwglog_files.files_id) where cnstdwglog_files.issuance_id = '$current_id' and cnstdwglog_files.cnstdwglog_id = '$cnstdwglog_id' and $keyfield = '$value' limit 1");
					
					if (!($exists)) {
						$cnstdwglog_file_info=getoneb("select * from cnstdwglog_files where cnstdwglog_id = '$cnstdwglog_id' and issuance_id = '$current_id'");
						if (!($cnstdwglog_file_info)) die ("Application error getting file information during reconciliation");
						if ($keyfield=="addendum") $additional=",parent_addenda_id = '$value'";
						@mysql_query("insert into cnstdwglog_addenda set files_id = '$cnstdwglog_file_info->files_id', description = '$description', $keyfield = '$value', required_in_this_issuance = 'Y' $additional"); 
						}
					
					
					
					}
				
				}
			}
		
		
		
		
		// Next check all addenda in this issuance against the addenda array
		// ...note, we may dupe some work there, but it's not as bad as any of 
		//    the other schemes I've come up with.. It gets *REALLY* complicated
		//    to avoid duping any work.
		
		// $current_id is the issuance_id
		// $cnstdwglog_id is the id of the drawing
		// gonna have to do a join to find the cnstdwglog_addenda to go with that.. 
		
		//$issuance_addenda_res=@mysql_query("select * from cnstdwglog_addenda where ");
		//tabledump("select * from cnstdwglog_addenda left join cnstdwglog_files on (cnstdwglog_addenda.files_id = cnstdwglog_files.files_id) where cnstdwglog_files.issuance_id = '$current_id' and cnstdwglog_files.cnstdwglog_id = '$cnstdwglog_id'");
		$issuance_addenda_res=@mysql_query("select * from cnstdwglog_addenda left join cnstdwglog_files on (cnstdwglog_addenda.files_id = cnstdwglog_files.files_id) where cnstdwglog_files.issuance_id = '$current_id' and cnstdwglog_files.cnstdwglog_id = '$cnstdwglog_id'");
		while ($issuance_addendum=@mysql_fetch_object($issuance_addenda_res)) {
			$unique_id="$issuance_addendum->doc_id xx $issuance_addendum->parent_addenda_id";
			//echo "<hr>xxxx $unique_id xxxxx ";
			if ($addenda[$unique_id]['status']=='') {
				//echo "<hr>setting status to " . $issuance_addendum->required_in_this_issuance . "<hr>";
				$addenda[$unique_id]['status']=$issuance_addendum->required_in_this_issuance;
				$addenda[$unique_id]['description']=$issuance_addendum->description;
				
				if ($issuance_addendum->doc_id>$issuance_addendum->parent_addenda_id) {
					$addenda[$unique_id]['type']='document';
					$addenda[$unique_id]['value']=$issuance_addendum->doc_id;
					} else { 
					$addenda[$unique_id]['type']='addendum';
					$addenda[$unique_id]['value']=$issuance_addendum->parent_addenda_id;
					}
				} else {
				///////////////////////////////////////////////////////////////////
				// If we're running with a N from last round, then we need to just
				// go ahead and delete whatever record we tripped onto.. it was
				// marked obsolete in the last issuance.. 
				///////////////////////////////////////////////////////////////////
				if ($addenda[$unique_id]['status']=='N') {
					//echo "delete<hr>";
					@mysql_query("delete from cnstdwglog_addenda where addenda_id = '$issuance_addendum->addenda_id'");
					}
				///////////////////////////////////////////////////////////////////
				// If we've got a Y in our array (addendum), but we have a N
				// in our database for this record, we do nothing but alter the
				// array so that all future issuances of this addendum are deleted
				///////////////////////////////////////////////////////////////////
				if ($addenda[$unique_id]['status']=='Y') {
					if ($issuance_addendum->required_in_this_issuance=='N') {
						$addenda[$unique_id]['status']='N';
						}
					}
				///////////////////////////////////////////////////////////////////
				// If we're finding a Y in both, then nothing really needs to be 
				// done.. we just continue.. 
				///////////////////////////////////////////////////////////////////
				}
			}
		$current_id=next($issuance_list);
		$current_name=next($issuance_list);
		$current_type=next($issuance_list);
		$current_current=next($issuance_list);
		}
	}

*/
?>
