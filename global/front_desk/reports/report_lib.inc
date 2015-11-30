<?
function gap_num($number,$prefix="") {
	$number=addslashes($number);
	$fontstart="";
	$fontend="";
	$number=round($number,8);
	if ($number < 0) { $fontstart="<font color='red'>("; $fontend=")</font>"; $number=abs($number); }
	$number=format_long_num((integer) round($number));
	return ($fontstart . $prefix . $number . $fontend);
	}

function gap_decimal($number,$digits=2,$prefix="",$suffix="") {
	$number=addslashes($number);
	$fontstart="";
	$fontend="";
	$number=round($number,$digits);
	if ($number < 0) { $fontstart="<font color='red'>("; $fontend=")</font>"; $number=abs($number); }
	$number=format_long_decimal(round($number,$digits));
	return ($fontstart . $prefix . $number . $suffix . $fontend);
	}

function cost_type_array() {
	global $conn;
	$ct_array=array();
	$ctlistres=@odbc_exec($conn, "select * from JCCT with (NOLOCK) where PhaseGroup = 1");
	while ($ctrow=@odbc_fetch_object($ctlistres)) {
		$ct_array[$ctrow->CostType]=$ctrow->Description;
		}
	return ($ct_array);
	}

function phase_cat($phase) {
	$twodig=$phase[0] . $phase[1];
	$phaselist=array(
		'15'=>'Plumbing',
		'16'=>'HVAC Pipe',
		'17'=>'HVAC Sheet',
		'18'=>'Info Tech',
		'19'=>'Vacation',
		'20'=>'Misc',
		'24'=>'Startup/Test/Refr',
		'25'=>'Indirects',
		'26'=>'Indirects',
		'29'=>'Indirects',
		'33'=>'Equip/Mat',
		'55'=>'Subcontractors',
		'60'=>'Prof Svc',
		'62'=>'Restructure',
		'72'=>'Life/Med/HR',
		'74'=>'Prelim/Deduct/Audit',
		'78'=>'Depriciation',
		'80'=>'HW/Svcs/Phone',
		'99'=>'Stock',
		'UM'=>'UMC Equip',
		'YY'=>'Accounting Use',
		'ZZ'=>'PM Call PR'
		);
	return($phaselist[$twodig]);
	}

function cache_financial_summary($results) {
	if ($results->permanent=='Y') {
		@mysql_query("delete from front_desk_financials_cache where permanent = 'Y' and week_ending = '$results->week_ending' and contract = '$results->contract'");
		} else {
		@mysql_query("delete from front_desk_financials_cache where permanent = 'N' and contract = '$results->contract'");
		}
	
	$query="insert into front_desk_financials_cache set 
	week_ending	= '$results->week_ending',
	permanent 	= '$results->permanent',
	contract	= '$results->contract',
	description	= '$results->description',
	retainage	= '$results->retainage',
	billedamt	= '$results->billedamt',
	receivedamt	= '$results->receivedamt',
	origesthours	= '$results->origesthours',
	curresthours	= '$results->curresthours',
	actualhours	= '$results->actualhours',
	currestcost	= '$results->currestcost',
	actualcost	= '$results->actualcost',
	cost_to_date	= '$results->cost_to_date',
	lowest_cost	= '$results->lowest_cost',
	paid_to_date	= '$results->paid_to_date',
	hours_remaining	= '$results->hours_remaining',
	cost_remaining	= '$results->cost_remaining',
	remaincmtdcost	= '$results->remaincmtdcost',
	paidtodate	= '$results->paidtodate',
	netbilledamt	= '$results->netbilledamt',
	netcashflow    	= '$results->netcashflow',
	total_composite_rate	= '$results->total_composite_rate',
	total_actual_hours	= '$results->total_actual_hours',
	total_labor_actual_cost	= '$results->total_labor_actual_cost',
	total_labor_est_hours	= '$results->total_labor_est_hours',
	total_labor_est_cost	= '$results->total_labor_est_cost',
	est_composite_rate	= '$results->est_composite_rate',
	rcc_material	= '$results->rcc_material',
	rcc_subcontracts	= '$results->rcc_subcontracts',
	rcc_equipment	= '$results->rcc_equipment',
	apui_all_count	= '$results->apui_all_count',
	apui_all_total	= '$results->apui_all_total',
	apui_persistent_count	= '$results->apui_persistent_count',
	apui_persistent_total	= '$results->apui_persistent_total',
	orig_contract_amount	= '$results->orig_contract_amount',
	current_contract_amount	= '$results->current_contract_amount',
	change_orders	= '$results->change_orders',
	current_gross_margin	= '$results->current_gross_margin',
	percent_complete	= '$results->percent_complete',
	estimated_billed	= '$results->estimated_billed',
	over_billed	= '$results->over_billed',
	over_billed_percent	= '$results->over_billed_percent'";
	@mysql_query($query);

	}


