<?
$employee_group=1;
$labor_manager=371;
$sheet_metal_manager=1043;
$teamster_manager=406;


function is_child_free($lam_id) {
	return(!(getoneb("select * from lam where parent_lam_id = '$lam_id' limit 1")));
	}

function lacks_recent_time($lam_id,$date_mysql) {
	$lam_info=getoneb("select * from lam where lam_id = '$lam_id'");
	$lamchop_info=getoneb("select * from lamchops where lamchops_id = '$lam_info->lamchops_id'");
	$start_date=invali_date($date_mysql);
	$end_date=invali_date($lamchop_info->week_ending);
	$jobinfo=getoneb("select * from jobinfo where jobinfo_id = '$lam_info->jobinfo_id'");
	$employee_info=getoneb("select * from contacts where contacts_id = '$lam_info->employee_id'");
	if (!($jobinfo)) die("Huh.. can't figure out job information in 'lacks_recent_time' function..(lam_id: $lam_id)");
	$contract_num=get_vp_contract_num($jobinfo->jobinfo_id);
	$res=@mssql_query("select * from PRTH with (NOLOCK) where Employee = '$employee_info->employee_num' and Job like '$contract_num%' and PREndDate >= '$start_date' and PREndDate <= '$end_date' and Hours != 0");
	$rows=@mssql_num_rows($res);
	if ($rows<=0) return(TRUE);
	else return(FALSE);
	}

function has_long_history($lam_id) {
	global $labor_manager;
	$lam_info=getoneb("select * from lam where lam_id = '$lam_id'");
	if ($lam_info->employee_id==$labor_manager) return TRUE;
	$lamchop_info=getoneb("select * from lamchops where lamchops_id = '$lam_info->lamchops_id'");
	
	$last_week_mysql=date('Y/m/d',strtotime("-7 days",strtotime($lamchop_info->week_ending)));
	$last_week_lamchop=getoneb("select * from lamchops where prime_cut = 'Y' and week_ending = '$last_week_mysql'");
	$last_week_lam=getoneb("select * from lam where lamchops_id = '$last_week_lamchop->lamchops_id' and jobinfo_id = '$lam_info->jobinfo_id' and employee_id = '$lam_info->employee_id'");
	
	if (!($last_week_lam)) return(FALSE);
	
	$last_week_mysql=date('Y/m/d',strtotime("-7 days",strtotime($last_week_lamchop->week_ending)));
	$last_week_lamchop=getoneb("select * from lamchops where prime_cut = 'Y' and week_ending = '$last_week_mysql'");
	$last_week_lam=getoneb("select * from lam where lamchops_id = '$last_week_lamchop->lamchops_id' and jobinfo_id = '$lam_info->jobinfo_id' and employee_id = '$lam_info->employee_id'");
	if (!($last_week_lam)) return(FALSE);
	
	$last_week_mysql=date('Y/m/d',strtotime("-7 days",strtotime($last_week_lamchop->week_ending)));
	$last_week_lamchop=getoneb("select * from lamchops where prime_cut = 'Y' and week_ending = '$last_week_mysql'");
	$last_week_lam=getoneb("select * from lam where lamchops_id = '$last_week_lamchop->lamchops_id' and jobinfo_id = '$lam_info->jobinfo_id' and employee_id = '$lam_info->employee_id'");
	if (!($last_week_lam)) return(FALSE);
	
	$last_week_mysql=date('Y/m/d',strtotime("-7 days",strtotime($last_week_lamchop->week_ending)));
	$last_week_lamchop=getoneb("select * from lamchops where prime_cut = 'Y' and week_ending = '$last_week_mysql'");
	$last_week_lam=getoneb("select * from lam where lamchops_id = '$last_week_lamchop->lamchops_id' and jobinfo_id = '$lam_info->jobinfo_id' and employee_id = '$lam_info->employee_id'");
	if (!($last_week_lam)) return(FALSE);
	
	$last_week_mysql=date('Y/m/d',strtotime("-7 days",strtotime($last_week_lamchop->week_ending)));
	$last_week_lamchop=getoneb("select * from lamchops where prime_cut = 'Y' and week_ending = '$last_week_mysql'");
	$last_week_lam=getoneb("select * from lam where lamchops_id = '$last_week_lamchop->lamchops_id' and jobinfo_id = '$lam_info->jobinfo_id' and employee_id = '$lam_info->employee_id'");
	if (!($last_week_lam)) return(FALSE);
	
	$last_week_mysql=date('Y/m/d',strtotime("-7 days",strtotime($last_week_lamchop->week_ending)));
	$last_week_lamchop=getoneb("select * from lamchops where prime_cut = 'Y' and week_ending = '$last_week_mysql'");
	$last_week_lam=getoneb("select * from lam where lamchops_id = '$last_week_lamchop->lamchops_id' and jobinfo_id = '$lam_info->jobinfo_id' and employee_id = '$lam_info->employee_id'");
	if (!($last_week_lam)) return(FALSE);
	
	return($last_week_mysql);
	}


