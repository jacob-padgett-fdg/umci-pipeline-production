<?
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

function verify_scope_completion($subcontracts_id) {
	$subcontracts_info=getoneb("select * from subcontracts where subcontracts_id = '$subcontracts_id'");
	$incomplete_data=getoneb("select * from subcontractor_scope_selections where subcontracts_id = '$subcontracts_info->subcontracts_id' and subcontractor_type_id = '$subcontracts_info->subcontractor_type_id' and in_scope != 'Yes' and in_scope != 'No' and in_scope != 'N/A' limit 1");
	if ($incomplete_data) {
		echo "<script>alert('Scope is not complete. Every item must be either Yes, No, or N/A!');</script>";
		return (FALSE);
		}
	return (TRUE);
	}

function check_register_subcontract_part($subcontracts_id,$part_template_id) {
	//tabledump("select * from subcontracts_parts where subcontracts_id = '$subcontracts_id' and part_template_id = '$part_template_id' limit 1");exit;
	$info=getoneb("select * from subcontracts_parts where subcontracts_id = '$subcontracts_id' and part_template_id = '$part_template_id' limit 1");
	if (!($info)) {
		register_subcontract_part($subcontracts_id,$part_template_id);
		$info=getoneb("select * from subcontracts_parts where subcontracts_id = '$subcontracts_id' and part_template_id = '$part_template_id' limit 1");
		}
	if (!($info)) die("Application error!");
	}

function check_unregister_subcontract_part($subcontracts_id,$part_template_id) {
	$info=getoneb("select * from subcontracts_parts where subcontracts_id = '$subcontracts_id' and part_template_id = '$part_template_id' limit 1");
	if ($info) { 
		echo "unregister";
		@mysql_query("delete from subcontracts_parts_pieces where parts_id = '$info->parts_id'"); 
		@mysql_query("delete from subcontracts_parts where parts_id = '$info->parts_id'");
		}
	return 0;
	}

function register_subcontract_part($subcontracts_id,$part_template_id) {
	$template_info=getoneb("select * from subcontracts_parts_template where part_template_id = '$part_template_id'");
	if (!($template_info)) die ("Application error: part template info.");
	@mysql_query("insert into subcontracts_parts set subcontracts_id = '$subcontracts_id', part_template_id = '$part_template_id', section_name = '$template_info->section_name', is_special = '$template_info->is_special'");
	$parts_id=@mysql_insert_id();
	$parts_qry="select * from subcontracts_parts_pieces_template where part_template_id = '$part_template_id'";
	$parts_res=@mysql_query($parts_qry);
	while ($part_template=@mysql_fetch_object($parts_res)) {
		@mysql_query("insert into  subcontracts_parts_pieces set 
		parts_id = '$parts_id',
		sort_priority = '$part_template->sort_priority',
		part_name = '$part_template->part_name',
		part_type = '$part_template->part_type',
		part_variable_input_text = '$part_template->part_variable_input_text',
		part_value = '$part_template->part_value'
		");
		}
	//tabledump("select * from subcontracts_parts_pieces_template where part_template_id = '$part_template_id'");
	
	}

function pdf_render_if_available($pdf,$subcontracts_id,$part_template_id) {
	$parts_res=@mysql_query("select * from subcontracts_parts where part_template_id = '$part_template_id' and subcontracts_id = '$subcontracts_id' order by sort_priority,parts_id,section_name");
	//tabledump("select * from subcontracts_parts where part_template_id = '$part_template_id' order by sort_priority,parts_id,section_name");exit;
	while ($part=@mysql_fetch_object($parts_res)) {
		$pdf=pdf_render_part($pdf,$part->parts_id);
		}
	return ($pdf);
	}

function replace_vars($variables,$text) {
	//print_r($variables);
	while ($varvalue=array_pop($variables)) {
		$varname=array_pop($variables);
		//echo "$varname/$varvalue";exit;
		$text=eregi_replace("UMCUMCUMC_${varname}_UMCUMCUMC","$varvalue",$text);
		//echo "<hr> $varname /$varvalue<hr>$text";
		}
	//die($text);
	return ($text);
	}


function pdf_render_part($pdf,$parts_id) {
	$part_info=getoneb("select * from subcontracts_parts where parts_id = '$parts_id'");
	$variables=array();
	$varres=@mysql_query("select * from subcontracts_parts_pieces where parts_id = '$parts_id' and part_type = 'variable'");
	while ($var=@mysql_fetch_object($varres)) {
		array_push($variables,$var->part_name);
		array_push($variables,$var->part_value);
		}
	
	$pieces_res=@mysql_query("select * from subcontracts_parts_pieces where parts_id = '$parts_id' and part_type != 'variable' order by sort_priority,piece_id,part_name");
	//echo "select * from subcontracts_parts_pieces where parts_id = '$parts_id' and part_type != 'variable' order by sort_priority,piece_id,part_name<hr>";
	//tabledump("select * from subcontracts_parts_pieces where parts_id = '$parts_id' and part_type != 'variable' order by sort_priority,piece_id,part_name");exit;
	while ($piece=@mysql_fetch_object($pieces_res)) {
		$tvars=$variables;
		$text=replace_vars($tvars,$piece->part_value);
		
		switch ($piece->part_type) {
			case "heading":
						$pdf->ezText("\r\n<b>$text</b> $pdf->ezPage",13,array("justification"=>"center"));
						break;;
					
			case "paragraph":
						$pdf->ezText("\r\n$text",10);
						break;;
						
			default: 	
						$pdf->ezText("Unknown type ($piece->part_type): $text",10);
						break;;
			}
		
		
		
		}
	return ($pdf);
	}


function show_variables_editors($subcontracts_id) {
	global $fd_color_6,$subcontracts_part_javascript;
	if ($subcontracts_part_javascript!=1) {
		echo "
		<script>
		function update_a_piece(piece_id,value) {
			newval=encodeURIComponent(value);
			ajaxManager('load_page','$pagename?mode=update_a_piece&piece_id=' + piece_id + '&value=' + newval,'cell_' + piece_id);
			}
		</script>
		
		
		
		
		
		
		
		
		";
		$subcontracts_part_javascript=1;
		}
	
	$res=@mysql_query("select * from subcontracts_parts where subcontracts_id = '$subcontracts_id'");
	$count=@mysql_num_rows($res);
	if ($count < 1) return 0;
	echo "<table border=0 cellspacing=0 cellpadding=4>";
	while ($row=@mysql_fetch_object($res)) {
		$vars_res=@mysql_query("select * from subcontracts_parts_pieces where part_type = 'variable' and parts_id = '$row->parts_id' order by sort_priority,piece_id,part_name");
		//tabledump("select * from subcontracts_parts_pieces where part_type = 'variable' and parts_id = '$row->parts_id' order by sort_priority,piece_id,part_name");
		$vars_count=@mysql_num_rows($vars_res);
		if ($vars_count>0) {
			echo "<tr><td colspan=2 align=center><font size=+1><b>$row->section_name</b></font></td></tr>";
			while ($var=@mysql_fetch_object($vars_res)) {
				if ($var->part_value=="") $bgcolor=$fd_color_6;
				else $bgcolor="#ffffff";
				
				echo "<tr><td>	
						$var->part_variable_input_text
					</td><td id=\"cell_$var->piece_id\">
						<input style=\"background: $bgcolor;\" type=text size=20 onchange=\"update_a_piece($var->piece_id,this.value)\" value=\"$var->part_value\">
					</td></tr>";
				}
			}
		}
	echo "</table>";
	}
	
	











?>