function job_financial_summary($contract_number="",$week_ending_date="",$permanent='N') {
	//echo "debug info";
	global $global_jobinfo_id,$debug;
	if ($debug==1) {
		echo "am I here";
        tcheck("start of financial summary");
    }
	//echo("did i make it again");
	if ($contract_number=="") $contract_number=get_vp_contract_num($global_jobinfo_id);
	
	//echo ("select * from front_desk_financials_cache where contract = '$contract_number' and permanent = 'N'");
	//die();
	$results=getoneb("select * from front_desk_financials_cache where contract = '$contract_number' and permanent = 'N' and timestampadd(MINUTE,65,last_updated_time) > now()");
	
	if (TRUE) {
	$results="";
	if ($debug==1) tcheck("summary: before JCCM main lookup");
	$results=ms_getoneb("select * from JCCM with (NOLOCK) where JCCo = 1 and Contract like '$contract_number%'");
	//echo "<hr><hr><hr>" . "select * from JCCM with (NOLOCK) where JCCo = 1 and Contract like '$contract_number%'";
	//ms_tabledump("select * from JCCM with (NOLOCK) where JCCo = 1 and Contract like '$contract_number%'");
	
	//ms_tabledump("select * from JCCM where JCCo = 1 and Contract like '$contract_number%'");
	//die("done");
	$tbtotalquery="
select sum(OrigEstHours) as OrigEstHours, 
sum(CurrEstHours) as CurrEstHours, 
sum(ActualHours) as ActualHours, 
sum(CurrEstCost) as CurrEstCost, 
sum(RemainCmtdCost) as RemainCmtdCost, 
sum(ActualCost) as ActualCost 
from JCCP with (NOLOCK) 
where JCCo = 1 and Job like '$contract_number%' group by JCCo

";

	if ($debug==1) tcheck("summary: before JCCP lookup for estimates");
	$tbrow=ms_getoneb($tbtotalquery);
	//$labortotalquery="select sum(OrigEstHours) as OrigEstHours,sum(CurrEstHours) as CurrEstHours,sum(ActualHours) as ActualHours, sum(CurrEstCost) as CurrEstCost, sum(RemainCmtdCost) as RemainCmtdCost, sum(ActualCost) as ActualCost	from JCCP where JCCo = 1 and Job like '$contract_number%' and ActualHours != 0 group by JCCo";
	$labortotalquery="select sum(OrigEstHours) as OrigEstHours,sum(CurrEstHours) as CurrEstHours,sum(ActualHours) as ActualHours, sum(CurrEstCost) as CurrEstCost, sum(RemainCmtdCost) as RemainCmtdCost, sum(ActualCost) as ActualCost	from JCCP with (NOLOCK) where JCCo = 1 and Job like '$contract_number%' and CostType = 1 group by JCCo";
	if ($debug==1) tcheck("summary: before JCCP lookup for labor only estimates");
	$labor_totals=ms_getoneb($labortotalquery);
	if ($debug==1) tcheck("summary: before lookup for paid_to_date_diffs");
	$paid_to_date_diff=ms_getoneb("
		select Sum(APTD.Amount) as total 
		from APTD with (NOLOCK) 
		RIGHT OUTER JOIN APTL with (NOLOCK) ON APTD.APCo = APTL.APCo and APTD.Mth = APTL.Mth and APTD.APTrans = APTL.APTrans and APTD.APLine = APTL.APLine 
		where (APTD.APCo = 1) AND (APTL.Job LIKE '$contract_number%') AND (APTD.Status = 1 OR APTD.Status = 2)");
	//ms_tabledump("select Sum(APTD.Amount) as total from APTD with (NOLOCK) RIGHT OUTER JOIN APTL with (NOLOCK) ON APTD.APCo = APTL.APCo and APTD.Mth = APTL.Mth and APTD.APTrans = APTL.APTrans and APTD.APLine = APTL.APLine where (APTD.APCo = 1) AND (APTL.Job LIKE '$contract_number%') AND (APTD.Status = 1 OR APTD.Status = 2)");
	
	//echo ("select Sum(APTD.Amount) as total from APTD RIGHT OUTER JOIN APTL ON APTD.APCo = APTL.APCo and APTD.Mth = APTL.Mth and APTD.APTrans = APTL.APTrans and APTD.APLine = APTL.APLine where (APTD.APCo = 1) AND (APTL.Job LIKE '$contract_number%') AND (APTD.Status = 1 OR APTD.Status = 2)");
	if ($debug==1) tcheck("summary: before committed cost lookups from POIT with (NOLOCK) for material");
	$rcc_material=ms_getoneb("SELECT sum(RemCost + RemTax) as total FROM POIT with (NOLOCK) WHERE  (JCCType = 2) and (POCo = 1) AND (Job LIKE '$contract_number%')");
	//$rcc_subcontracts=ms_getoneb("SELECT sum(RemCost + RemTax) as total FROM POIT WHERE  (JCCType = 3) and (POCo = 1) AND (Job LIKE '$contract_number%')");
	//ms_tabledump("SELECT sum(RemCost + RemTax) as total FROM POIT WHERE  (JCCType = 3) and (POCo = 1) AND (Job LIKE '$contract_number%')");
	//$rcc_subcontracts=ms_getoneb("SELECT sum(RemCost + RemTax) as total FROM POIT WHERE  (JCCType = 3) and (POCo = 1) AND (Job LIKE '$contract_number%')");
	if ($debug==1) tcheck("summary: before committed cost lookups from POIT with (NOLOCK) for subcontracts");
	$rcc_subcontracts=ms_getoneb("SELECT sum(RemainCmtdCost) as total FROM JCCP with (NOLOCK) WHERE (CostType = 3) and (JCCo = 1) AND (Job LIKE '$contract_number%')");
	if ($debug==1) tcheck("summary: before committed cost lookups from POIT with (NOLOCK) for equipment");
	$rcc_equipment=ms_getoneb("SELECT sum(RemCost + RemTax) as total FROM POIT with (NOLOCK) WHERE (JCCType = 4) and (POCo = 1) AND (Job LIKE '$contract_number%')");
	$apui_cutoff_date=date("m/d/Y",strtotime("1 week ago"));
	if ($debug==1) tcheck("summary: before APUI totals lookup");
	$apui_all=ms_getoneb("SELECT SUM(InvTotal) AS total,SUM(1) AS count FROM APUI with (NOLOCK) WHERE (udJOB LIKE '$contract_number%') AND (APCo = 1) GROUP BY APCo");
	if ($debug==1) tcheck("summary: before persistent APUI totals lookup");
	$apui_persistent=ms_getoneb("SELECT SUM(InvTotal) AS total,SUM(1) AS count FROM APUI with (NOLOCK) WHERE (udJOB LIKE '$contract_number%') AND (APCo = 1)  and (udentrydate < '$apui_cutoff_date') GROUP BY APCo");
	if ($debug==1) tcheck("summary: all lookups done...");
	////////////////////////////////////////////////////////////////////
	//	Queries are done.. time to calculate things now..
	////////////////////////////////////////////////////////////////////
	if ($week_ending_date!="") {
		$we_date_info=date_info($week_ending_date);
		$results->week_ending=$we_date_info->week_end_mysql;
		} else {
		$we_date_info=date_info();
		$results->week_ending=$we_date_info->week_end_mysql;
		}
	$results->permanent=$permanent;
	$results->contract=$results->Contract;
	$results->description=$results->Description;
	$results->retainage=$results->CurrentRetainAmt;
	$results->billedamt=$results->BilledAmt;
	$results->receivedamt=$results->ReceivedAmt;
	$results->origesthours=$tbrow->OrigEstHours;
	$results->curresthours=$tbrow->CurrEstHours;
	$results->actualhours=$tbrow->ActualHours;
	$results->currestcost=$tbrow->CurrEstCost;
	$results->actualcost=$tbrow->ActualCost;
	$results->cost_to_date=$results->actualcost;
	$results->lowest_cost=$results->currestcost;
	if ($results->actualcost > $results->lowest_cost) $results->lowest_cost=$results->actualcost;
	//$results->paid_to_date=$results->actualcost - $paid_to_date_diff->total;
	$results->hours_remaining=$tbrow->CurrEstHours - $tbrow->ActualHours;
	$results->cost_remaining=$tbrow->CurrEstCost - $tbrow->ActualCost;
	$results->remaincmtdcost=$tbrow->RemainCmtdCost;
	$results->billedamt=$results->BilledAmt;
	//$results->receivedamt=$results->ReceivedAmt;
	$open_ap=$paid_to_date_diff;
	//$results->paidtodate=$results->actualcost - $open_ap->total;
	$results->paidtodate=$results->actualcost - $paid_to_date_diff->total;
	$results->netbilledamt=$results->billedamt - $results->retainage;
	$results->netcashflow=$results->receivedamt - $results->paidtodate;
	if ($labor_totals->ActualHours!=0) $results->total_composite_rate=$labor_totals->ActualCost / $labor_totals->ActualHours;
	$results->total_actual_hours=$labor_totals->ActualHours;
	$results->total_labor_actual_cost=$labor_totals->ActualHours;
	$results->total_labor_est_hours=$labor_totals->CurrEstHours;
	$results->total_labor_est_cost=$labor_totals->CurrEstCost;
	if ($results->total_labor_est_hours>0) $results->est_composite_rate=$results->total_labor_est_cost / $results->total_labor_est_hours;
	$results->rcc_material=$rcc_material->total;
	$results->rcc_subcontracts=$rcc_subcontracts->total;
	$results->rcc_equipment=$rcc_equipment->total;
	$results->apui_all_count=$apui_all->count;
	$results->apui_all_total=$apui_all->total;
	$results->apui_persistent_count=$apui_persistent->count;
	$results->apui_persistent_total=$apui_persistent->total;
	$results->orig_contract_amount=$results->OrigContractAmt;
	$results->current_contract_amount=$results->ContractAmt;
	$results->change_orders=$results->ContractAmt - $results->OrigContractAmt;
	$results->current_gross_margin=$results->current_contract_amount - $results->lowest_cost;
	if ($results->currestcost!=0) $results->percent_complete=$results->cost_to_date / $results->currestcost;
	$results->estimated_billed=$results->actualcost + ($results->percent_complete * $results->current_gross_margin);
	$results->over_billed=$results->billedamt - $results->estimated_billed;
	if ($results->billedamt!=0) $results->over_billed_percent=$results->over_billed / $results->billedamt;
	
	cache_financial_summary($results);
	}
	//echo "</center><pre><div style=\"text-align: left;\">";
	//print_r($results);
	//echo "</div></pre>";
	//die();
	
	
	
	
	
	
	
	
	
	
	////////////////////////////////////////////////////////////////////
	// Queries and calcuations are done.. now we just format everything.
	////////////////////////////////////////////////////////////////////
	$results->retainage_decimal=format_long_decimal($results->retainage,2);
	$results->retainage_gap_num=gap_num($results->retainage);
	//
	$results->billedamt_decimal=format_long_decimal($results->billedamt,2);
	$results->billedamt_gap_num=gap_num($results->billedamt);
	//
	$results->receivedamt_decimal=format_long_decimal($results->receivedamt,2);
	$results->receivedamt_gap_num=gap_num($results->receivedamt);
	//
	$results->origesthours_decimal=format_long_decimal($results->origesthours,2);
	$results->origesthours_gap_num=gap_num($results->origesthours);
	//
	$results->curresthours_decimal=format_long_decimal($results->curresthours,2);
	$results->curresthours_gap_num=gap_num($results->curresthours);
	//
	$results->actualhours_decimal=format_long_decimal($results->actualhours,2);
	$results->actualhours_gap_num=gap_num($results->actualhours);
	//
	$results->currestcost_decimal=format_long_decimal($results->currestcost,2);
	$results->currestcost_gap_num=gap_num($results->currestcost);
	//
	$results->actualcost_decimal=format_long_decimal($results->actualcost,2);
	$results->actualcost_gap_num=gap_num($results->actualcost);
	//
	$results->cost_to_date_decimal=$results->actualcost_decimal;
	$results->cost_to_date_gap_num=$results->actualcost_gap_num;
	//	
	$results->lowest_cost_decimal=format_long_decimal($results->lowest_cost,2);
	$results->lowest_cost_gap_num=gap_num($results->lowest_cost);
	//
	$results->paid_to_date_decimal=format_long_decimal($results->paid_to_date,2);;
	$results->paid_to_date_gap_num=gap_num($results->paid_to_date);;
	//
	$results->hours_remaining_decimal=format_long_decimal($results->hours_remaining,2);
	$results->hours_remaining_gap_num=gap_num($results->hours_remaining);
	//
	$results->cost_remaining_decimal=format_long_decimal($results->cost_remaining,2);
	$results->cost_remaining_gap_num=gap_num($results->cost_remaining);
	//
	$results->remaincmtdcost_decimal=format_long_decimal($results->remaincmtdcost,2);
	$results->remaincmtdcost_gap_num=gap_num($results->remaincmtdcost);
	//
	$results->billedamt_decimal=format_long_decimal($results->billedamt,2);
	$results->billedamt_gap_num=gap_num($results->billedamt);
	//
	$results->receivedamt_decimal=format_long_decimal($results->receivedamt,2);
	$results->receivedamt_gap_num=gap_num($results->receivedamt);
	//
	$results->paidtodate_decimal=format_long_decimal($results->paidtodate,2);
	$results->paidtodate_gap_num=gap_num($results->paidtodate);
	//
	$results->netbilledamt_decimal=format_long_decimal($results->netbilledamt,2);
	$results->netbilledamt_gap_num=gap_num($results->netbilledamt);
	//
	$results->netcashflow_decimal=format_long_decimal($results->netcashflow,2);
	$results->netcashflow_gap_num=gap_num($results->netcashflow);
	//
	$results->total_composite_rate_decimal=format_long_decimal($results->total_composite_rate,2);
	$results->total_composite_rate_gap_num=gap_num($results->total_composite_rate);
	//
	$results->total_actual_hours_decimal=format_long_decimal($results->total_actual_hours,2);
	$results->total_actual_hours_gap_num=gap_num($results->total_actual_hours);
	//
	$results->total_labor_actual_cost_decimal=format_long_decimal($results->total_labor_actual_cost,2);
	$results->total_labor_actual_cost_gap_num=gap_num($results->total_labor_actual_cost);
	//
	$results->total_labor_est_hours_decimal=format_long_decimal($results->total_labor_est_hours,2);
	$results->total_labor_est_hours_gap_num=gap_num($results->total_est_hours_cost);
	//
	$results->total_labor_est_cost_decimal=format_long_decimal($results->total_labor_est_cost,2);
	$results->total_labor_est_cost_gap_num=gap_num($results->total_labor_est_cost);
	//
	$results->est_composite_rate_decimal=format_long_decimal($results->est_composite_rate,2);
	$results->est_composite_rate_gap_num=gap_num($results->est_composite_rate);
	//
	$results->rcc_material_decimal=format_long_decimal($results->rcc_material,2);
	$results->rcc_material_gap_num=gap_num($results->rcc_material);
	//
	$results->rcc_subcontracts_decimal=format_long_decimal($results->rcc_subcontracts,2);
	$results->rcc_subcontracts_gap_num=gap_num($results->rcc_subcontracts);
	//
	$results->rcc_equipment_decimal=format_long_decimal($results->rcc_equipment,2);
	$results->rcc_equipment_gap_num=gap_num($results->rcc_equipment);
	//
	$results->apui_all_total_decimal=format_long_decimal($results->apui_all_total);
	$results->apui_all_total_gap_num=gap_num($results->apui_all_total);
	//
	$results->apui_persistent_total_decimal=format_long_decimal($results->apui_persistent_total,2);
	$results->apui_persistent_total_gap_num=gap_num($results->apui_persistent_total);
	//
	$results->orig_contract_amount_decimal=format_long_decimal($results->orig_contract_amount,2);
	$results->orig_contract_amount_gap_num=gap_num($results->orig_contract_amount);
	//
	$results->current_contract_amount_decimal=format_long_decimal($results->current_contract_amount,2);
	$results->current_contract_amount_gap_num=gap_num($results->current_contract_amount);
	//
	$results->change_orders_decimal=format_long_decimal($results->change_orders,2);
	$results->change_orders_gap_num=gap_num($results->change_orders);
	//
	$results->current_gross_margin_decimal=format_long_decimal($results->current_gross_margin);
	$results->current_gross_margin_gap_num=gap_num($results->current_gross_margin);
	//
	//$results->percent_complete=$results->actualhours / $results->curresthours;
	//$results->percent_complete_decimal=format_long_decimal(100 * $results->percent_complete);
	//$results->percent_complete_gap_num=gap_num(100 * $results->percent_complete);
	$results->percent_complete_decimal=format_long_decimal(100 * $results->percent_complete);
	$results->percent_complete_gap_num=gap_num(100 * $results->percent_complete);
	//
	$results->estimated_billed_decimal=format_long_decimal($results->estimated_billed);
	$results->estimated_billed_gap_num=gap_num($results->estimated_billed);
	//
	$results->over_billed_decimal=format_long_decimal($results->over_billed);
	$results->over_billed_gap_num=gap_num($results->over_billed);
	//
	$results->over_billed_percent_decimal=format_long_decimal($results->over_billed_percent * 100,2);
	//$results->over_billed_percent_gap_decimal=gap_decimal($results->over_billed_percent * 100,1);
	$results->over_billed_percent_gap_num=gap_num($results->over_billed_percent * 100);
	//
	//
	if ($debug==1) tcheck("summary: end of summary lookups.. nothing but math and formatting since lookups.");
	return ($results);
	}

function get_vp_contract_num($jobinfo_id="") {
	global $global_jobinfo;
	//echo "<hr>$jobinfo_id<hr>";
	if ($jobinfo_id=="") $jobinfo_id=$global_jobinfo->jobinfo_id;
	
	if ($jobinfo_id!=$global_jobinfo->jobinfo_id) {
		//echo "looking up passed in jobinfo information";
		$jobinfo_id=addslashes($jobinfo_id);
		$jobinfo=getoneb("select * from jobinfo where jobinfo_id = '$jobinfo_id'");
		//echo "<pre>";print_r($jobinfo);echo"</pre>";
		//if (!($jobinfo)) die("error picking up info for passed in job");
		} else {
		$jobinfo=$global_jobinfo;
		}
	$job_num=strtoupper(escapeshellcmd($jobinfo->contract_num));
	$job_num_p1=str_pad(ereg_replace("-.*","",$job_num),5,"0",STR_PAD_LEFT);
	if ($job_num_p1=="00000") return FALSE;
	$job_num_p2=ereg_replace(".*-","",$job_num);
	if ($job_num_p1==$job_num_p2) $job_num_p2="";
	if ($job_num_p2=="") {
		$job_str_query1="$job_num_p1-";
	} else {
		$job_num_p2=str_pad($job_num_p2,3," ",STR_PAD_LEFT);
		$job_str_query1="$job_num_p1-$job_num_p2";
		$job_num_p2=ereg_replace(" ","0",$job_num_p2);
		$job_str_query2="$job_num_p1-$job_num_p2";
		if ($job_str_query1==$job_str_query2) $job_str_query2="";
	}
	$job_query1="select * from JCJM with (NOLOCK) where JCCo = 1 and Job = '$job_str_query1'"; 
	//ms_tabledump($job_query1);
	$job_res=ms_getoneb($job_query1);
    
	if ($job_res) {
		$job_res->job=$job_str_query1;
		$job_res->job_query=$job_query1;
		$job_res->Description=escapeshellcmd($job_res->Description);
	} else {
		if ($job_str_query2!="") {
			$job_query2="select * from JCJM with (NOLOCK) where JCCo = 1 and Job = '$job_str_query2'"; 
			$job_res=ms_getoneb($job_query2);
			if ($job_res) {
				$job_res->job=$job_str_query2;
				$job_res->job_query=$job_query2;
				$job_res->Description=escapeshellcmd($job_res->Description);
			}
		}
	}
	return ($job_res->job);
	}

/*
function po_unread_count($employee_num,$jobinfo_id) {
	$jobinfo_id=addslashes($jobinfo_id);
	$jobinfo=getoneb("select * from jobinfo where jobinfo_id = '$jobinfo_id'");
	
	$unviewed_po_query="
	select 
	polist.PO
	from (
	select POHD.PO as PO,
	sum(POIT.TotalCost + POIT.TotalTax) as TotalCostSum

	from POHD 
	left join POIT on POHD.PO = POIT.PO and POIT.POCo = 1
	where POIT.Job like '0$jobinfo->contract_num%' and POHD.POCo = 1
	group by POHD.PO ) polist 
	
	left join zz_umc_fd_po_review_history on 
	polist.PO = zz_umc_fd_po_review_history.PO and 
	polist.TotalCostSum = zz_umc_fd_po_review_history.last_amount and 
	zz_umc_fd_po_review_history.Employee = '$global_user->employee_num'
	where po_hist_id is null
	";
	$res=mssql_query($unviewed_po_query);
	$count=mssql_num_rows($res);
	return $count;
	}
*/

function po_mark_all_viewed($employee_num,$jobinfo_id) {
	$jobinfo_id=addslashes($jobinfo_id);
	$jobinfo=getoneb("select * from jobinfo where jobinfo_id = '$jobinfo_id'");
	
	$contract_num=get_vp_contract_num($jobinfo->jobinfo_id);
	/*
	
	$unviewed_po_query="
	select 
	polist.PO, polist.TotalCostSum,po_hist_id,zz_umc_fd_po_review_history.Employee
	
	from (
	select POHD.PO as PO,
	sum(POIT.TotalCost + POIT.TotalTax) as TotalCostSum
	
	from POHD 
	left join POIT on POHD.PO = POIT.PO and POIT.POCo = 1
	where POIT.Job like '$job%' and POHD.POCo = 1
	group by POHD.PO ) polist
	
	left join zz_umc_fd_po_review_history on 
	polist.PO = zz_umc_fd_po_review_history.PO and 
	polist.TotalCostSum = zz_umc_fd_po_review_history.last_amount and 
	zz_umc_fd_po_review_history.Employee = '$global_user->employee_num'
	
	where po_hist_id is null
	";
	*/
	$unviewed_po_query="
	select
	polist.PO,polist.TotalCostSum,Vendor,OrderDate,polist.Contract
	from (
	select POHD.PO as PO,POHD.Vendor as Vendor, POHD.OrderDate as OrderDate,
	sum(POIT.TotalCost + POIT.TotalTax) as TotalCostSum, JCJM.Contract
	from POHD with (NOLOCK)  
	left join POIT with (NOLOCK) on POHD.PO = POIT.PO and POIT.POCo = 1
	left join JCJM with (NOLOCK) on POIT.Job = JCJM.Job and JCJM.JCCo = POIT.POCo
	where ( JCJM.Contract = '$contract_num' ) and POHD.POCo = 1
	group by POHD.PO,Vendor,POHD.OrderDate,JCJM.Contract ) polist 
	left join zz_umc_fd_po_review_history with (NOLOCK) on 
	polist.PO = zz_umc_fd_po_review_history.PO and 
	polist.TotalCostSum = zz_umc_fd_po_review_history.last_amount and 
	zz_umc_fd_po_review_history.Employee = '$employee_num'
	where po_hist_id is null order by polist.Contract,polist.PO";
	//ms_tabledump($unviewed_po_query);
	$res=@mssql_query($unviewed_po_query);
	while ($row=@mssql_fetch_object($res)) {
		po_history_update($row->PO,$row->TotalCostSum,addslashes($employee_num));
		}
	//exit;
	}


function po_history_data($po_number,$total,$force_employee_num="") {
	global $po_proxy_for,$global_user;
	if ($force_employee_num!="") $emp=addslashes($force_employee_num);
	else $emp=$global_user->employee_num;
	$total=round($total,2);
	//echo "<hr>select * from zz_umc_fd_po_review_history where PO = '$po_number' and Employee = '$global_user->employee_num' and last_amount = $total";
	return(ms_getoneb("select * from zz_umc_fd_po_review_history with (NOLOCK) where PO = '$po_number' and Employee = '$emp' and last_amount = $total"));
	}

function po_history_update($po_number,$total,$force_employee_num="") {
	global $global_user,$po_proxy_for;
	$total=round(addslashes($total),2);
	if ($force_employee_num!="") $force_employee_num=addslashes($force_employee_num);
	else $force_employee_num=$global_user->employee_num;
	//echo "$po_number,$total,$force_employee_num<hr>";
	//if ($force_employee_num=="") $force_employee_num
	if ($po_proxy_for!="") {
		$po_proxy_for=addslashes($po_proxy_for);
		$po_proxy_for_info=getoneb("select * from contacts where contacts_id = '$po_proxy_for'");
		$employee_num=$po_proxy_for_info->employee_num;
		$proxying_employee_num=$global_user->employee_num;
		} else {
		$employee_num=$global_user->employee_num;
		$proxying_employee_num=$global_user->employee_num;
		}
	//if ($force_employee_num!="") $employee_num=addslashes($force_employee_num);
	$current=po_history_data($po_number,$total,$force_employee_num);
	if (!($current)) {
		//echo "<hr>insert into zz_umc_fd_po_review_history (PO,Employee,last_amount,proxying_Employee,first_reviewed) values ('$po_number','$force_employee_num',$total,'$proxying_employee_num',{fn now()})<hr>";exit;
		@mssql_query("insert into zz_umc_fd_po_review_history (PO,Employee,last_amount,proxying_Employee,first_reviewed) values ('$po_number','$force_employee_num',$total,'$proxying_employee_num',{fn now()})");
		} else {
		//echo "<hr>update zz_umc_fd_po_review_history set last_reviewed = { fn now() } where po_hist_id = '$current->po_hist_id'<hr>";exit;
		@mssql_query("update zz_umc_fd_po_review_history set last_reviewed = { fn now() } where po_hist_id = '$current->po_hist_id'");
		}
	return ($current);
	}

function contact_trade_view_old($employee_id) {
	$employee_info=getoneb("select * from contacts where contacts_id = '$employee_id'");
	if ($employee_info->umc_emp != 'Y') return (FALSE);
	$employee_id=$employee_info->contacts_id;
	if ($employee_info->employee_trade_description == "") update_trade($employee_id);
	}


function get_class_name_old($class,$udtrade) {
	$class=addslashes($class);
	$first_c=substr($class,0,1);
	$first2_c=substr($class,0,2);
	if ($udtrade=="Laborers") return("Laborers");
	if ($udtrade=="Operating Engineers") return("Operating Engineers");
	if ($class=="PURC") return ("Purchasing");
	if ($class=="PRAD") return ("Project Admin");
	if ($class=="PRMG") return ("Project Mgmt");
	switch ($first_c) {
		case "0":	
		case "1":	
		case "2":	
		case "3":	
		case "4":	
		case "5":	
		case "6":	
		case "7":	
		case "8":	
		case "9":	
		case "U":	
		case "H":
		case "h":
						$return="Apprentice";
						break;;
		case "J":
		case "j":
						$return="Journeymen";
						break;;
		case "F":
		case "f":
						$return="Foreman";
						break;;
		case "G":
		case "g":
						$return="General Foreman";
						break;;
		case "T":
		case "t":
						$return="Technicians";
						break;;
		default: 		
						$return="ERROR: ($class)";
						break;;
		}
	return($return);
	}

?>