function lam_copy_forward($lam_id,$weeks=3) {
	global $labor_manager;
	if ($weeks<=0) return TRUE;
	$lam_info=getoneb("select * from lam where lam_id = '$lam_id'");
	
	$lamchop_info=getoneb("select * from lamchops where lamchops_id = '$lam_info->lamchops_id'");
	$parent_info=getoneb("select * from lam where lam_id = '$lam_info->parent_lam_id'");
	if ((!($parent_info))&&($lam_info->employee_id!=$labor_manager)) {
		echo "<hr>no parent info.. aborting<hr>";
		return FALSE;
		} 

	$next_week_mysql=date('Y/m/d',strtotime("+7 days",strtotime($lamchop_info->week_ending)));
	$new_lamchop_query="select * from lamchops where prime_cut = 'Y' and week_ending = '$next_week_mysql' and ( status = 'new' or status = 'locked')";
	$new_lamchop_info=getoneb($new_lamchop_query);
	if (!($new_lamchop_info)) return FALSE;
	
	$new_lam=getoneb("select * from lam where lamchops_id = '$new_lamchop_info->lamchops_id' and jobinfo_id = '$lam_info->jobinfo_id' and employee_id = '$lam_info->employee_id'");
	if (!($new_lam)) {
		if ($lam_info->employee_id == $labor_manager) {
			return(TRUE);
			}
		$new_parent=getoneb("select * from lam where lamchops_id = '$new_lamchop_info->lamchops_id' and jobinfo_id = '$parent_info->jobinfo_id' and employee_id = '$parent_info->employee_id'");
		if (!($new_parent)) {
			lam_copy_forward($lam_info->parent_lam_id,$weeks);
			} 
		// wrong job here because it was the labor manager.. 
		$new_parent=getoneb("select * from lam where lamchops_id = '$new_lamchop_info->lamchops_id' and jobinfo_id = '$parent_info->jobinfo_id' and employee_id = '$parent_info->employee_id'");
		if (!($new_parent)) return (FALSE);
		@mysql_query("insert into lam set lamchops_id = '$new_lamchop_info->lamchops_id', parent_lam_id = '$new_parent->lam_id', jobinfo_id = '$lam_info->jobinfo_id', employee_id = '$lam_info->employee_id',lam_note = 'copied forward from timesheet(s)'");
		$new_lam=getoneb("select * from lam where lamchops_id = '$new_lamchop_info->lamchops_id' and jobinfo_id = '$lam_info->jobinfo_id' and employee_id = '$lam_info->employee_id'");
		if (!($new_lam)) {
			echo "ERROR: couldn't create the next lam.. aborting copy forward function..";
			return (FALSE);
			}
		} 
	$weeks=$weeks - 1;
	lam_copy_forward($new_lam->lam_id);
	}



function find_default_lamchop($date) {
	$date_info=date_info($date);
	$lamchop=getoneb("select * from lamchops where week_ending = '$date_info->week_end_mysql' and prime_cut = 'Y'");
	return ($lamchop->lamchops_id);
	}

