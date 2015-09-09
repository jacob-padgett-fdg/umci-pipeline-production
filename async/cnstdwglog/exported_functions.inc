<?
function issuances($section="") {
	global $global_jobinfo_id;
	if ($section=="") {
		$section=current_section();
		}
	
	$list_issuances_qry="select * from cnstdwglog_issuances where jobinfo_id = '$global_jobinfo_id' and section = '$section' order by issuance_type desc,issuance_date,sort_priority,name,issuance_id";
	
	$res=@mysql_query($list_issuances_qry);
	while ($row=@mysql_fetch_object($res)) {
		$issuances[]=$row->issuance_id;
		$issuances[]=$row->name;
		$issuances[]=$row->issuance_type;
		$issuances[]=$row->current;
		}
	return($issuances);
	}



function fill_all_gaps($cnstdwglog_id="") {
	global $global_jobinfo_id,$current_cnstdwglog_section;
	if ($cnstdwglog_id!="") $cnstdwglog_id=addslashes($cnstdwglog_id);
	//if ($cnstdwglog_id=="") {
		$mainquery="select * from cnstdwglog_issuances where jobinfo_id = '$global_jobinfo_id' and section = '$current_cnstdwglog_section'";
	//	} else {
	//	$mainquery="select * from cnstdwglog_issuances where jobinfo_id = '$global_jobinfo_id' and section = '$current_cnstdwglog_section' and cnstdwglog_id = '$cnstdwglog_id'";
	//	}
	$res=@mysql_query($mainquery);
	if (!($res)) {
		die("Go get Jeff Please and show him this error.<hr>select * from cnstdwglog_issuances where jobinfo_id = '$global_jobinfo_id' and section = '$current_cnstdwglog_section'");
		}
	$rowcount=@mysql_num_rows($res);
		if ($rowcount < 1) {
			echo "<h2>Warning: Reconciling all issuances not possible at this point</h2><hr>Hint toward the problem:<br>$mainquery<hr>";
			}
	//tabledump("select * from cnstdwglog_issuances where jobinfo_id = '$global_jobinfo_id' and section = '$current_cnstdwglog_section'");
	while ($row=@mysql_fetch_object($res)) {
		
		if ($cnstdwglog_id=="") $dwgres=@mysql_query("select * from cnstdwglog where jobinfo_id = '$global_jobinfo_id' and section = '$current_cnstdwglog_section'");
		else $dwgres=@mysql_query("select * from cnstdwglog where cnstdwglog_id = '$cnstdwglog_id'");
		
		if (!($dwgres)) {
			die("Go get Jeff please and show him this error..<hr>select * from cnstdwglog where jobinfo_id = '$global_jobinfo_id' and section = '$current_cnstdwglog_section'");
			}
		//tabledump("select * from cnstdwglog where jobinfo_id = '$global_jobinfo_id' and section = '$current_cnstdwglog_section'");
		while ($dwg_info=@mysql_fetch_object($dwgres)) {
			if (!(getoneb("select * from cnstdwglog_files where issuance_id = '$row->issuance_id' and cnstdwglog_id = '$dwg_info->cnstdwglog_id' limit 1"))) {
				//echo "<hr>insert into cnstdwglog_files set issuance_id = '$row->issuance_id', cnstdwglog_id = '$dwg_info->cnstdwglog_id'<hr>";
				$insres=@mysql_query("insert into cnstdwglog_files set issuance_id = '$row->issuance_id', cnstdwglog_id = '$dwg_info->cnstdwglog_id'");
				if (!($insres)) {
					die("Go get Jeff please and show him this error...<hr>insert into cnstdwglog_files set issuance_id = '$row->issuance_id', cnstdwglog_id = '$dwg_info->cnstdwglog_id'");
					}
				}
			}
		}
	}


function check_addenda($cnstdwglog_id) {
	global $current_cnstdwglog_section;
	$addenda=array();
	$drawing_info=getoneb("select * from cnstdwglog where cnstdwglog_id = '$cnstdwglog_id'");
	if ($drawing_info) $current_cnstdwglog_section=$drawing_info->section;
	else return FALSE;
	$issuance_list=issuances($drawing_info->section);
	reset($issuance_list);
	$current_id=current($issuance_list);
	$current_name=next($issuance_list);
	$current_type=next($issuance_list);
	$current_current=next($issuance_list);
	while ($current_id) {
		$previous_items=array_keys($addenda);
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
			if ($addenda[$unique_id]['status']=='') {
				$addenda[$unique_id]['status']=$issuance_addendum->required_in_this_issuance;
				$addenda[$unique_id]['posted']=$issuance_addendum->posted;
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
				////////////////////////////////////////////////////////////////////
				// If we have an N in our Posted stuff.. need to keep working on 
				// it.. This currently doesn't work right and needs new fields 
				// added to make it.. 
				////////////////////////////////////////////////////////////////////
				//if ($addenda[$unique_id]['status']=='Y') {
				//	if ($issuance_addendum->required_in_this_issuance=='N') {
				//		$addenda[$unique_id]['status']='N';
				//		}
				//	}
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

?>
