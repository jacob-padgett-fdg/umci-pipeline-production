<?
$appsections['vitals']="Contact Information";
$appsections['employment_history']="Employment History";
$appsections['education']="Education";
$appsections['job_references']="Job References";
$appsections['eligibility']="Notices/Consent";
$appsections['openings']="Openings";
$appsections['main']="Back to Listings";

$sections['requirements']="Job Skills/Requirements";
$sections['responsibilites']="Essential Functions/Major Responsibilites";
$sections['education']="Education and/or Experience";
$sections['questions']="Application Questions";


function application_pdf($pdf,$application_id) {
	$app_info=getone("select * from hr_applications where application_id = '$application_id'");
	$pdf->ezText("<b>Confidential Application</b>",24,array('justification'=>'center'));
	$pdf->ezHr();
	$pdf->ezText("<b>Applicant Name:</b> $app_info->last_name, $app_info->first_name $app_info->middle_name","14");
	$pdf->ezText("<b>Application status:</b> $app_info->status","10");
	$pdf->ezText("<b>Day Phone:</b> $app_info->day_phone","10");
	$pdf->ezText("<b>Home Phone:</b> $app_info->home_phone","10");
	$pdf->ezText("<b>Address:</b> $app_info->address\r\n                 $app_info->address_city, $app_info->address_state $app_info->address_zip","10");
	$pdf->ezText("<b>E-mail:</b> $app_info->email","10");
	$pdf->ezText("<b>Date Available:</b> $app_info->date_available","10");
	$pdf->ezText("<b>Expected Salary:</b> $app_info->expected_salary per $app_info->expected_salary_period","10");
	$pdf->ezText("<b>Availability:</b> $app_info->hours_available","10");
	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// Employment History
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	$pdf->ezText("\r\nEmployment History",18,array('justification'=>'center'));
	$pdf->ezHr();
	$res=@mysql_query("select * from hr_applications_employment_history where application_id = '$app_info->application_id' order by employment_history_id");
	while ($row=@mysql_fetch_object($res)) {
		//$row->responsibilities=quotemeta($row->responsibilities);
		$pdf->ezText("<b>$row->employer</b>",10);
		$pdf->ezSetDy(10);
		$pdf->ezText("$row->job_title",10,array('justification'=>'right'));
		
		$pdf->ezText("$row->employer_address",10);
		$pdf->ezSetDy(10);
		$pdf->ezText("$row->there_from - $row->there_to",10,array('justification'=>'right'));
		
		$pdf->ezText("<b>Supervisor:</b> $row->supervisor_name / $row->supervisor_phone",10);
		$pdf->ezText("<b>Salary:</b> $row->salary per $row->salary_period",10);
		$pdf->ezText("<b>Responsibilities:</b> $row->responsibilities",10);
		$pdf->ezText("<b>Reason for Leaving:</b> $row->left_because\r\n",10);
		}
	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// Education
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	$pdf->ezText("\r\nEducation",18,array('justification'=>'center'));
	$pdf->ezHr();
	$res=@mysql_query("select * from hr_applications_education_history where application_id = '$app_info->application_id' order by education_history_id");
	while ($row=@mysql_fetch_object($res)) {
		$pdf->ezText("<b>$row->school_name</b>",10);
		$pdf->ezSetDy(10);
		$pdf->ezText("$row->school_type",10,array('justification'=>'right'));
		
		$pdf->ezText("$row->location",10);
		$pdf->ezText("<b>Major:</b> $row->major",10,array('justification'=>'right'));
		$pdf->ezText("<b>Diploma:</b> $row->diploma",10,array('justification'=>'right'));
		}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// Job References
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	$pdf->ezText("\r\nJob References",18,array('justification'=>'center'));
	$pdf->ezHr();
	$res=@mysql_query("select * from hr_applications_references_history where application_id = '$app_info->application_id' order by references_history_id");
	while ($row=@mysql_fetch_object($res)) {
		$pdf->ezText("<b>$row->name</b>",10);
		$pdf->ezSetDy(10);
		$pdf->ezText("$row->phone",10,array('justification'=>'right'));
		
		$pdf->ezText("<b>Company/Title:</b> $row->company_title",10);
		$pdf->ezText("<b>Years Known:</b> $row->years_known\r\n",10);
		}
	return ($pdf);
	}


