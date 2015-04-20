<?
session_write_close();

if ($pp_libarary!=1) {
	$pp_libarary=1;

/////////////////////////////////////////////////////////////
//	Returns an array of checks for both ap and pr for the
//	account that it is passed
/////////////////////////////////////////////////////////////
function send_error($message) {
	global $quiet_mode;
	
	if ($quiet_mode!="on") echo $message . "<br>\r\n";
	queue_email('2',"401 Automation Error Message",$message,'Y');
	queue_email('352',"401 Automation Error Message",$message,'Y');
	queue_email('353',"401 Automation Error Message",$message,'Y');
	queue_email('2834',"401 Automation Error Message",$message,'Y');
	queue_email('2995',"401 Automation Error Message",$message,'Y');
	return TRUE;
	}

function send_error_die($message) {
	send_error("ABORTING WITH FATAL ERROR: " . $message);
	die();
	}

function year_to_date($employee,$year="") {
	if ($year=="") $year=date('Y');
	if (($year>2050)||($year<2000)) die("Application error! Year to date function was fed and invalid year!!!!! ($year)");
	$start_date="01-01-$year";
	$end_date="12-31-$year";
	$value=ms_getoneb("select sum(Amount) as YTD from PRDT with (NOLOCK) where PREndDate > '$start_date' and PREndDate < '$end_date' and Employee = '$employee_num' and EDLCode = '401'");
	return($value);
	}

function tm_data_store() {
	global $total_record_count,$employee_contrib_total,$employee_roth_contrib_total,$match_contrib_total,$loan_totals,$ytd_match_contrib_total,$ytd_roth_match_contrib_total,$roth_employee_contrib_total,$roth_match_contrib_total;
	$save_totals_query="insert into positivepay_401k_totals set record_count = '$total_record_count', employee_contrib_pennies = '$employee_contrib_total', employee_roth_contrib_pennies = '$employee_roth_contrib_total',match_contrib_pennies = '$match_contrib_total',loan_repayment_pennies = '$loan_totals'";
	@mysql_query($save_totals_query);
	}
	
function tm_data_footer() {
	global $total_record_count,$tm_plan_number,$employee_contrib_total,$employee_roth_contrib_total,$match_contrib_total,$loan_totals,$ytd_match_contrib_total;
	$line="";
	# 1 - Description 
	$line = $line . tm_alpha_num("UMC 401k upload",20);
	if ($debug) $line=$line . '","';
	# 2 - Record count 
	$line = $line . tm_numeric($total_record_count,5);
	if ($debug) $line=$line . '","';
	# 3 - Employee contrib total
	$line = $line . tm_money($employee_contrib_total/100,9);
	if ($debug) $line=$line . '","';
	# 4 - Match contrib total
	$line = $line . tm_money($match_contrib_total/100,9);
	if ($debug) $line=$line . '","';
	# 5 - YTD match contrib total
	$line = $line . tm_money($ytd_match_contrib_total/100,9);
	if ($debug) $line=$line . '","';
	# 6 - Roth contrib total
	//$line = $line . tm_numeric("$employee_roth_contrib_total",9);	// $roth_employee_contrib_total
	$line = $line . tm_money($employee_roth_contrib_total/100,9);	// $roth_employee_contrib_total
	if ($debug) $line=$line . '","';
	# 7 - Roth match total
	//$line = $line . tm_numeric("0",9);	// $roth_match_contrib_total
	$line = $line . tm_money(0,9);	// $roth_match_contrib_total
	if ($debug) $line=$line . '","';
	# 8 - YTD roth match total
	$line = $line . tm_money(0,9);	// $ytd_roth_match_contrib_total ... bogus.. could use this slot for: $ytd_employee_roth_contrib though
	if ($debug) $line=$line . '","';
	# 9 - Loan repayment total
	$line = $line . tm_money($loan_totals/100,9);
	if ($debug) $line=$line . '","';
	return ($line . "\r\n");
	}
	
function tm_process_employee($employee_num,$start_date,$end_date,$pay_year,$bonuses_only=0) {
	global $tm_plan_number,$debug,$total_record_count,$employee_contrib_total,$employee_roth_contrib_total,$match_contrib_total,$ytd_match_contrib_total,$ytd_roth_match_contrib_total,$loan_totals,$roth_employee_contrib_total,$roth_match_contrib_total;
	$contact_info=getoneb("select * from contacts where employee_num = '$employee_num'");
	$hr_info=ms_getoneb("select * from HRRM with (NOLOCK) where HRCo = 1 and HRRef = '$employee_num'");
	$pr_info=ms_getoneb("select * from PREH with (NOLOCK) where PRCo = 1 and Employee = '$employee_num'");
	if ($pr_info->Craft!="9999") return ("");
	$rate_info=ms_getoneb("select * from PRAE with (NOLOCK) where PRCo = 1 and Employee = '$employee_num' and EarnCode = 401");
	$rate_info_roth=ms_getoneb("select * from PRAE with (NOLOCK) where PRCo = 1 and Employee = '$employee_num' and EarnCode = 405");
	$rate=$rate_info->RateAmt * -100;
	//if ($employee_num=="8906") $rate=0;
	
	$rate_roth=$rate_info_roth->RateAmt * -100;
	$contact_info=getoneb("select * from contacts where employee_num = '$employee_num'");
	if ($pr_info->Craft=="9999") $union_member=0;
	else $union_member=1;
	
	
	// For roth, cap this as a cumulative rate of both 401k and 401k roth
	//////////////////////////////////////////////////////////////
	//////////// The next couple of lines are garbage, and are
	//////////// overridden later in the code.....................
	//////////////////////////////////////////////////////////////
	//if (($rate+$rate_roth) > 6) $rate_matched=3;
	//else $rate_matched=($rate+$rate_roth)/2;
	if ($rate+$rate_roth>4) $rate_matched=4;
	else $rate_matched=$rate + $rate_roth;
	//////////////////////////////////////////////////////////////
	// </garbage>
	//////////////////////////////////////////////////////////////
	$ytd_top=$pay_year + 1;
	$ytd_bottom=$pay_year - 1;
	// All these people are 5% (or more) owners.
	// Everyone else is in the "default" category
	switch ($employee_num) {
		case "2507":
		case "2571":
		case "3330":
		case "4361":  
				 $owner="owner";
				 break;;
		default: $owner="";
				 break;;
		}
	$line="";
	$earncodes_query="( EarnCode = 1 or EarnCode = 2 or EarnCode = 3 or EarnCode = 8 or EarnCode = 9 or EarnCode = 10 or Earncode = 11 or EarnCode = 12 or EarnCode = 13 or EarnCode = 14 or EarnCode = 15 or EarnCode = 144 ) ";
	///////////////////////////////////////////////////
	// YTD Earnings
	///////////////////////////////////////////////////
	$query="SELECT SUM(PRDT.Amount) AS compensation, SUM(PRDT.Hours) AS hours
			FROM PRDT with (NOLOCK)
			INNER JOIN PRPC with (NOLOCK) ON 
				PRDT.PREndDate = PRPC.PREndDate AND PRDT.PRGroup = PRPC.PRGroup
			WHERE
				(PRDT.PRCo = 1) AND 
				(PRPC.PRCo = 1) AND 
				(PRDT.EDLType = 'E') AND 
				(PRDT.Employee = '$employee_num') AND 
				
				(PRPC.LimitMth >= '1/1/$pay_year') AND 
				(PRPC.LimitMth <= '12/31/$pay_year') AND
				
				(PRDT.PREndDate >= '12-1-$ytd_bottom') AND 
				(PRDT.PREndDate <= '1-31-$ytd_top') AND 
				
				(PRDT.EDLCode = 1 OR
				PRDT.EDLCode = 2 OR
				PRDT.EDLCode = 3 OR
				( PRDT.EDLCode >= 8 AND PRDT.EDLCode <= 13 ) OR
				PRDT.EDLCode = 135 OR
				PRDT.EDLCode = 162
				)
				";
	//ms_tabledump($query);die("ytd_stuff above");
	$ytd_total=ms_getoneb($query);
	//ms_tabledump($query);
	if (!($ytd_total)) die("Error detecting YTD for $employee_num");
	/////////////////////////////////////////////////////
	// Bonuses YTD
	/////////////////////////////////////////////////////
	$query="SELECT SUM(PRDT.Amount) AS bonuses, SUM(PRDT.Hours) AS hours
			FROM PRDT with (NOLOCK) 
			INNER JOIN PRPC with (NOLOCK) ON 
				PRDT.PREndDate = PRPC.PREndDate AND PRDT.PRGroup = PRPC.PRGroup
			WHERE
				(PRDT.PRCo = 1) AND 
				(PRPC.PRCo = 1) AND 
				(PRDT.EDLType = 'E') AND 
				(PRDT.Employee = '$employee_num') AND 
				
				(PRPC.LimitMth >= '1/1/$pay_year') AND 
				(PRPC.LimitMth <= '12/31/$pay_year') AND
				
				(PRDT.PREndDate >= '12-1-$ytd_bottom') AND 
				(PRDT.PREndDate <= '1-31-$ytd_top') AND 
				
				( PRDT.EDLCode = 11 OR PRDT.EDLCode = 12 )
				";
	$ytd_bonus_total=ms_getoneb($query);
	//ms_tabledump($query);
	if (!($ytd_bonus_total)) die("Error detecting bonus pay for this period for employee $employee_num");
	/////////////////////////////////////////////////////
	// Bonuses this period
	/////////////////////////////////////////////////////
	$query="SELECT SUM(PRDT.Amount) AS bonuses, SUM(PRDT.Hours) AS hours
			FROM PRDT with (NOLOCK)
			INNER JOIN PRPC with (NOLOCK) ON 
				PRDT.PREndDate = PRPC.PREndDate AND PRDT.PRGroup = PRPC.PRGroup
			WHERE
				(PRDT.PRCo = 1) AND 
				(PRPC.PRCo = 1) AND 
				(PRDT.EDLType = 'E') AND 
				(PRDT.Employee = '$employee_num') AND 
				
				(PRPC.LimitMth >= '1/1/$pay_year') AND 
				(PRPC.LimitMth <= '12/31/$pay_year') AND
				
				
				(PRDT.PREndDate >= '$start_date') AND 
				(PRDT.PREndDate <= '$end_date') AND 
				
				( PRDT.EDLCode = 11 OR PRDT.EDLCode = 12 )
				";
	$period_bonus_total=ms_getoneb($query);
	//$period_bonus_total=FALSE;
	//$period_bonus_total->bonuses=0;
	//ms_tabledump($query);
	if (!($period_bonus_total)) die("Error detecting bonus pay for this period for employee $employee_num");
	/////////////////////////////////////////////////////
	// This period's compensation
	/////////////////////////////////////////////////////
	$query="SELECT SUM(PRDT.Amount) AS compensation, SUM(PRDT.Hours) AS Hours
			FROM PRDT with (NOLOCK)
			INNER JOIN PRPC with (NOLOCK) ON 
				PRDT.PREndDate = PRPC.PREndDate AND PRDT.PRGroup = PRPC.PRGroup
			WHERE
				(PRDT.PRCo = 1) AND 
				(PRPC.PRCo = 1) AND 
				(PRDT.EDLType = 'E') AND 
				(PRDT.Employee = '$employee_num') AND 
				
				(PRPC.LimitMth >= '1/1/$pay_year') AND 
				(PRPC.LimitMth <= '12/31/$pay_year') AND
				
				
				(PRDT.PREndDate >= '$start_date') AND 
				(PRDT.PREndDate <= '$end_date') AND 
				
				(PRDT.EDLCode = 1 OR
				PRDT.EDLCode = 2 OR
				PRDT.EDLCode = 3 OR
				( PRDT.EDLCode >= 8 AND PRDT.EDLCode <= 13 ) OR
				PRDT.EDLCode = 135 OR
				PRDT.EDLCode = 162
				)
				";
	$this_period_total=ms_getoneb($query);
	if (!($this_period_total)) die("Error detecting compensation for this period for employee $employee_num");
	/////////////////////////////////////////////////////
	// This period's loan repayments
	/////////////////////////////////////////////////////
	$query="SELECT *
			FROM PRDT with (NOLOCK)
			INNER JOIN PRPC with (NOLOCK) ON 
				PRDT.PREndDate = PRPC.PREndDate AND PRDT.PRGroup = PRPC.PRGroup
			WHERE
				(PRDT.PRCo = 1) AND 
				(PRPC.PRCo = 1) AND 
				(PRDT.Employee = '$employee_num') AND 
				
				(PRPC.LimitMth >= '1/1/$pay_year') AND 
				(PRPC.LimitMth <= '12/31/$pay_year') AND
				
				(PRDT.PREndDate >= '$start_date') AND 
				(PRDT.PREndDate <= '$end_date') AND 
				
				(PRDT.EDLCode >= 31) AND
				(PRDT.EDLCode <= 39) AND
				((PRDT.Amount != 0 or PRDT.OverAmt!=0))
				ORDER BY PRDT.UseOver, PRDT.Amount DESC";
	$query="SELECT *
			FROM PRDT with (NOLOCK) 
			INNER JOIN PRPC with (NOLOCK) ON 
				PRDT.PREndDate = PRPC.PREndDate AND PRDT.PRGroup = PRPC.PRGroup
			WHERE
				(PRDT.PRCo = 1) AND 
				(PRPC.PRCo = 1) AND 
				(PRDT.Employee = '$employee_num') AND 
				
				(PRPC.LimitMth >= '1/1/$pay_year') AND 
				(PRPC.LimitMth <= '12/31/$pay_year') AND
				
				(PRDT.PREndDate >= '$start_date') AND 
				(PRDT.PREndDate <= '$end_date') AND 
				
				(PRDT.EDLCode >= 31) AND
				(PRDT.EDLCode <= 39) 
				ORDER BY PRDT.UseOver, PRDT.Amount DESC";

	$loannum1="0";
	$loannum2="0";
	$loannum3="0";
	$loannum4="0";
	$loannum5="0";

	$loanpay1="0";
	$loanpay2="0";
	$loanpay3="0";
	$loanpay4="0";
	$loanpay5="0";
	
	$this_periods_loans=mssql_query($query);
	$this_period_repay_total=0;
	$loancount=mssql_num_rows($this_periods_loans);
	if ($loancount > 0) {
		if ($loancount >= 1) {
			$row=mssql_fetch_object($this_periods_loans);
			if ($row->UseOver=='Y') $row->Amount=$row->OverAmt;
			//$loannum1=find_tm_loan_num($row->EDLCode);
			$loanpay1=$row->Amount;
			$loan_totals=round($loan_totals + ($row->Amount * 100));
			$this_period_repay_total=$this_period_repay_total + $row->Amount;
			}
		if ($loancount >= 2) {
			$row=mssql_fetch_object($this_periods_loans);
			if ($row->UseOver=='Y') $row->Amount=$row->OverAmt;
			//$loannum2=find_tm_loan_num($row->EDLCode);
			$loanpay2=$row->Amount;
			$loan_totals=round($loan_totals + ($row->Amount * 100));
			$this_period_repay_total=$this_period_repay_total + $row->Amount;
			}
		if ($loancount >= 3) {
			$row=mssql_fetch_object($this_periods_loans);
			if ($row->UseOver=='Y') $row->Amount=$row->OverAmt;
			//$loannum3=find_tm_loan_num($row->EDLCode);
			$loanpay3=$row->Amount;
			$loan_totals=round($loan_totals + ($row->Amount * 100));
			$this_period_repay_total=$this_period_repay_total + $row->Amount;
			if ($row->Amount > 0) send_error("Warning. A third 401k loan for employee $employee_num has been detected. I am sending the data to Merrill Lynch, but our current policy (as of this programs writing) is to only support 2 loans at a time!");
			}
		if ($loancount >= 4) {
			$row=mssql_fetch_object($this_periods_loans);
			if ($row->UseOver=='Y') $row->Amount=$row->OverAmt;
			//$loannum4=find_tm_loan_num($row->EDLCode);
			$loanpay4=$row->Amount;
			$loan_totals=round($loan_totals + ($row->Amount * 100));
			$this_period_repay_total=$this_period_repay_total + $row->Amount;
			if ($row->Amount > 0) send_error("Warning. A fourth 401k loan for employee $employee_num has been detected. I am sending the data to Merrill Lynch, but our current policy (as of this programs writing) is to only support 2 loans at a time!");
			}
		} 
	
	//if ($employee_num == "8283") {
	//	$loannum1="1";
	//	$loanpay1="362.71";
	//	$loan_totals=round($loan_totals + ($loanpay1 * 100));
	//	}
	/////////////////////////////////////////////////////
	// This period's 401k roth withdrawal
	/////////////////////////////////////////////////////
	// This section needs to be a dupe of the normal 401k
	// withdrawal section below, but it needs to be based
	// off of EDLCode = 405 instead.
	$query="SELECT SUM(PRDT.Amount) AS deduction
			FROM PRDT with (NOLOCK)
			INNER JOIN PRPC with (NOLOCK) ON 
				PRDT.PREndDate = PRPC.PREndDate AND PRDT.PRGroup = PRPC.PRGroup
			WHERE
				(PRDT.PRCo = 1) AND 
				(PRPC.PRCo = 1) AND 
				(PRDT.EDLType = 'E') AND 
				(PRDT.Employee = '$employee_num') AND 
				
				(PRPC.LimitMth >= '1/1/$pay_year') AND 
				(PRPC.LimitMth <= '12/31/$pay_year') AND
				
				(PRDT.PREndDate >= '$start_date') AND 
				(PRDT.PREndDate <= '$end_date') AND 
				
				(PRDT.EDLCode = 405)
				";
	$this_period_employee_roth_contrib=ms_getoneb($query);
	$employee_roth_contrib=$this_period_employee_roth_contrib->deduction * -1;
	$employee_roth_contrib_pennies=round(100 * $employee_roth_contrib);
	/////////////////////////////////////////////////////
	// YTD 401k roth withdrawal
	/////////////////////////////////////////////////////
	$query="SELECT SUM(PRDT.Amount) AS deduction
			FROM PRDT with (NOLOCK)
			INNER JOIN PRPC with (NOLOCK) ON 
				PRDT.PREndDate = PRPC.PREndDate AND PRDT.PRGroup = PRPC.PRGroup
			WHERE
				(PRDT.PRCo = 1) AND 
				(PRPC.PRCo = 1) AND 
				(PRDT.EDLType = 'E') AND 
				(PRDT.Employee = '$employee_num') AND 
				
				(PRPC.LimitMth >= '1/1/$pay_year') AND 
				(PRPC.LimitMth <= '12/31/$pay_year') AND
				
				(PRDT.PREndDate >= '12-1-$ytd_bottom') AND 
				(PRDT.PREndDate <= '1-31-$ytd_top') AND 
				
				(PRDT.EDLCode = 405)
				";
	$ytd_employee_roth_contrib_res=ms_getoneb($query);
	$ytd_employee_roth_contrib=$ytd_employee_roth_contrib_res->deduction * -1;
	/////////////////////////////////////////////////////
	// This period's 401k withdrawal
	/////////////////////////////////////////////////////
	$query="SELECT SUM(PRDT.Amount) AS deduction
			FROM PRDT with (NOLOCK)
			INNER JOIN PRPC with (NOLOCK) ON 
				PRDT.PREndDate = PRPC.PREndDate AND PRDT.PRGroup = PRPC.PRGroup
			WHERE
				(PRDT.PRCo = 1) AND 
				(PRPC.PRCo = 1) AND 
				(PRDT.EDLType = 'E') AND 
				(PRDT.Employee = '$employee_num') AND 
				
				(PRPC.LimitMth >= '1/1/$pay_year') AND 
				(PRPC.LimitMth <= '12/31/$pay_year') AND
				
				(PRDT.PREndDate >= '$start_date') AND 
				(PRDT.PREndDate <= '$end_date') AND 
				
				(PRDT.EDLCode = 401)
				";
	/*
	$query="SELECT PRDT.Amount AS deduction
			FROM PRDT 
			INNER JOIN PRPC ON 
				PRDT.PREndDate = PRPC.PREndDate AND PRDT.PRGroup = PRPC.PRGroup
			WHERE
				(PRDT.Amount != 1653.75) AND
				(PRDT.PRCo = 1) AND 
				(PRPC.PRCo = 1) AND 
				(PRDT.EDLType = 'E') AND 
				(PRDT.Employee = '$employee_num') AND 
				
				(PRPC.LimitMth >= '1/1/$pay_year') AND 
				(PRPC.LimitMth <= '12/31/$pay_year') AND
				
				(PRDT.PREndDate >= '$start_date') AND 
				(PRDT.PREndDate <= '$end_date') AND 
				
				(PRDT.EDLCode = 401)
				";
	*/
	//if ($employee_num=="8906") {
	//	ms_tabledump($query);
	//	die();
	//	}			
	$this_period_employee_contrib=ms_getoneb($query);
	$employee_contrib=$this_period_employee_contrib->deduction * -1;
	//if ($employee_num=="8906") $employee_contrib=0;

	$employee_contrib_pennies=round(100 * $employee_contrib);
	/////////////////////////////////////////////////////
	// YTD 401k withdrawal
	/////////////////////////////////////////////////////
	$query="SELECT SUM(PRDT.Amount) AS deduction
			FROM PRDT with (NOLOCK)
			INNER JOIN PRPC with (NOLOCK) ON 
				PRDT.PREndDate = PRPC.PREndDate AND PRDT.PRGroup = PRPC.PRGroup
			WHERE
				(PRDT.PRCo = 1) AND 
				(PRPC.PRCo = 1) AND 
				(PRDT.EDLType = 'E') AND 
				(PRDT.Employee = '$employee_num') AND 
				
				(PRPC.LimitMth >= '1/1/$pay_year') AND 
				(PRPC.LimitMth <= '12/31/$pay_year') AND
				
				(PRDT.PREndDate >= '12-1-$ytd_bottom') AND 
				(PRDT.PREndDate <= '1-31-$ytd_top') AND 
				
				(PRDT.EDLCode = 401)
				";
	$ytd_employee_contrib_res=ms_getoneb($query);
	$ytd_employee_contrib=$ytd_employee_contrib_res->deduction * -1;
	////////////////////////////////////////////////////
	$compensation_pennies=round($this_period_total->compensation * 100); 
	if ((($employee_contrib_pennies + $employee_roth_contrib_pennies) != 0)&&($compensation_pennies!=0)) {
		$employee_contrib_percent=($employee_contrib_pennies + $employee_roth_contrib_pennies) / $compensation_pennies * 100;
		if ($employee_contrib_percent>4) $rate_matched=4;
		else $rate_matched=$employee_contrib_percent;
		$match_conrib=0;
		$match_contrib=$rate_matched*$compensation_pennies/10000;
	} else {
		$employee_contrib_percent=0;
		$match_contrib=0;
	}
	
	$middlename=$pr_info->MidName;
	$middleinit=$middlename[0];

	/////////////////////////////////////////////////////////////////////////////////
	// OK, the prep work is done.. start pumping out data
	/////////////////////////////////////////////////////////////////////////////////
	# 0 - Line Numbers
	#$line = $line . tm_numeric($total_record_count,3);
	#if ($debug) $line=$line . '","';
	
	
	# 1 - Last Name
	if ($pr_info->Suffix!="") $pr_info->LastName=$pr_info->LastName . ' ' . $pr_info->Suffix;
	$line = $line . tm_alpha_num($pr_info->LastName,15);
	if ($debug) $line=$line . '","';
	# 2 - First Name
	$line = $line . tm_alpha_num($pr_info->FirstName ,15);
	if ($debug) $line=$line . '","';
	# 3 - Middle Initial
	$line = $line . tm_alpha_num($middleinit,1);
	if ($debug) $line=$line . '","';
	# 4 - SSN 
	$line = $line . tm_numeric(ereg_replace("-","",$pr_info->SSN),9);
	if ($debug) $line=$line . '","';
	#5 - Union member
	$line = $line . tm_numeric($union_member,1);
	if ($debug) $line=$line . '","';
	# 6 - Employee Number
	$line = $line . tm_alpha_num($pr_info->Employee,13);
	if ($debug) $line=$line . '","';
	#7 - Address Line 1
	//$addr_full=$pr_info->Address;
	$line = $line . tm_alpha_num(substr($pr_info->Address,0.29),60);
	if ($debug) $line=$line . '","';
	#8 - Address Line 2 // No spot in Viewpoint, so always blank
	$line = $line . tm_alpha_num(substr($pr_info->Address,29.29),30);
	if ($debug) $line=$line . '","';
	#9 - City
	$line = $line . tm_alpha_num($pr_info->City,18);
	if ($debug) $line=$line . '","';
	#10 - State
	$line = $line . tm_alpha_num($pr_info->State,2);
	if ($debug) $line=$line . '","';
	#11 - Zip 
	$line = $line . tm_alpha_num(ereg_replace("-","",$pr_info->Zip),9);
	if ($debug) $line=$line . '","';
	#12 - Date of Birth
	$line = $line . tm_date($pr_info->BirthDate,8);
	if ($debug) $line=$line . '","';
	#13 - Hire Date
	$line = $line . tm_date($pr_info->HireDate,8);
	if ($debug) $line=$line . '","';
	#14 = Orig Hire Date (They only need this until we switch to Safe Harbor at the end of the year
	$first_hr_info=ms_getoneb("select TOP 1 * FROM HREH with (NOLOCK) WHERE ( HRRef = '$employee_num' ) and ( HRCo = 1 ) order by Seq");
	$orighiredate="";
	if ($first_hr_info) $orighiredate=tm_date($first_hr_info->DateChanged,8);
	else  $orighiredate=tm_date($pr_info->HireDate,8);
	#15 - Date of Eligability (Hire date again)
	$line = $line . tm_date($pr_info->HireDate,8);
	if ($debug) $line=$line . '","';
	#16 - Date of Termination
	$line = $line . tm_date($pr_info->TermDate,8);
	if ($debug) $line=$line . '","';
	#17 - Pay Period Compensation (optional)
	//$this_period_total->compensation="99999";
	//$this_period_total->compensation="0";
	$line = $line . tm_money("$this_period_total->compensation",9);
	if ($debug) $line=$line . '","';
	#18 - YTD Compensation (optional)
	$line = $line . tm_money("$ytd_total->compensation",9);
	if ($debug) $line=$line . '","';
	#19 - Pay Period Hours
	$line = $line . tm_numeric(($this_period_total->Hours*100),7);
	if ($debug) $line=$line . '","';
	#20 - YTD Hours
	$line = $line . tm_numeric(($ytd_total->hours*100),7);
	if ($debug) $line=$line . '","';
	#21 - Employee Deferral Election (Deduction %) (Viewpoint.PRAE.RateAmt * 10000)
	//if ($employee_num=="8906") die("clean still");
	$line = $line . tm_numeric(($rate * 100),5);
	if ($debug) $line=$line . '","';
	#22 - Employee Contribution
	//if ($employee_contrib<0) $employee_contrib=0;
	// Temp hack for Shane to fix up what we sent..
	//if ($employee_num=="6211") $employee_contrib="-41.44";
	//if ($employee_num=="8906") $employee_contrib="-1653.75";
	//if ($employee_num=="8906") $employee_contrib=0;
	$line = $line . tm_money("$employee_contrib",9);
	if ($debug) $line=$line . '","';
	$employee_contrib_total=round($employee_contrib_total + (100 * $employee_contrib));
	#23 - Employer 401k Match
	// Temp - More of the Shane hack.. 
	//if ($employee_num=="6211") $match_contrib="48.45";
	//if ($employee_num=="8906") $match_contrib=0;
	$line = $line . tm_money("$match_contrib",9);
	if ($debug) $line=$line . '","';
	$match_contrib_total=round($match_contrib_total + (100 * $match_contrib));
	#24 - YTD Employee Contribution
	//if ($ytd_employee_contrib<0) $ytd_employee_contrib=0;
	$line = $line . tm_money("$ytd_employee_contrib",9);
	$ytd_employee_contrib_total=round($ytd_employee_contrib_total + (100 * $ytd_employee_contrib));
	if ($debug) $line=$line . '","';
	#25 - Employee Roth Election (Deduction %) (Place Holder)
	//$rothrate="0.0"; // Force this to be a placeholder
	$line = $line . tm_numeric(($rate_roth * 100),5);
	if ($debug) $line=$line . '","';
	#26 - Employee Roth Contribution
	//$employee_roth_contrib=0; // Force this to be a placeholder
	//if ($employee_roth_contrib<0) $employee_roth_contrib=0;
	$line = $line . tm_money("$employee_roth_contrib",9);
	if ($debug) $line=$line . '","';
	$employee_roth_contrib_total=round($employee_roth_contrib_total + (100 * $employee_roth_contrib));
	#27 - YTD Employee Roth Contribution
	$ytd_employee_roth_contrib=0; // Force this to be a placeholder
	//if ($ytd_employee_roth_contrib<0) $ytd_employee_roth_contrib=0;
	$line = $line . tm_money("$ytd_employee_roth_contrib",9);
	if ($debug) $line=$line . '","';
	#28 - Employer Roth Match
	$match_roth_contrib=0; // Force this to be a placeholder
	$line = $line . tm_money("$match_roth_contrib",9);
	if ($debug) $line=$line . '","';
	$match_roth_contrib_total=round($match_roth_contrib_total + (100 * $match_roth_contrib));
	#29 - Loan Repayment 
	$line = $line . tm_money("$this_period_repay_total",9);
	if ($debug) $line=$line . '","';

	$line=$line . "\r\n";
	$total_record_count++;
	return ($line);
	
	
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	// Below are the remnants of the old ML 401k crap.. 
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	# 1 - Record type
	if ($debug) $line=$line . '"';
	$line = $line . '71';
	if ($debug) $line=$line . '","';
	# 2 - Plan Number
	$line = $line . $tm_plan_number;
	if ($debug) $line=$line . '","';
	# 3 - SSN 
	$line = $line . tm_numeric(ereg_replace("-","",$pr_info->SSN),9);
	if ($debug) $line=$line . '","';
	# 4 - Participant status code
	if ($pr_info->ActiveYN=='Y') $participant_status_code='04';
	else $participant_status_code='30';
	$line = $line . tm_numeric($participant_status_code,2);
	if ($debug) $line=$line . '","';
	# 5 - Division 
	$line = $line . tm_alpha_num($pr_info->PRGroup,4);
	if ($debug) $line=$line . '","';
	# 6 - Employee Number
	$line = $line . tm_alpha_num($pr_info->Employee,13);
	if ($debug) $line=$line . '","';
	# 7 - Last Name
	if ($pr_info->Suffix!="") $pr_info->LastName=$pr_info->LastName . ' ' . $pr_info->Suffix;
	$line = $line . tm_alpha_num($pr_info->LastName,15);
	if ($debug) $line=$line . '","';
	# 8 - First Name
	$line = $line . tm_alpha_num($pr_info->FirstName . ' ' .$pr_info->Midname,15);
	if ($debug) $line=$line . '","';
	# 9 - Full Name
	$line = $line . tm_alpha_num('',30);
	if ($debug) $line=$line . '","';
	#10 - Date of Birth
	$line = $line . tm_date($pr_info->BirthDate,8);
	if ($debug) $line=$line . '","';
	#11 - Hire Date
	$line = $line . tm_date($pr_info->HireDate,8);
	if ($debug) $line=$line . '","';
	#12 - Date of Eligability (Hire date again)
	$line = $line . tm_date($pr_info->HireDate,8);
	if ($debug) $line=$line . '","';
	#13 - Date of Termination
	$line = $line . tm_date($pr_info->TermDate,8);
	if ($debug) $line=$line . '","';
	#14 - Alternate Vest Date
	$line = $line . tm_date("",8); // We don't use this, so send blank
	if ($debug) $line=$line . '","';
	#15 - Payroll Frequency (Everyone is W for weekly)
	$line = $line . tm_alpha_num('W',1);
	if ($debug) $line=$line . '","';
	#16 - Section 16 Insider (insider trading?)
	$line = $line . tm_alpha_num('0',1);
	if ($debug) $line=$line . '","';
	#17 - Address Line 1
	$line = $line . tm_alpha_num($pr_info->Address,60);
	if ($debug) $line=$line . '","';
	#18 - Address Line 2 // No spot in Viewpoint, so always blank
	$line = $line . tm_alpha_num("",30);
	if ($debug) $line=$line . '","';
	#19 - City
	$line = $line . tm_alpha_num($pr_info->City,18);
	if ($debug) $line=$line . '","';
	#20 - State
	$line = $line . tm_alpha_num($pr_info->State,2);
	if ($debug) $line=$line . '","';
	#21 - Zip 
	$line = $line . tm_alpha_num(ereg_replace("-","",$pr_info->Zip),9);
	if ($debug) $line=$line . '","';
	#22 - Source Label 1 // We use "A" for Employee Contribution
	$line = $line . tm_alpha_num("A",1);
	if ($debug) $line=$line . '","';
	#23 - Source Amount 1
	if ($employee_contrib<0) $employee_contrib=0;
	$line = $line . tm_money("$employee_contrib",9);
	if ($debug) $line=$line . '","';
	//$employee_contrib_total=round(100 * ($employee_contrib + $employee_contrib));
	$employee_contrib_total=round($employee_contrib_total + (100 * $employee_contrib));
	#24 - Source Label 2 // We use "D" for UMC matching contribution
	$line = $line . tm_alpha_num("D",1);
	if ($debug) $line=$line . '","';
	#25 - Source Amount 2
	$line = $line . tm_money("$match_contrib",9);
	if ($debug) $line=$line . '","';
	$match_contrib_total=round($match_contrib_total + (100 * $match_contrib));
	#26 - Source Label 3
	$line = $line . tm_alpha_num("",1);
	if ($debug) $line=$line . '","';
	#27 - Source Amount 3
	$line = $line . tm_money("",9);
	if ($debug) $line=$line . '","';
	#28 - Source Label 4
	$line = $line . tm_alpha_num("",1);
	if ($debug) $line=$line . '","';
	#29 - Source Amount 4
	$line = $line . tm_money("",9);
	if ($debug) $line=$line . '","';
	#30 - Source Label 5
	$line = $line . tm_alpha_num("",1);
	if ($debug) $line=$line . '","';
	#31 - Source Amount 5
	$line = $line . tm_money("",9);
	if ($debug) $line=$line . '","';
	#32 - Source Label 6
	$line = $line . tm_alpha_num("",1);
	if ($debug) $line=$line . '","';
	#33 - Source Amount 6
	$line = $line . tm_money("",9);
	if ($debug) $line=$line . '","';
	#34 - Pay Period Compensation (optional)
	//$line = $line . tm_money("$this_period_total->compensation",9);
	$line = $line . tm_money("",9);
	if ($debug) $line=$line . '","';
	#35 - ML Loan #
	$line = $line . tm_numeric("$loannum1",2);
	if ($debug) $line=$line . '","';
	#36 - ML Loan Repayment Amount
	$line = $line . tm_money("$loanpay1",7);
	if ($debug) $line=$line . '","';
	#37 - ML Loan #
	$line = $line . tm_numeric("$loannum2",2);
	if ($debug) $line=$line . '","';
	#38 - ML Loan Repayment Amount
	$line = $line . tm_money("$loanpay2",7);
	if ($debug) $line=$line . '","';
	#39 - ML Loan #
	$line = $line . tm_numeric("$loannum3",2);
	if ($debug) $line=$line . '","';
	#40 - ML Loan Repayment Amount
	$line = $line . tm_money("$loanpay3",7);
	if ($debug) $line=$line . '","';
	#41 - ML Loan #
	$line = $line . tm_numeric("$loannum4",2);
	if ($debug) $line=$line . '","';
	#42 - ML Loan Repayment Amount
	$line = $line . tm_money("$loanpay4",7);
	if ($debug) $line=$line . '","';
	#43 - ML Loan #
	$line = $line . tm_numeric("$loannum5",2);
	if ($debug) $line=$line . '","';
	#44 - ML Loan Repayment Amount
	$line = $line . tm_money("$loanpay5",7);
	if ($debug) $line=$line . '","';
	#45 - Current Base Pay (optional) // ML requests we leave blank
	$line = $line . tm_money("0",9);
	if ($debug) $line=$line . '","';
	#46 - Plan Comp
	$line = $line . tm_money("$ytd_total->compensation",9);
	if ($debug) $line=$line . '","';
	#47 - Bonuses and Commissions (non-recurring compensation)
	$line = $line . tm_money("$ytd_bonus_total->bonuses",9);
	if ($debug) $line=$line . '","';
	#48 - YTD Sec 125 Contrib (Cafeteria IRS thing) // No one uses it
	$line = $line . tm_money("0",9);
	if ($debug) $line=$line . '","';
	#49 - Before-tax contrib %
	$line = $line . tm_numeric($rate,4);
	if ($debug) $line=$line . '","';
	#50 - After-tax contrib %
	$line = $line . tm_numeric(0,4);
	if ($debug) $line=$line . '","';
	#51 - Profit Sharing // Probably not needed
	$line = $line . tm_money("$ytd_total->compensation",11);
	if ($debug) $line=$line . '","';
	#52 - Plan YTD Match compensation // Probably not needed
	$line = $line . tm_money("$ytd_total->compensation",11);
	if ($debug) $line=$line . '","';
	#53 - Period match compensation // Probably not needed
	$line = $line . tm_money("$this_period_total->compensation",11);
	if ($debug) $line=$line . '","';
	#54 - Non-discrim testing compensation
	$line = $line . tm_money("$ytd_total->compensation",11);
	if ($debug) $line=$line . '","';
	#55 - Access annual salary // We have no users in this program so n/a
	$line = $line . tm_money("0",10);
	if ($debug) $line=$line . '","';
	#56 - Salary Effective Date // We don't use scaled contrib % so n/a
	$line = $line . tm_alpha_num("",8);
	if ($debug) $line=$line . '","';
	#57 - Filler (4 spaces)
	$line = $line . tm_alpha_num("",4);
	if ($debug) $line=$line . '","';
	#58 - Part Indicator (constant of "R")
	$line = $line . tm_alpha_num("R",1);
	if ($debug) $line=$line . '","';
	#59 - Plan YTD Hours
	$line = $line . tm_numeric(($ytd_total->Hours * 1000),7);
	if ($debug) $line=$line . '","';
	#60 - 0=normal, 1=5%_owner, 4=officer
	$line = $line . tm_numeric($owner,1);
	if ($debug) $line=$line . '","';
	#61 - Key employee (n/a ML will calculate on their end for us)
	$line = $line . tm_numeric("0",1);
	if ($debug) $line=$line . '","';
	#62 - Excludable top 20% // Currently not required
	$line = $line . tm_numeric("0",1);
	if ($debug) $line=$line . '","';
	#63 - Highly compensated employee (n/a, ML will calculate for us)
	$line = $line . tm_numeric("0",1);
	if ($debug) $line=$line . '","';
	#64 - Union member
	$line = $line . tm_numeric($union_member,1);
	if ($debug) $line=$line . '","';
	#65 - Eligability Flag (no EZ enroll, send only a space!)
	$line = $line . tm_alpha_num(" ",1);
	if ($debug) $line=$line . '","';
	#66 - Eligible hours - YTD hours when client tracks eligiblity
	$line = $line . tm_numeric(($ytd_total->Hours*1000),7);
	if ($debug) $line=$line . '","';
	#67 - Payroll division // N/A we only have one payroll division
	$line = $line . tm_alpha_num(" ",1);
	if ($debug) $line=$line . '","';
	#68 - Rule 144 indicator // ML says fix this at 0 always for us (we're not publicly traded)
	$line = $line . tm_numeric(0,1);
	if ($debug) $line=$line . '","';
	#69 - 415 test compensation // just use normal ytd compensation
	$line = $line . tm_money("$ytd_total->compensation",9);
	if ($debug) $line=$line . '","';
	#70 - Resident of Peurto Rico // Just fix this as "  " (no) for now
	$line = $line . tm_alpha_num("  ",2);
	if ($debug) $line=$line . '","';
	#71 - Employer flag // Obsolete, do not use
	$line = $line . tm_alpha_num("  ",2);
	if ($debug) $line=$line . '","';
	#72 - Rehire date // Don't use, we nuke the term date anyway when someone comes back
	$line = $line . tm_alpha_num("",8);
	if ($debug) $line=$line . '","';
	#73 - Leave of absence type // Obsolete field, just fill with junk
	$line = $line . tm_alpha_num("",2);
	if ($debug) $line=$line . '","';
	#74 - Sex? (Sure, thanks...)
	$line = $line . tm_alpha_num("$pr_info->Sex",1);
	if ($debug) $line=$line . '","';
	#75 - Marital Status
	switch ($hr_info->MaritalStatus) {
		case "M":	$mar_status='2';
					break;;
		case "S":	$mar_status='1';
					break;;
		case "D":	$mar_status='1';
					break;;
		default:	send_error_die("Marital Status Error for $employee_num (status is '$hr_info->MaritalStatus')");
					break;;
		}
	$line = $line . tm_alpha_num("$mar_status",1);
	if ($debug) $line=$line . '","';
	#76 - FSE/LSE Inidicator // Obsolete, do not use
	$line = $line . tm_alpha_num("",1);
	if ($debug) $line=$line . '","';
	#77 - Save Rate Usage // Obsolete, do not use
	$line = $line . tm_alpha_num("",1);
	if ($debug) $line=$line . '","';
	#78 - Eligibility date // Obsolete, do not use
	$line = $line . tm_alpha_num("",8);
	if ($debug) $line=$line . '","';
	#79 - Business email address (optional, left out to promote privacy)
	//$line = $line . tm_alpha_num($contact_info->email,50);
	$line = $line . tm_alpha_num("",50);
	if ($debug) $line=$line . '","';
	#80 - USERRA start date // Military leave start date
	$line = $line . tm_alpha_num("",8);
	if ($debug) $line=$line . '","';
	#81 - USERRA end date // Military leave end date
	$line = $line . tm_alpha_num("",8);
	if ($debug) $line=$line . '","';
	#82 - LOA start date // Leave start date
	$line = $line . tm_alpha_num("",8);
	if ($debug) $line=$line . '","';
	#83 - LOA end date // Leave end date
	$line = $line . tm_alpha_num("",8);
	if ($debug) $line=$line . '","';
	#84 - Filler
	$line = $line . tm_alpha_num("",9);
	if ($debug) $line=$line . '"';
	
	$line=$line . "\r\n";
	$total_record_count++;
	return ($line);
	}

function tm_alpha_num($text,$length=30) {
	$text=str_pad($text , $length, ' ', STR_PAD_RIGHT );
	verify_length($text,$length);
	return $text;
	}

function tm_numeric($amount,$length=11) {
	$amount=round($amount);
	$amount=ereg_replace('[^0-9]','',$amount);
	$amount=str_pad($amount,$length,'0',STR_PAD_LEFT);
	verify_length($amount,$length);
	return($amount);
	}

function tm_signed_numeric($amount,$length=11) {
	}

function tm_money($amount,$length=11) {
	$pennies=$amount * 100;
	$pennies=round($pennies);
	/////////////////////////////////////////
	// Temporary fix to null out negatives..
	if ($pennies<0) {
		//$pennies=0;
		$pennies=round($pennies * -1);
		$output="-" . str_pad($pennies,$length - 1,'0',STR_PAD_LEFT);
		} else {
		$output=str_pad($pennies,$length,'0',STR_PAD_LEFT);
		//verify_length($output,$length);
		}
	verify_length($output,$length);
	/////////////////////////////////////////
	return($output);
	}

function tm_date($date,$length=8) {
	if ($date != "") $newdate=date('Ymd',strtotime($date));
	else $newdate=str_pad('',$length, ' ' ,STR_PAD_RIGHT);
	verify_length($newdate,$length);
	return ($newdate);
	}

function verify_length($string,$length) {
	global $current_employee_num;
	$real_length=strlen($string);
	if ($real_length!=$length) die("Application Error while processing employee number $current_employee_num! String length doesn't match requested! ($string, $real_length, $length");
	return $real_length;
	}

function log_batch_activity ($batch_id,$activity) {
	global $global_user_id;
	$batch_id=addslashes($batch_id);
	$activity=addslashes($activity);
	@mysql_query("insert into positivepay_batch_activity set activity = '$activity', batch_id = '$batch_id', action_date = now(), action_time = now(), action_user = '$global_user_id'");
	return 0;
	}

function get_account_checks($account_number) {
	$new_data=array();
	$array_count=0;
	$account_info=ms_getoneb("select * from CMAC with (NOLOCK) where CMCo = 1 and BankAcct = '$account_number'"); 
	$pennies=0;
	$ap_query="select * from APPH with (NOLOCK) where APCo = 1 and CMAcct = '$account_info->CMAcct' and PaidDate >= dateadd(day,-60,getdate())";
	$ap_res=@mssql_query($ap_query);
	while ($row=@mssql_fetch_object($ap_res)) {
		$new_data[$array_count]['vp_amount']=$row->Amount;
		$new_data[$array_count]['pennies']=round((100 * $row->Amount));
		$new_data[$array_count]['account_number']=$account_number;
		$new_data[$array_count]['pay_type']='ap';
		$new_data[$array_count]['payee']=$row->Name;
		$new_data[$array_count]['check_type']=$row->ChkType;
		$new_data[$array_count]['check_number']=eregi_replace(" ","",$row->CMRef);
		if ($row->VoidYN=='Y') $new_data[$array_count]['void']='Y';
		else $new_data[$array_count]['void']='N';
		$new_data[$array_count]['checkdate']=$row->PaidDate;
		$new_data[$array_count]['mysqldate']=date('Y-m-d',strtotime($row->PaidDate));
		$array_count++;
		}
	$pr_query="select * from PRPH with (NOLOCK),PREH with (NOLOCK) where PRPH.PRCo = 1 and PRPH.PayMethod != 'E' and PREH.PRCo = 1 and PRPH.Employee = PREH.Employee and PRPH.CMAcct = '$account_info->CMAcct' and PRPH.PaidDate >= dateadd(day,-60,getdate()) order by PRPH.CMRef";
	$pr_res=@mssql_query($pr_query);
	while ($row=@mssql_fetch_object($pr_res)) {
		$middlename="";
		$middlename=$row->MidName;
		$middleinit=$middlename[0];
		
		$payee=$row->FirstName . " " . $middleinit . ". " . $row->LastName;
		$new_data[$array_count]['vp_amount']=$row->PaidAmt;
		$new_data[$array_count]['pennies']=round((100 * $row->PaidAmt));
		$new_data[$array_count]['account_number']=$account_number;
		$new_data[$array_count]['pay_type']='pr';
		$new_data[$array_count]['payee']=$payee;
		$new_data[$array_count]['check_type']=$row->ChkType;
		$new_data[$array_count]['check_number']=eregi_replace(" ","",$row->CMRef);
		if ($row->VoidYN=='Y') $new_data[$array_count]['void']='Y';
		else $new_data[$array_count]['void']='N';
		$new_data[$array_count]['checkdate']=$row->PaidDate;
		$new_data[$array_count]['mysqldate']=date('Y-m-d',strtotime($row->PaidDate));
		$array_count++;
		}
	return $new_data;
	}

function check_check($check_info) {
	$account_number=$check_info['account_number'];
	$query="select * from positivepay_trans_history where 
	account_number = '$account_number' and 
	check_number = '$check_info[check_number]' and 
	void_status = '$check_info[void]' limit 1";
	return (getoneb($query));
	}

function get_new_batch_number() {
	return (getoneb("select ifnull(max(batch_number),0) + 1 as newnum from positivepay_trans_history")->newnum);
	}

function register_batch($trans_array,$batch_number) {
	$count=0;
	$batchlen=sizeof($trans_array);
	while ($count<$batchlen) {
		$trans_info=$trans_array[$count];
		$trans_info['payee']=addslashes($trans_info['payee']);
		$query="insert into positivepay_trans_history set
			account_number = '$trans_info[account_number]',
			check_number = '$trans_info[check_number]',
			void_status = '$trans_info[void]',
			payee = '$trans_info[payee]',
			check_date = '$trans_info[mysqldate]',
			pennies = '$trans_info[pennies]',
			vp_amount = '$trans_info[vp_amount]',
			pay_type = '$trans_info[pay_type]',
			batch_number = '$batch_number',
			status = 'sent',
			run_date = now()";
		mysql_query($query);
			$count++;
		}
	}

function chop_list($trans_array) {
	$list_size=sizeof($trans_array);
	$count=0;
	$newcount=0;
	$new_array=array();
	while ($count < $list_size) {
		if (!(check_check($trans_array[$count]))) {
			$new_array[$newcount]=$trans_array[$count];
			$newcount++;
			}
		$count++;
		}
	return ($new_array);
	}

/*	
function date_info($date="") {
if ($date=="") $date=date("m-d-Y");
$vali_date=vali_date($date);

$day=new bsclass;
$day->unixtime=strtotime($vali_date);
$day->unixtime2=$day->unixtime + 14400;
$day->day_of_week=date("D",$day->unixtime2);
$day->year=date("Y",$day->unixtime2);
$day->day_of_week_long=date("l",$day->unixtime2);
$day->date=$date;
$day->date_slashes=ereg_replace("-","/",$date);
$day->day_of_week_num=date("w",$day->unixtime2);

$day->week_end_unixtime=strtotime("saturday",$day->unixtime2);
$day->week_end_unixtime2=$day->week_end_unixtime + 14400;
$day->week_end=date("m-d-Y",$day->week_end_unixtime2);
$day->week_end_slashes=date("m/d/Y",$day->week_end_unixtime2);
$day->week_end_mysql=date("Y-m-d",$day->week_end_unixtime2);
$day->next_week_end_unixtime2=$day->week_end_unixtime2 + 604800;
$day->next_week_end=date("m-d-Y",$day->next_week_end_unixtime2);
$day->next_week_end_mysql=date("Y-m-d",$day->next_week_end_unixtime2);
$day->last_week_end_unixtime2=$day->week_end_unixtime2 - 604800;
$day->last_week_end=date("m-d-Y",$day->last_week_end_unixtime2);
$day->last_week_end_mysql=date("Y-m-d",$day->last_week_end_unixtime2);

$day->week_start_unixtime=strtotime("-6 days",$day->week_end_unixtime2);
$day->week_start_unixtime2=$day->week_start_unixtime + 14400;
$day->week_start=date("m-d-Y",$day->week_start_unixtime2);
$day->week_start_slashes=date("m/d/Y",$day->week_start_unixtime2);
$day->week_start_mysql=date("Y-m-d",$day->week_start_unixtime2);
$day->next_week_start_unixtime2=$day->week_start_unixtime2 + 604800;
$day->next_week_start=date("m-d-Y",$day->next_week_start_unixtime2);
$day->next_week_start_mysql=date("Y-m-d",$day->next_week_start_unixtime2);
$day->last_week_start_unixtime2=$day->week_start_unixtime2 - 604800;
$day->last_week_start=date("m-d-Y",$day->last_week_start_unixtime2);
$day->last_week_start_mysql=date("Y-m-d",$day->last_week_start_unixtime2);

$day->date_mysql=date("Y-m-d",$day->unixtime2);
$day->date_short=date("m-d",$day->unixtime2);
$day->date_short_slash=date("m/d",$day->unixtime2);
$day->next_unixtime=strtotime("+1 days",$day->unixtime2);
$day->next_unixtime2=$day->next_unixtime + 14400;
$day->next=date("m-d-Y",$day->next_unixtime2);
$day->previous_unixtime=strtotime("-1 days",$day->unixtime2);
$day->previous_unixtime2=$day->previous_unixtime + 14400;
$day->previous=date("m-d-Y",$day->previous_unixtime2);
$day->prev=$day->previous;

return $day;
}
*/
}
?>
