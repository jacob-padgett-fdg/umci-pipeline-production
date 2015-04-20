<?

function pdf_timesheet_2 ($pdf,$timesheet_id,$watch_job="",$hide_others='false') {
	global $this_is_first_page;
	if ($this_is_first_page!="no") $this_is_first_page="no";
	else $pdf->ezNewPage();
	
	$timesheet_info=getone("select * from timesheet_index where timesheet_id = '$timesheet_id'");
	$date_info=date_info($timesheet_info->week_ending);
	// Turn on the headers at the top of the page for the timesheets
	$pdf=header_start($pdf,$timesheet_info);
	
	//////////////////////////////////////////
	// Reset some of the variables that we'll
	// use for tracking things.
	//////////////////////////////////////////
	$pdf->ts_sum_st=0;$pdf->ts_sum_ot=0;$pdf->ts_sum_dt=0;
	$pdf->ts_sum_v=0;$pdf->ts_sum_s=0;$pdf->ts_sum_h=0;
	$pdf->ts_sum_job_hours=0;
	$pdf->ts_sum_job_exps=0;$pdf->ts_sum_job_exps_printed=0;
	$pdf->ts_sum_job_trav=0;$pdf->ts_sum_job_trav_printed=0;
	$pdf->ts_sum_job_miles=0;$pdf->ts_sum_job_miles_printed=0;
	$pdf->ts_sum_job_park=0;$pdf->ts_sum_job_park_printed=0;
	$pdf->ts_sum_exps=0;$pdf->ts_sum_exps_printed=0;
	$pdf->ts_sum_subs=0;$pdf->ts_sum_subs_printed=0;
	$pdf->ts_sum_park=0;$pdf->ts_sum_park_printed=0;
	$pdf->ts_sum_trav=0;$pdf->ts_sum_trav_printed=0;
	$pdf->ts_sum_miles=0;$pdf->ts_sum_miles_printed=0;
	//$pdf->ts_sum_miles=0;$pdf->ts_sum_miles_printed=0;

	if (($timesheet_info->status!="posted")&&($timesheet_info->status!="processing")) {
		$pdf->setColor(1,.3,.3);
		$pdf->addText(220,400,50,"PROCESSING",315);
		$pdf->addText(300,405,50,"($timesheet_info->status)",315);
		$pdf->setColor(0,0,0);
		}
	$working_date=$date_info->week_start;
	while ("$working_date" != "$date_info->next") {
		$working_date_info=date_info($working_date);
		$pdf=pdf_timesheet_print_day2($pdf,$timesheet_info,$working_date_info,$watch_job,$hide_others);
		$working_date=$working_date_info->next;
		}
	$pdf->ezTable($data=array(array('day_of_week' => "Totals")),"","",array('shadeCol2'=>array(.9,.9,.9),'showHeadings' => 0,'shaded' => 2,'width' => ($pdf->ez['pageWidth'] - $pdf->ez['leftMargin']- $pdf->ez['rightMargin']),'fontSize'=>$pdf->ez['fontSize']));
	$total=getone("select sum(hours) as hours from timesheet_time where timesheet_id = '$row->timesheet_id'");
	$totals_row_hours=0;
	$totals_row_exps=0;
	$total=getone("select sum(hours) as hours from timesheet_time where timesheet_id = '$timesheet_info->timesheet_id'");
	$totals_row_hours=0;
	$totals_row_exps=0;
	if ($watch_job!="") {
		$wj_print="(Job " . ereg_replace('- *','',$watch_job) . ")";
		
		//$total_hours_text[$totals_row_hours]['col1']="Selected Job:";
		//$total_hours_text[$totals_row_hours]['col2']="$wj_print";
		//$totals_row_hours++; 
		}
	if ("$pdf->ts_sum_job_hours"!=0) {
		$total_hours_text[$totals_row_hours]['col1']="Hours $wj_print:";
		$total_hours_text[$totals_row_hours]['col2']=$pdf->ts_sum_job_hours;
		$totals_row_hours++; }
	if ("$pdf->ts_sum_st"!=0) {
		$total_hours_text[$totals_row_hours]['col1']="Straight Time Hours:";
		$total_hours_text[$totals_row_hours]['col2']=$pdf->ts_sum_st;
		$totals_row_hours++; }
	if ("$pdf->ts_sum_ot"!=0) {
		$total_hours_text[$totals_row_hours]['col1']="Over Time Hours:";
		$total_hours_text[$totals_row_hours]['col2']=$pdf->ts_sum_ot;
		$totals_row_hours++; }
	if ("$pdf->ts_sum_dt"!=0) {
		$total_hours_text[$totals_row_hours]['col1']="Double Time Hours:";
		$total_hours_text[$totals_row_hours]['col2']=$pdf->ts_sum_dt;
		$totals_row_hours++; }
	if ("$pdf->ts_sum_v"!=0) {
		$total_hours_text[$totals_row_hours]['col1']="Vacation Hours:";
		$total_hours_text[$totals_row_hours]['col2']=$pdf->ts_sum_v;
		$totals_row_hours++; }
	if ("$pdf->ts_sum_s"!=0) {
		$total_hours_text[$totals_row_hours]['col1']="Sick Hours:";
		$total_hours_text[$totals_row_hours]['col2']=$pdf->ts_sum_s;
		$totals_row_hours++; }
	if ("$pdf->ts_sum_h"!=0) {
		$total_hours_text[$totals_row_hours]['col1']="Holiday Hours:";
		$total_hours_text[$totals_row_hours]['col2']=$pdf->ts_sum_h;
		$totals_row_hours++; }
	if ($total->hours!=0) {
		$total_hours_text[$totals_row_hours]['col1']="Total Hours:";
		$total_hours_text[$totals_row_hours]['col2']=$total->hours;
		$totals_row_hours++; }
	if ("$pdf->ts_sum_job_exps"!=0) {
		$total_hours_text[$totals_row_exps]['col3']="Expenses $wj_print:";
		$total_hours_text[$totals_row_exps]['col4']='$' . sprintf("%01.2f",$pdf->ts_sum_job_exps);
		$totals_row_exps++; }
	if ("$pdf->ts_sum_job_park"!=0) {
		$total_hours_text[$totals_row_exps]['col3']="Parking $wj_print:";
		$total_hours_text[$totals_row_exps]['col4']='$' . sprintf("%01.2f",$pdf->ts_sum_job_park);
		$totals_row_exps++; }	
	if ("$pdf->ts_sum_job_trav"!=0) {
		$total_hours_text[$totals_row_exps]['col3']="Travel $wj_print:";
		$total_hours_text[$totals_row_exps]['col4']='$' . sprintf("%01.2f",$pdf->ts_sum_job_trav);
		$totals_row_exps++; }
	if ("$pdf->ts_sum_job_miles"!=0) {
		$total_hours_text[$totals_row_exps]['col3']="Mileage $wj_print:";
		//die(sprintf("%01.2f",$pdf->ts_sum_job_miles));
		$total_hours_text[$totals_row_exps]['col4']='$' . sprintf("%01.2f",$pdf->ts_sum_job_miles);
		$totals_row_exps++; }
	$print_all_spt=FALSE;
	if ($watch_job=="") $print_all_spt=TRUE;
	if (!($hide_others=='true')) $print_all_spt=TRUE;
	
	//if (!($hide_others)) {
	if ($print_all_spt) {
	if ("$pdf->ts_sum_subs"!=0) {
		$total_hours_text[$totals_row_exps]['col3']="Subsistence:";
		$total_hours_text[$totals_row_exps]['col4']='$' . sprintf("%01.2f",$pdf->ts_sum_subs);
		$totals_row_exps++; }
	if ("$pdf->ts_sum_park"!=0) {
		$total_hours_text[$totals_row_exps]['col3']="Parking:";
		$total_hours_text[$totals_row_exps]['col4']='$' . sprintf("%01.2f",$pdf->ts_sum_park);
		$totals_row_exps++; }
	if ("$pdf->ts_sum_trav"!=0) {
		$total_hours_text[$totals_row_exps]['col3']="Travel:";
		$total_hours_text[$totals_row_exps]['col4']='$' . sprintf("%01.2f",$pdf->ts_sum_trav);
		$totals_row_exps++; }
	if ("$pdf->ts_sum_miles"!=0) {
		$total_hours_text[$totals_row_exps]['col3']="Mileage:";
		$total_hours_text[$totals_row_exps]['col4']='$' . sprintf("%01.2f",$pdf->ts_sum_miles);
		$totals_row_exps++; }
	if ("$pdf->ts_sum_exps"!=0) {
		$total_hours_text[$totals_row_exps]['col3']="Total Expenses:";
		$total_hours_text[$totals_row_exps]['col4']='$' . sprintf("%01.2f",$pdf->ts_sum_exps);
		$totals_row_exps++; }
	}
	if ($total_row_exps==0) {
		$total_hours_text[$totals_row_exps]['col3']=" ";
		$total_hours_text[$totals_row_exps]['col4']=" "; }		
	if ($total_row_hours==0) {
		$total_hours_text[$totals_row_hours]['col1']=" ";
		$total_hours_text[$totals_row_hours]['col2']=" "; }		
	$pdf->ezTable($total_hours_text,"","",array('showLines'=>'0','shaded'=>'0','showHeadings'=>'0','width'=>($pdf->ez['pageWidth'] - $pdf->ez['leftMargin'] - $pdf->ez['rightMargin']),'rowGap'=>0,'fontSize'=>9,'cols'=>array('col1'=>array('justification'=>'right',width=>'110'),'col2'=>array('justification'=>'left',width=>'200'),'col3'=>array('justification'=>'right'),'col4'=>array('justification'=>'left',width=>'50'))));
	
	//if (($watch_job=="")&&($timesheet_info->status!="posted")&&(($timesheet_info->time_is_accurate!='Y')||($timesheet_info->no_unreported_injuries!='Y'))) {
	if (($watch_job=="")&&(($timesheet_info->time_is_accurate!='Y')||($timesheet_info->no_unreported_injuries!='Y'))) {
		$employee_info=getoneb("select * from contacts where contacts_id = '$timesheet_info->employee_id'");
		$pdf->ezText("\nI certify that the above hours were worked by me on the noted project(s). I further certify that I have reported to my supervisor or a UMC safety professional any safety violations that I witnessed or was otherwise aware of; and, that I have truthfuly reported any work related injury I may have had through the date of this time card.",10);
		$pdf->ezText("\n                                                                                                           Signature: _____________________________",10);
		$pdf->ezText("                                                                                                                              ($employee_info->first_name $employee_info->last_name)",10);
		}

	
	if (($timesheet_info->status!="posted")&&($timesheet_info->status!="processing")) {
		$pdf->setColor(1,.3,.3);
		$pdf->addText(220,400,50,'PROCESSING',315);
		$pdf->addText(300,405,50,"($timesheet_info->status)",315);
		$pdf->setColor(0,0,0);
		}
	
	
	$pdf=header_stop($pdf);
	return($pdf);
	}