function find_tree_top($lamchop_id) {
	$topofthechop=getoneb("select * from lam where lamchops_id = '$lamchop_id' and parent_lam_id = 0");
	if ($topofthechop) return ($topofthechop->lam_id);
	else return (FALSE);
	}

function place_me($old_lamchops_id,$lam_id,$ignore_last_week='N') {
	//echo "<hr>$old_lamchops_id<hr>";exit;
	$lam_info=getoneb("select * from lam where lam_id = '$lam_id'");
	$emp_info=getoneb("select * from contacts where contacts_id = '$lam_info->employee_id'");
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// First try.. do we have an exact match from the "other" week. Only try it though if the old ID
	// was set to a non-zero number. If zero is set we just skip this test/trick.. 
	$tray_lam=FALSE;
	if ($old_lamchops_id!=0) {
		if ($ignore_last_week=='Y') $try_lam=getoneb("select * from lam where lamchops_id = '$old_lamchops_id' and jobinfo_id = '$lam_info->jobinfo_id' and employee_id = '$lam_info->employee_id'");
		if ($try_lam) {
			$try_lam_parent=getoneb("select * from lam where lam_id = '$try_lam->parent_lam_id'");
			if ($try_lam_parent) {
				$pi_lam=getoneb("select * from lam where employee_id = '$try_lam_parent->employee_id' and jobinfo_id = '$try_lam_parent->jobinfo_id' and lamchops_id = '$lam_info->lamchops_id'");
				if ($pi_lam) {
					@mysql_query("update lam set parent_lam_id = '$pi_lam->lam_id' where lam_id = '$lam_info->lam_id'");
					return TRUE;
					}
				}
			}
		}
	
	$job_info=getoneb("select * from jobinfo where jobinfo_id = '$lam_info->jobinfo_id'");
	if (($job_info->contract_num<1000)||($job_info->contract_num>9999)) {
		return FALSE;
		}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Second... Exclude updating if they're a general foreman since I'd not now how to place them
	// unless I had already been told last week.
	if ($emp_info->employee_class=="General Foreman") {
		return FALSE;
		}
	
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// Third... If they're not a foreman, then assign them to the only foreman of their trade
	if ($emp_info->employee_class!="Foreman") {
		$pi_lam=getoneb("select * from lam right join contacts on (contacts.contacts_id = lam.employee_id) where lamchops_id = '$lam_info->lamchops_id' and lam.jobinfo_id = '$lam_info->jobinfo_id' and employee_class = 'Foreman' and employee_trade = '$emp_info->employee_trade'");
		if ($pi_lam) {
			@mysql_query("update lam set parent_lam_id = '$pi_lam->lam_id' where lam_id = '$lam_info->lam_id'");
			return TRUE;
			}
		}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// At this point, we only have foreman, or people without them left.. We can now lose all
	// the "if's" about foreman cause we do the same thing either way here..
	////////////////////////////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Fourth... If they're a foreman, try to find a (single) general foreman in their trade 
	// to put them under..
	//if ($emp_info->employee_class=="Foreman") {
	$pi_lam=getoneb("select * from lam right join contacts on (contacts.contacts_id = lam.employee_id) where lamchops_id = '$lam_info->lamchops_id' and lam.jobinfo_id = '$lam_info->jobinfo_id' and employee_class = 'General Foreman' and employee_trade = '$emp_info->employee_trade'");
	if ($pi_lam) {
		@mysql_query("update lam set parent_lam_id = '$pi_lam->lam_id' where lam_id = '$lam_info->lam_id'");
		return TRUE;
		}
	//	}
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Fifth... If they're a foreman, try to find a (single) general foreman not in their trade 
	// to put them under..
	//if ($emp_info->employee_class=="Foreman") {
	$pi_lam=getoneb("select * from lam right join contacts on (contacts.contacts_id = lam.employee_id) where lamchops_id = '$lam_info->lamchops_id' and lam.jobinfo_id = '$lam_info->jobinfo_id' and employee_class = 'General Foreman'");
	if ($pi_lam) {
		@mysql_query("update lam set parent_lam_id = '$pi_lam->lam_id' where lam_id = '$lam_info->lam_id'");
		return TRUE;
		}
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// At this point, I think we're out of guesses for now.. We'll just return a false and forget
	// about the move.. So sorry Matt, but more sorting for you to do.. 
	///////////////////////////////////////////////////////////////////////////////////////////////////
	return FALSE;
	}

