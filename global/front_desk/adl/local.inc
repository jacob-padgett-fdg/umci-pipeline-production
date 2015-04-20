<?

function extension_end_date($mssqldate,$plusdays) {
	$mssqldate=addslashes($mssqldate);
	$plusdays=addslashes($plusdays);
	$tstamp=strtotime($mssqldate);
	$expire_date=date('Y-m-d',strtotime("+$plusdays days",$tstamp));
	//echo "$mssqldate/$expire_date<hr>";
	return ($expire_date);
	}


function get_approval_data($employee_num,$today) {
	global 	$driver_safety_term,$driver_safety_extension_length,$driver_safety_new_hire_extension,
			$driver_abstract_term, $driver_abstract_extension_length,
			$drug_screen_term, $drug_screen_extension_length;
			
	$approval_info->employee_num=addslashes($employee_num);
	///////////////////////////////////////////////////////////////////////////////////////////
	// Load up all the employee's HR information..
	///////////////////////////////////////////////////////////////////////////////////////////
	$hr_info=@ms_getoneb("select * from HRRM with (NOLOCK) where HRRef = '$employee_num' and HRCo = 1");
	if ($hr_info->udAPPROVALSTATUS!='') { echo "<pre>";print_r($hr_info);die("</pre>"); }
	//if (!($hr_info)) die("Application error! Unable to load the employee information correctly!");
	//if ($hr_info->HRRef=="6817") { echo "</table><pre>";print_r($hr_info);die("</pre>"); }
	
	///////////////////////////////////////////////////////////////////////////////////////////
	// Figure out all the important dates...
	///////////////////////////////////////////////////////////////////////////////////////////
	$today_real_ts=strtotime($today);
	$today_real=date('m/d/Y',$today_real_ts);
	
	//////////////////////////////////
	// Driver safety dates
	$driver_safety_term_cutoff=date('m/d/Y',strtotime("$driver_safety_term days ago",$today_real_ts));
	$driver_safety_extension=$driver_safety_term + $driver_safety_extension_length;
	$driver_safety_extension_cutoff=date('m/d/Y',strtotime("$driver_safety_extension days ago",$today_real_ts));
	$driver_safety_new_hire_extension_cutoff=date('m/d/Y',strtotime("$driver_safety_new_hire_extension days ago",$today_real_ts));
	//////////////////////////////////
	// Driver abstract dates
	$driver_abstract_term_cutoff=date('m/d/Y',strtotime("$driver_abstract_term days ago",$today_real_ts));
	$driver_abstract_extension=$driver_abstract_term + $driver_abstract_extension_length;
	$driver_abstract_extension_cutoff=date('m/d/Y',strtotime("$driver_abstract_extension days ago",$today_real_ts));
	//////////////////////////////////
	// Drug test dates
	$drug_screen_term_cutoff=date('m/d/Y',strtotime("$drug_screen_term days ago",$today_real_ts));
	$drug_screen_extension=$drug_screen_term + $drug_screen_extension_length;
	$drug_screen_extension_cutoff=date('m/d/Y',strtotime("$drug_screen_extension days ago",$today_real_ts));
	

	///////////////////////////////////////
	// Check to see if they have a truck..
	///////////////////////////////////////

	$truck_info=ms_getoneb("select top 1 * from EMEM with (NOLOCK) where Status = 'A' and EMCo = 1 and Operator = '$approval_info->employee_num'");
	$approval_info->truck_number=$truck_info->Equipment;
	
	
	///////////////////////////////////////
	// Check the drivers safety class first
	///////////////////////////////////////
	$approval_info->driver_safety_status='ERROR';
	$approval_info->driver_safety_extension='N';
	$approval_info->driver_safety_extension_description='';

	$driver_safety_term_ok=ms_getoneb("select top 1 * from HRET with (NOLOCK) where HRCo = 1 and HRRef = '$approval_info->employee_num' and TrainCode = 'Driving' and Date > '$driver_safety_term_cutoff'");
	if ($driver_safety_term_ok) {
		$approval_info->driver_safety_status='Y';
		} else {
		$driver_safety_extension_ok=ms_getoneb("select top 1 * from HRET with (NOLOCK) where HRRef = '$approval_info->employee_num' and TrainCode = 'Driving' and Date > '$driver_safety_extension_cutoff'");
		if ($driver_safety_extension_ok) {
			$approval_info->driver_safety_status="N";
			$approval_info->driver_safety_extension='Y';
			$approval_info->driver_safety_extension_description='Class expired';
			$approval_info->driver_safety_extension_date=extension_end_date($driver_safety_extension_ok->Date,$driver_safety_extension);
			} else {
			///////////////////////////////////////
			// Now I should check new hire status
			///////////////////////////////////////
			$hire_date_res=ms_getoneb("select top 1 * from HRRM with (NOLOCK) where HRCo = 1 and PREmp = '$approval_info->employee_num' and HireDate > '$driver_safety_new_hire_extension_cutoff'");
			if ($hire_date_res) {
				$approval_info->driver_safety_status="N";
				$approval_info->driver_safety_extension='Y';
				$approval_info->driver_safety_extension_date=extension_end_date($hire_date_res->HireDate,$driver_safety_new_hire_extension);
				$approval_info->driver_safety_extension_description='Class expired';
				} else {
				$approval_info->driver_safety_status="N";
				}
			}
		}
	///////////////////////////////////////
	// Next check the drivers abstract
	///////////////////////////////////////
	$approval_info->driver_abstract_status='ERROR';
	$approval_info->driver_abstract_extension='N';
	$approval_info->driver_abstract_extension_description='';

	$driver_abstract_term_ok=ms_getoneb("select top 1 * from HRRM with (NOLOCK) where HRCo = 1 and PREmp = '$approval_info->employee_num' and udDriverAbstractPrintedDate > '$driver_abstract_term_cutoff'");
	if ($driver_abstract_term_ok) {
		$approval_info->driver_abstract_status='Y';
		////////////////////////////////////
		/////////hrrm stuff here..
		////////////////////////////////////
		//$approval_info->driver_abstract_pending=FALSE;
		} else {
		$approval_info->driver_abstract_status='N';
		
		//$driver_abstract_extension_cutoff=1;
		$driver_abstract_extension_ok=ms_getoneb("select top 1 * from HRRM with (NOLOCK) where HRCo = 1 and PREmp = '$approval_info->employee_num' and udDriverAbstractPrintedDate > '$driver_abstract_extension_cutoff'");
		//echo "<hr>select top 1 * from HRRM where HRCo = 1 and PREmp = '$approval_info->employee_num' and udDriverAbstractPrintedDate > '$driver_abstract_extension_cutoff'<hr>";
		//ms_tabledump("select top 1 * from HRRM where HRCo = 1 and PREmp = '$approval_info->employee_num' and udDriverAbstractPrintedDate > '$driver_abstract_extension_cutoff'");
		if ($driver_abstract_extension_ok) {
			$approval_info->driver_abstract_extension='Y';
			$approval_info->driver_abstract_extension_description='Report Expired';
			$approval_info->driver_abstract_extension_date=extension_end_date($driver_abstract_extension_ok->udDriverAbstractPrintedDate,$driver_abstract_extension);
			}
		
		//die("shit");
		}
	if ($hr_info->udDRIVERAPPROVALSTATUS=='U') $approval_info->driver_abstract_status='N';
		if ($hr_info->udDRIVERAPPROVALSTATUS=='P') $approval_info->driver_abstract_pending=TRUE;
		$approval_info->driver_abstract_as=$hr_info->udDRIVERAPPROVALSTATUS;

	///////////////////////////////////////
	// Next check the drug tests
	///////////////////////////////////////
	$approval_info->drug_screen_status='ERROR';
	$approval_info->drug_screen_extension='N';
	$approval_info->drug_screen_extension_description='';

	//$drug_screen_term_ok=ms_getoneb("select top 1 * from HRRM where HRCo = 1 and PREmp = '$approval_info->employee_num' and udDriverAbstractPrintedDate > '$driver_abstract_term_cutoff'");
	$drug_screen_term_ok=ms_getoneb("select top 1 * from HRDT with (NOLOCK) where HRCo = 1 and HRRef = '$approval_info->employee_num' and Date > '$drug_screen_term_cutoff'");
	
	
	if ($drug_screen_term_ok) {
		$approval_info->drug_screen_status='Y';
		} else {
		$approval_info->drug_screen_status='N';
		
		$drug_screen_extension_ok=ms_getoneb("select top 1 * from HRDT with (NOLOCK) where HRCo = 1 and HRRef = '$approval_info->employee_num' and Date > '$drug_screen_extension_cutoff'");
		//ms_tabledump("select top 1 * from HRDT where HRCo = 1 and HRRef = '$approval_info->employee_num' and Date > '$drug_screen_extension_cutoff'");
		if ($drug_screen_extension_ok) {
			$approval_info->drug_screen_extension='Y';
			$approval_info->drug_screen_extension_description='Drug Test Expired';
			$approval_info->drug_screen_extension_date=extension_end_date($drug_screen_extension_ok->Date,$drug_screen_extension);
			}
		}
	///////////////////////////////////////
	// Next check the handbook status
	///////////////////////////////////////
	$approval_info->uddatehandbooksigned=date('Y-m-d',strtotime($hr_info->uddatehandbooksigned));
//	$approval_info->uddatehandbooksigned=$hr_info->uddatehandbooksigned;
	if ($approval_info->uddatehandbooksigned<"2000-01-01") {
		$approval_info->uddatehandbooksigned="";
		$approval_info->handbook_sig="N";
		} else {
		$approval_info->handbook_sig="Y";
		}

	///////////////////////////////////////
	// Next check the driving policy sig status
	///////////////////////////////////////
	$approval_info->uddatedrivingpolicysigned=date('Y-m-d',strtotime($hr_info->uddatedrivingpolicysigned));
	if ($approval_info->uddatedrivingpolicysigned<"2000-01-01") {
		$approval_info->uddatedrivingpolicysigned="";
		$approval_info->driving_policy_sig="N";
		} else {
		$approval_info->driving_policy_sig="Y";
		}




	
	
	
	
	/////////////////////////////////////////
	/////////////////////////////////////////
	// Finish things up.. 
	/////////////////////////////////////////
	/////////////////////////////////////////
	
	
	$approval_info->approval_status='Y';
	$approval_info->extension_ok='Y';
	
	if ($approval_info->driver_safety_status!='Y') $approval_info->approval_status='N';
	if ($approval_info->driver_abstract_status!='Y') $approval_info->approval_status='N';
	if ($approval_info->drug_screen_status!='Y') $approval_info->approval_status='N';
	if ($approval_info->handbook_sig!='Y') {
		$approval_info->approval_status='N';
		$approval_info->extension_ok='N';
		}

	if ($approval_info->driving_policy_sig!='Y') {
		$approval_info->approval_status='N';
		$approval_info->extension_ok='N';
		}
	
	
	if ($approval_info->approval_status!='Y') {
		$approval_info->extension_end_date="9999/99/99";
		
		if ($approval_info->driver_safety_status!='Y') {
			if ($approval_info->driver_safety_extension=="Y") {
				if ($approval_info->driver_safety_extension_date < $approval_info->extension_end_date)
					$approval_info->extension_end_date=$approval_info->driver_safety_extension_date;
				} else {
				$approval_info->extension_ok='N';
				}
			}
		if ($approval_info->driver_abstract_status!='Y') {
			if ($approval_info->driver_abstract_extension=="Y") {
				if ($approval_info->driver_abstract_extension_date < $approval_info->extension_end_date)
					$approval_info->extension_end_date=$approval_info->driver_abstract_extension_date;
				} else {
				$approval_info->extension_ok='N';
				}
			}
		if ($approval_info->drug_screen_status!='Y') {
			if ($approval_info->drug_screen_extension=="Y") {
				if ($approval_info->drug_screen_extension_date < $approval_info->extension_end_date)
					$approval_info->extension_end_date=$approval_info->drug_screen_extension_date;
				} else {
				$approval_info->extension_ok='N';
				}
			}
		if ($approval_info->extension_end_date=="9999/99/99") $approval_info->extension_end_date="";
		}
	
	if ($approval_info->approval_status=='Y') $approval_info->color="#ffffff";
	else {
		$approval_info->color="#ffaaaa";
		if ($approval_info->extension_ok=='Y') $approval_info->color="#ffff99";
		}
	
	return ($approval_info);
	}


?>