function header_stop ($pdf) {
	$pdf->stopObject($pdf->header_timesheet_object_id);
	$pdf->stopObject($pdf->header_letterhead_object_id);
	return($pdf);
	}

function header_start($pdf,$timesheet_info) {
	////////////// Info //////////////////
	$pdf->ezSetMargins(108,36,36,36);
	$dates_info=date_info($timesheet_info->week_ending);
	$contact_info=getone("select * from contacts where contacts_id = '$timesheet_info->employee_id'");
	////////////// The actual header object defnition //////////////
	$header_height=($pdf->ez['pageHeight'] - ($height=40) - 36);
	if ($pdf->header_letterhead_defined!="1") {
		$pdf->header_letterhead_object_id=$pdf->openObject();
        // GPH MARK - absolute path
		$pdf->addPngFromFile("/data/web/pipeline/php/global/timesheet/umclogo_wbv2.png",36,$header_height,120,$height);
		$pdf->setLineStyle(1,'butt');
		$pdf->line($pdf->ez['leftMargin'],$header_height - 5,$pdf->ez['pageWidth'] - $pdf->ez['rightMargin'],$header_height - 5);
		$pdf->closeObject();
		$pdf->header_letterhead_defined="1";
		}
	
	$pdf->header_timesheet_object_id=$pdf->openObject();
	$pdf->ezSetY($header_height + ($fontsize=17) + 5);
	$pdf->ezText("Timesheet for $contact_info->first_name $contact_info->last_name ($contact_info->employee_num)",$fontsize,array('justification'=>'right'));

	$pdf->ezSetY($header_height - 5);
	$pdf->ezText("$dates_info->week_start_slashes - $dates_info->week_end_slashes",14,array('justification'=>'center'));
	$pdf->closeObject();
	
	////////////////// Puts the object on all pages until the object stop call
	$pdf->addObject($pdf->header_letterhead_object_id,"all");
	$pdf->addObject($pdf->header_timesheet_object_id,"all");
	return $pdf;
	}