function copy_lam_children($src,$dst) {
	$src_info=getoneb("select * from lam where lam_id = '$src'");
	$dst_info=getoneb("select * from lam where lam_id = '$dst'");
	if (!($src_info||$dst_info)) {
		echo "Warning, invalid src or dst lam.. ($src/$dst). Please contact your system administrator!";
		return FALSE;
		}
	$src_children_res=@mysql_query("select * from lam where parent_lam_id = '$src_info->lam_id'");
	while ($child=@mysql_fetch_object($src_children_res)) {
		$new_child_lam_qry="insert into lam set parent_lam_id = '$dst_info->lam_id',jobinfo_id = '$child->jobinfo_id',employee_id = '$child->employee_id',lamchops_id = '$dst_info->lamchops_id', lam_note = '$child->lam_note'";
		mysql_query($new_child_lam_qry);
		$id=@mysql_insert_id();
		copy_lam_children($child->lam_id,$id);
		}
	}

function delete_lam ($lam_id,$lamchops_id) {
	
	if ($lam_id==0) return (FALSE);
	if ($lam_id=="") return (FALSE);
	$lam_id=addslashes($lam_id);
	$lam_info=getoneb("select * from lam where lam_id = '$lam_id'");
	if (!($lam_info)) return (FALSE);
	$sublamres=@mysql_query("select * from lam where parent_lam_id = '$lam_info->lam_id'");
	while ($row=@mysql_fetch_object($sublamres)) {
		delete_lam($row->lam_id,$lamchops_id);
		}
	@mysql_query("delete from lam where lam_id = '$lam_id'");
	
	}

function verify_lam_sanity($target_lam_id,$contact_id) {
	global $labor_manager;
	
	$lam_info=getoneb("select * from lam where lam_id = '$target_lam_id'");
	$parent_lam_info=getoneb("select * from lam where lam_id = '$lam_info->parent_lam_id'");
	$count=99;
	while ($count>0) {
			if ($lam_info->employee_id==$contact_id) return (FALSE);
			$count=$count -1;
			if (!($parent_lam_info)) return(TRUE);
			$lam_info=$parent_lam_info;
			$parent_lam_info=getoneb("select * from lam where lam_id = '$lam_info->parent_lam_id'");
			}
	return (FALSE);
	}

function recursively_set_job($lam_id,$jobinfo_id,$depth=99) {
	if ($depth>100) return FALSE;
	@mysql_query("update lam set jobinfo_id = '$jobinfo_id' where lam_id = '$lam_id'");
	$res=@mysql_query("select * from lam where parent_lam_id = '$lam_id'");
	$depth=$depth - 1;
	if ($depth<1) return FALSE;
	while ($row=@mysql_fetch_object($res)) {
		recursively_set_job($row->lam_id,$jobinfo_id,$depth);
		}
	}