function bullets($hr_openings_id,$section="",$view_only=0) {
	global $sections;
	if ($section!="") $sections_to_do=array("0"=>"$section");
	else $sections_to_do=array_keys($sections);

	//print_r($sections_to_do);

	$total=sizeof($sections_to_do);
	$count=0;
	while($count < $total) {
		if (($view_only!=0)&&($sections_to_do[$count]=='questions')) {
			$count++;
			continue;
			}
		$the_section=$sections_to_do[$count];
		//echo "$the_section<hr>";
		$section_title=$sections[$the_section];

		$query="select * from hr_opening_bullets where bullet_category = '$the_section' and hr_openings_id = '$hr_openings_id' order by sort_priority,hr_opening_bullets_id";
		$res=@mysql_query($query);
		$section_count=@mysql_num_rows($res);
		if (($section_count==0)&&($view_only!=0)) { $count++; continue; }
		
		echo "<font size=+1><b>$section_title:&nbsp;</b></font>";
		if ($view_only==0) echo "<i><a href=$pagename?mode=bullet_edit&bullet_category=$the_section&hr_openings_id=$hr_openings_id><font color=blue>(Add)</font></a></i>";
		echo "<br>";
	
		
		while ($row=@mysql_fetch_object($res)) {
			echo "
				<ul><li>
					$row->bullet_text
				";
				if ($view_only==0) echo "<a href=$pagename?mode=bullet_edit&hr_opening_bullets_id=$row->hr_opening_bullets_id><font color=blue><i>...Modify entry.</i></font></a>";
				echo "</li></ul>";
			}
		echo "<p>";
		$count++;
		}
	
	}


function list_openings($query) {
	$res=@mysql_query($query);
	$count=@mysql_num_rows($res);
	if ($count == "0") {
		echo "<b>No Results Found...</b><br>";
		return 0;
		}
	echo "<table border=0 cellspacing=5 cellpadding=5>";
	
	while ($row=@mysql_fetch_object($res)) {
		$row->deadline=invali_date($row->deadline);
		
		$completed_count=getoneb("select sum(1) as total 
		from hr_applications,hr_application_submissions 
		where hr_applications.application_id = 
		hr_application_submissions.application_id 
		and hr_openings_id = '$row->hr_openings_id' 
		and status = 'complete'");
		$total_count=getoneb("select sum(1) as total from hr_applications,hr_application_submissions where hr_applications.application_id = hr_application_submissions.application_id and hr_openings_id = '$row->hr_openings_id'");
		
		echo "<tr><td>
				<a href=$pagename?mode=opening_edit&hr_openings_id=$row->hr_openings_id><font color=blue>$row->job_title</font></a>
			</td><td>
				$row->deadline
			</td><td>
				<a href=$pagename?mode=opening_view&hr_openings_id=$row->hr_openings_id><font color=blue><i>View</i></font></a>
			</td><td>
				<a href=$pagename?mode=application_download_complete&hr_openings_id=$row->hr_openings_id><font color=blue>
				$completed_count->total 
				</font></a>
				/ 
				<a href=$pagename?mode=application_download_all&hr_openings_id=$row->hr_openings_id><font color=blue>
				$total_count->total</font></a> apps completed
			</td></tr>
			";
				//<a href=$pagename?mode=apply&hr_openings_id=$row->hr_openings_id><font color=blue><i>Apply</i></font></a>
		}
	
	echo "</table>";
	}

function list_openings_public($query) {
	$res=@mysql_query($query);
	$count=@mysql_num_rows($res);
	if ($count == "0") {
		echo "<b>No Results Found...</b><br>";
		return 0;
		}
	echo "<table border=0 cellspacing=5 cellpadding=5 height=200>";
	
	while ($row=@mysql_fetch_object($res)) {
		$row->deadline=invali_date($row->deadline);
		echo "<tr height=20><td>
				<a href=$pagename?mode=opening_view&hr_openings_id=$row->hr_openings_id><font color=black size=-1><i>$row->job_title</i></font></a>
			</td><td>
				<a href=$pagename?mode=apply&hr_openings_id=$row->hr_openings_id><img src=\"images_umc/apply.gif\" border=0></a>
			</td></tr>
			";
		}
	
	echo "<tr><td>&nbsp;</td></tr></table>";
	}
?>