function pdf_timesheet_print_day2($pdf,$timesheet_info,$date_info,$watch_job="",$hide_others='false') {
	$query="select * from timesheet_time left join contacts on (creator = contacts_id) where timesheet_time.employee_id = '$timesheet_info->employee_id' and timesheet_time.workday = '$date_info->date_mysql'";
	$res=@mysql_query($query);
	if (!($res)) die("Application Error: Could not query properly for time! Please contact your system administrator!<p>Failed query:<hr>$query");
	$rowcount=@mysql_num_rows($res);
	if ($rowcount<1) return ($pdf);
	///////////////////////////////////////////////////////////////////
	// Draw the "day of week" box and shade it...
	///////////////////////////////////////////////////////////////////
	$current_y=$pdf->y;
	$pdf->ez['fontSize']=13;
	
	$pdf->ezTable($data=array(array('day_of_week' => "$date_info->day_of_week_long")),"","",array('shadeCol2'=>array(.9,.9,.9),'showHeadings' => 0,'shaded' => 2,'width' => ($pdf->ez['pageWidth'] - $pdf->ez['leftMargin']- $pdf->ez['rightMargin']),'fontSize'=>$pdf->ez['fontSize']));
	
	$pdf->ezSetY($current_y);
	$pdf->ezTable($data=array(array('day_of_week' => "$date_info->date_slashes")),"","",array('shadeCol2'=>array(.9,.9,.9),'showHeadings' => 0,'shaded' => 0,'width' => ($pdf->ez['pageWidth'] - $pdf->ez['leftMargin']- $pdf->ez['rightMargin']),'fontSize'=>$pdf->ez['fontSize'],'cols'=>array('day_of_week'=>array('justification'=>'right'))));
	$pdf->ezSetDy(-1);
	
	$pdf->ezTableDumpStart();
	$pdf->ezTableDumpAdd("<b>Job #","Phase","Description","Entered By","Shift","</b>");

	while($row=@mysql_fetch_object($res)) {
	////////////////////////////////////////////////////	
	// Figure out if we need to watch for a job, and
	// do the math if we do/need to.
	////////////////////////////////////////////////////	
	$watch_length=strlen($watch_job);
	$highlight="no";
	if ($watch_length>0) {
		$row->phase_desc="";
		if (!(strncmp($row->job_num,$watch_job,$watch_length))) {
			$pdf->ts_sum_job_hours=$pdf->ts_sum_job_hours + $row->hours;
			$pdf->ts_sum_job_exps=$pdf->ts_sum_job_exps + $row->dollars;
			$highlight="yes";
		}
	}
	
	////////////////////////////////////////////////////
	
	$ectype="ERROR!";
	switch ($row->earnings_code) {
	case 1:
		$ectype="ST";$pdf->ts_sum_st=$pdf->ts_sum_st + $row->hours;break;
	case 2:
		$ectype="OT";$pdf->ts_sum_ot=$pdf->ts_sum_ot + $row->hours;break;
	case 3:
		$ectype="DT";$pdf->ts_sum_dt=$pdf->ts_sum_dt + $row->hours;break;
	case 8:
		$ectype="V ";$pdf->ts_sum_v=$pdf->ts_sum_v + $row->hours;break;
	case 9:
		$ectype="S ";$pdf->ts_sum_s=$pdf->ts_sum_s + $row->hours;break;
	case 10:
		$ectype="H ";$pdf->ts_sum_h=$pdf->ts_sum_h + $row->hours;break;
	case 5:
		$row->phase_desc="Subsistence";
		$ectype="$";$row->hours=$row->dollars;
		$pdf->ts_sum_subs=$pdf->ts_sum_subs + $row->dollars;
		$pdf->ts_sum_exps=$pdf->ts_sum_exps + $row->dollars;
		break;
	case 6:
		$row->phase_desc="Parking";
		$ectype="$";$row->hours=$row->dollars;
		$pdf->ts_sum_park=$pdf->ts_sum_park + $row->dollars;
		$pdf->ts_sum_exps=$pdf->ts_sum_exps + $row->dollars;
		if ($watch_length>0) {
			if (!(strncmp($row->job_num,$watch_job,$watch_length))) {
				$pdf->ts_sum_job_park=$pdf->ts_sum_job_park + $row->dollars;
				}
			}
		
		break;
	case 7:
		$row->phase_desc="Travel";
		$ectype="$";$row->hours=$row->dollars;
		if (ereg("^[0-9][0-9.]* miles -",$row->comments)) {
			if ($watch_length>0) {
				if (!(strncmp($row->job_num,$watch_job,$watch_length))) {
					$pdf->ts_sum_job_miles=$pdf->ts_sum_job_miles + $row->dollars;
					}
				}
			$pdf->ts_sum_miles=$pdf->ts_sum_miles + $row->dollars;
			$row->phase_desc="Mileage";
			} else {
			if ($watch_length>0) {
				if (!(strncmp($row->job_num,$watch_job,$watch_length))) {
					$pdf->ts_sum_job_trav=$pdf->ts_sum_job_trav + $row->dollars;
					}
				}
			$row->phase_desc="Travel";
			$pdf->ts_sum_trav=$pdf->ts_sum_trav + $row->dollars;
			}
		$pdf->ts_sum_exps=$pdf->ts_sum_exps + $row->dollars;
		break;
	}
	// Set the font for the line entries
	if ($highlight=="yes") {
		$hs="<b>";$he="</b>";
		} else {
		$hs="";$he="";
		}
	if (($highlight!='yes')&&($hide_others=='true')) continue;
	$row->job_num=ereg_replace("-$","",$row->job_num);
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	// Debug line added - remove before publishing!!!!!!!!!!!!!
	//$watch_length=0;
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	if ($watch_length>0) {
		$description="$row->job_desc";
		} else {
		$description="$row->job_desc - " . strtolower($row->phase_desc);
		}
	$pdf->eztableDumpAdd("$hs$row->job_num","$row->phase","$description",$row->login_name,$row->shift,"$ectype $row->hours$he");
	}	
	$pdf->ezTableDumpPrint(0,array('justification'=>'center','fontSize'=>'8','showLines'=>'0','shaded'=>'0','width'=>($pdf->ez['pageWidth'] - $pdf->ez['leftMargin'] - $pdf->ez['rightMargin']),'colGap'=>5,'rowGap'=>0,'cols'=>array ('0'=>array('width'=>'49'),'1'=>array('width'=>'49'),'3'=>array('width'=>'65'),'4'=>array('width'=>'28','justification'=>'center'),'5'=>array('width'=>'45','justification'=>'right'))));
	return ($pdf);
	}

function pdf_cover_page($pdf,$text) {
	global $this_is_first_page;
	if ($this_is_first_page!="no") $this_is_first_page="no";
	else $pdf->ezNewPage();
	$pdf->ezSetY($pdf->ez['pageHeight'] / 2);
	$pdf->ezText("$text",25,array('justification'=>'center'));
	return ($pdf);
	}
?>