function print_job_header($lam_id,$jobinfo_id) {
	global $pagename,$fd_color_4,$mode,$report_name,$fd_color_3;
	$lam_info=getoneb("select * from lam where lam_id = '$lam_id'");
	$job_info=getoneb("select * from jobinfo where jobinfo_id = '$jobinfo_id'");
	$lamchop_info=getoneb("select * from lamchops where lamchops_id = '$lam_info->lamchops_id'");
	//echo "$lamchop_info->week_ending";
	$lamchop_date=invali_date($lamchop_info->week_ending,'/');
	echo "<div id=\"render_jobid-$job_info->jobinfo_id-$lam_info->lamchops_id\" style=\"padding-bottom: 2px;\"><div id=\"jobid-$job_info->jobinfo_id-$lam_info->lamchops_id\" onmouseover=\"this.catcher_type='active_job';above_object(this)\" onmouseout=\"not_above_object(this)\" style=\"z-index: 4; position: relative; page-break-before: always; height: 16px; width: 550px; border: 2px solid black; background: $fd_color_4;\">
		
		 ";
		 if ($job_info->work_recovery_job=='Y') {
		 	echo "<div style=\"height: 15px; float: right; background: $fd_color_3; border: 1px solid black;\"><font size=-1><b>WR</b></font></div>";
		 	}
		 echo "
		 <div style=\"height: 16px; float: right;\"><b><font size=-1>$job_info->contract_type&nbsp;</font></b></div>
		
		
		<a href=$pagename?mode=$mode&report_name=$report_name&lamchops_id=$lam_info->lamchops_id&default_jobinfo_id=$job_info->jobinfo_id><font color=blue>
		$job_info->contract_num</font></a>
		 - $job_info->job_name</div>

		 </div>";
	}

///////////////////////////////////////////////////////////
//		print_lam_div ($lam_id,$options)
///////////////////////////////////////////////////////////
//		options['parentnodrag']='Y' 
//			Makes just this instance of the lam_div
//			not dragable.. others below still will be	
//		options['childrennodrag']='Y'
//			Makes all lam_divs not dragable from here down
//		options['depth']=##
//			This is how deep to follow lam_divs.. if it's
//			set to "" it's the same is infinity.. This gets
//			decremented before calling children to control
//			the recursion.
//		options['copy']='Y'
//			This option tells javascript to move the div
//			back to it's original location after the copy
// 			and to not destroy/hide the containing div when done
//		options['jobinfo_id']=''
//			If this is set to something, then only lams with this
//			jobinfo_id will be used.. AKA: Shows one job at a time.
//		options['suppress_job_header']='N'
//			If this is set, then the automatic job header/seperator is disabled..
//		options['redraw_hint']='N'
//			If this is set, it tries to default things toward what is desired
//			in a redraw situation. Currently it sets the default job header
//			suppression to Y, but that can still be overriden with an explicit
//			setting of the option.. 
//		options['replacement_text']=''
//			If this is not null, then this is the text that shows up where
//			name/local normally does.
//		options['exceptions_object']='';
//			If this is set to 'Y', then the javascript will know to pass a 
//			pay it forward option to the db updater when an object is dropped
//			into the database, and dropped in entries will get applied to the
//			next few weeks as well.


function print_lam_div($lam_id,$options="") {
	global $trade_details,$labor_manager,$fd_color_3, $fd_color_4, $fd_color_1,$fd_color_2,$last_lam_jobinfo_id,$mode,$report_name;
	$lam_info=getoneb("select * from lam where lam_id = '$lam_id'");
	$emp_info=getoneb("select * from contacts where contacts_id = '$lam_info->employee_id'");
	$job_info=getoneb("select * from jobinfo where jobinfo_id = '$lam_info->jobinfo_id'");
	if ($options['exceptions_object']=='Y') $exceptions_script="this.exceptions_object='Y';";
	else $exceptions_script="";
	$parent_info=getoneb("select * from lam where lam_id = '$lam_info->parent_lam_id'");
	$suppress='N';
	if ($options['redraw_hint']=='Y') {
		$suppress='Y';
		}
	if ($options['suppress_job_header']!='') $suppress=$options['suppress_job_header'];
	$jobopt=addslashes($options['jobinfo_id']);
	////////////////////////////////////////////////////////////////////////////////////
	if ($job_info->jobinfo_id!=$last_lam_jobinfo_id) {
		if ($parent_info->jobinfo_id != $lam_info->jobinfo_id) {
			if (($suppress!='Y')&&($lam_info->jobinfo_id!=0)&&($lam_info->jobinfo_id!=999999)) {
				$lamchop_info=getoneb("select * from lamchops where lamchops_id = '$lam_info->lamchops_id'");
				$lamchop_date=invali_date($lamchop_info->week_ending,'/');
				echo "<div id=\"render_jobid-$job_info->jobinfo_id-$lam_info->lamchops_id\" style=\"padding-top: 15px; padding-bottom: 1px;\"><div id=\"jobid-$job_info->jobinfo_id-$lam_info->lamchops_id\" onmouseover=\"this.catcher_type='lam_job';above_object(this)\" onmouseout=\"not_above_object(this)\" style=\"z-index: 4; position: relative; page-break-before: always; height: 16px; width: 600px; border: 2px solid black; background: $fd_color_4;\">";
					 if ($job_info->work_recovery_job=='Y') {
					 	echo "<div style=\"height: 16px; float: right; background: $fd_color_3;\"><b><font size=-1>WR</font></b></div>";
					 	}
					 echo "
					 <div style=\"height: 16px; float: right;\"><b><font size=-1>$job_info->contract_type&nbsp;</font></b></div>
					<a href=$pagename?mode=$mode&report_name=$report_name&lamchops_id=$lam_info->lamchops_id&default_jobinfo_id=$job_info->jobinfo_id><font color=blue>$job_info->contract_num</font></a><font size=-1><b> - $job_info->job_name <i>(Week End: $lamchop_date)</b></font></i>
					 </div></div>";
				}
			}
		}
	////////////////////////////////////////////////////////////////////////////////////
	
	$last_lam_jobinfo_id=$job_info->jobinfo_id;
	$bgcolor=$trade_details[$emp_info->employee_trade][$emp_info->employee_class];
	$textcolor="#000000";
	if ($emp_info->current=='N') {
		$textcolor="#aaaaaa";
		$bgcolor="#000000";
		}
	$first_letter_trade=strtoupper(substr($emp_info->employee_trade,0,1));
	$first_letter_class=substr($emp_info->employee_class,0,1);
	$class="dragme";
	if ($options['childrennodrag']=='Y') $options['parentnodrag']='Y';
	if ($options['parentnodrag']=='Y') { $options['parentnodrag']='N'; $class="dragmenot"; }
	$d=$options['depth'];
	if ($options['copy']=='Y') {
		$copyadd="title=\"drag to copy\"";
		$copyaddstyle="";
		}
	
	$trade_insert="<div title=\"$emp_info->employee_trade\" style=\"width: 20px; float: left; background: white; border-right: 1px solid black;\">&nbsp;$first_letter_trade&nbsp;</div>";
	$class_insert="<div title=\"$emp_info->employee_class\" style=\"width: 20px; float: right; background: white; border-left: 1px solid black; border-right: 1px solid black; float: right;\">&nbsp;$first_letter_class&nbsp;</div>";
	$delete_insert="<div title=\"Delete Branch\" style=\"width: 20px; float: right; border: 1px solid white; float: right; color: white; background: darkblue;\" onclick=\"lam_del($lam_info->lam_id)\">&nbsp;X&nbsp;</div>";
	$tools_insert="<div title=\"Info/tools\" style=\"width: 20px; float: right; border-top: 1px solid white; border-left: 1px solid white; float: right; color: white; background: darkblue;\" onclick=\"lam_tools($lam_info->lam_id)\">&nbsp;?&nbsp;</div>";
	if ($options['hide_notes']=='Y') {
		$visible_object_width="325";
		$note_insert="";
		} else {
		$visible_object_width="705";
		if ($options['notes_read_only']=='Y') {
			$lam_onclick="";
			} else {
			$lam_onclick="notes_click(this)";
			}
		$note_insert="<div onclick=\"$lam_onclick\" id=\"lam_edit_note_$lam_info->lam_id\" title=\"Edit Note\" style=\"overflow: hidden; width: 380px; height: 18px; float: right; float: right; background: white;\">Note:&nbsp;$lam_info->lam_note</div>";
		}
	$drag_object_width=$visible_object_width + 5;
	
	$job_selection=FALSE;
	$outer_div_color="#FFFFFF";
	if (($lam_info->jobinfo_id == 999999)&&($lam_info->employee_id != $labor_manager)) {
		$job_selection=TRUE;
		}
	if ($job_selection) {
		dropbox("select jobinfo_id,contract_num, ' - ', job_name from jobinfo where active = 'Y' order by contract_num","","","update_lam_job($lam_info->lam_id,this.value)");
		}
	
	$other_lams=getoneb("select sum(1) as total from lam where lamchops_id = '$lam_info->lamchops_id' and employee_id = '$lam_info->employee_id'");
	if ($options['replacement_text']!="") $lam_text=$options['replacement_text']; 
	else $lam_text="${job_insert}$emp_info->display_name<i>($emp_info->employee_union_local)</i>";
	echo "<div style=\"cursor: hand; cursor: pointer; width: ${drag_object_width}px; border: 0px solid black; border-collapse: collapse;\"><div $copyadd id=\"render_lam_$lam_info->lam_id\" style=\"width: 710px; $copyaddstyle z-index: 0; background: $outer_div_color;\"><div $copyadd id=\"lam_$lam_info->lam_id\" class=\"$class\" onmouseover=\"${exceptions_script}above_object(this)\" onmouseout=\"not_above_object(this)\" style=\"$copyaddstyle width: ${visible_object_width}px; border: 1px solid black; color: $textcolor; background: $bgcolor;\">${note_insert}${delete_insert}${tools_insert}${class_insert}${trade_insert}$lam_text</div>";
	
	if ($jobopt) {
		if ($lam_info->employee_id==$labor_manager) {
			print_job_header($lam_info->lam_id,$jobopt);
			/*$job_info=getoneb("select * from jobinfo where jobinfo_id = '$jobopt'");
			echo "<div id=\"render_jobid-$job_info->jobinfo_id-$lam_info->lamchops_id\" style=\"padding-bottom: 2px;\"><div id=\"jobid-$job_info->jobinfo_id-$lam_info->lamchops_id\" onmouseover=\"this.catcher_type='active_job';above_object(this)\" onmouseout=\"not_above_object(this)\" style=\"z-index: 4; position: relative; page-break-before: always; height: 16px; width: 550px; border: 2px solid black; background: $fd_color_4;\">
			<a href=$pagename?mode=$mode&report_name=$report_name&lamchops_id=$lam_info->lamchops_id&default_jobinfo_id=$job_info->jobinfo_id><font color=blue>
			$job_info->contract_num</font></a> - $job_info->job_name</div>";
			if ($job_info->work_recovery_job=='Y') {
				echo "<div style=\"float: right; background: $fd_color_3; border: 1px solid black;\"><b>WR</b></div>";
				}
			echo "</div>";
			*/
			}
		}
	
	if (("$d"=="")||($d>0)) {
		if ($jobopt) {
			$sublams=getoneb("select * from lam where parent_lam_id = '$lam_info->lam_id' and jobinfo_id = '$jobopt' order by lam_id limit 1");
			$sublam_list=@mysql_query("select * from lam right join contacts on (contacts.contacts_id = lam.employee_id) right join contacts_classes_sort_priority on (contacts.employee_class = contacts_classes_sort_priority.class_name) where parent_lam_id = '$lam_info->lam_id' and jobinfo_id = '$jobopt' order by employee_trade,class_sort_priority,alphasort");
			} else {
			$sublams=getoneb("select * from lam where parent_lam_id = '$lam_info->lam_id' order by lam_id limit 1");
			$sublam_list=@mysql_query("select * from lam right join jobinfo on (lam.jobinfo_id = jobinfo.jobinfo_id) right join contacts on (contacts.contacts_id = lam.employee_id) left join contacts_classes_sort_priority on (contacts.employee_class = contacts_classes_sort_priority.class_name) where parent_lam_id = '$lam_info->lam_id' order by length(contract_num),contract_num,employee_trade,class_sort_priority,alphasort");
			}
		if ("$d"!="") $options['depth']=$d - 1;
		while ($sl=@mysql_fetch_object($sublam_list)) {
			echo "<ul>";
			print_lam_div($sl->lam_id,$options);
			echo "</ul>";
			}
		} else {
		if ($sublams) echo "<ul><i><a href=javascript:refresh_lam('lam_$lam_info->lam_id')>more...</a></i></ul>";
		}
	echo "</div></div>";
	}

function print_trade_header($trade,$lamchops_id,$show_members='N') {
	global $pagename,$devel;
	$trade_link=urlencode($trade);
	if ($show_members=='N') {
		$onclick="ajaxManager('load_page','/global/front_desk$devel/reports/?mode=lam_labor_trade&trade=$trade_link&lamchops_id=$lamchops_id&show_members=Y','lam labor $trade')";
		} else {
		$onclick="ajaxManager('load_page','/global/front_desk$devel/reports/?mode=lam_labor_trade&trade=$trade_link&lamchops_id=$lamchops_id&show_members=N','lam labor $trade')";
		}
	echo "<div onclick=\"$onclick\" style=\"border: 1px solid black; text-align: center;\"><b>$trade</b></div>";
	}

function print_trade_class($trade,$class="") {
	$class_add=" and employee_class = '$class'";
	if ($class=="") $class_add=" and
			employee_class != 'General Foreman' and 
			employee_class != 'Foreman' and 
			employee_class != 'Journeyman' and 
			employee_class != 'Apprentice'";
	
		$tres=@mysql_query("select * from contacts where 
			umc_emp = 'Y' and 
			employee_group = '1' and 
			current = 'Y' and
			employee_trade = '$trade'
			$class_add
			order by alphasort");
	
	while ($trow=@mysql_fetch_object($tres)) {
		$lam_info=getoneb("select * from lam where 
			employee_id = '$trow->contacts_id' and 
			lamchops_id = '0' and 
			jobinfo_id = 0 and 
			parent_lam_id = 0 limit 1");
		if (!($lam_info)) {
@mysql_query("insert into lam set employee_id = '$trow->contacts_id',
lamchops_id = '0',jobinfo_id = 0,parent_lam_id = 0");
			$lam_info=getoneb("select * from lam where 
				employee_id = '$trow->contacts_id' and 
				lamchops_id = '0' and 
				jobinfo_id = 0 and 
				parent_lam_id = 0");
			}
		if (!($lam_info)) die("Error adding ghost records to lam database. Please contact your database administrator!");
		print_lam_div($lam_info->lam_id,array('copy'=>'Y','depth'=>'0','hide_notes'=>'Y'));
		}
	}

$trade_details=array(
		'carpenters'=>array(
			'letter'=>'C',
			'General Foreman'=>'#777777',
			'Foreman'=>'#aa9977',
			'Journeyman'=>'#ccaa99'
			),
		'fitter'=>array(
			'letter'=>'F',
			'General Foreman'=>'#777777',
			'Foreman'=>'#994444',
			'Journeyman'=>'#bb7777',
			'Apprentice'=>'#eeaaaa'
			),
		'iron workers'=>array(
			'letter'=>'I',
			'General Foreman'=>'#777777',
			'Foreman'=>'#993399',
			'Journeyman'=>'#bb66bb'
			),
		'plumber'=>array(
			'letter'=>'P',
			'General Foreman'=>'#777777',
			'Foreman'=>'#ffff00',
			'Journeyman'=>'#ffff66',
			'Apprentice'=>'#ffffbb'
			),
		'sheet metal workers'=>array(
			'letter'=>'S',
			'General Foreman'=>'#777777',
			'Foreman'=>'#6666ff',
			'Journeyman'=>'#9999ff',
			'Apprentice'=>'#bbbbff'
			),
		'plumbers & fitters'=>array(
			'letter'=>'F',
			'General Foreman'=>'#777777',
			'Foreman'=>'#994444',
			'Journeyman'=>'#bb7777',
			'Apprentice'=>'#eeaaaa'
			));
?>
