<?

function margin_to_markup($margin) {
	return (1 / ((1 - $margin/100)));
	}

function markup_to_margin($markup) {
	return (round((1-(1 / $markup))*10000))/100;
	}

function sync_with_proplog($agreement_id) {
	$agreement_info=load_agreement_info($agreement_id);
	if ($agreement_info->forced_total_value>0) $agreement_info->total_value=$agreement_info->forced_total_value;
	
	$total_hours=agreement_total_man_hours($agreement_info->agreement_id);
	$p_sales_person=$agreement_info->creator;
	$p_customer="$agreement_info->company_name";
	$p_description="$agreement_info->agreement_type - $agreement_info->years year(s)";
	$p_type="Service Agreement";
	$p_proposal_amount=round($agreement_info->total_value / $agreement_info->years);
	$p_projected_bill_date=$agreement_info->expected_start_date_mysql;
	$p_probability=$agreement_info->probability;
	$p_estimated_hours=round($total_hours / $agreement_info->years);
	$p_estimated_margin_percent=round(markup_to_margin($agreement_info->markup_percent));
	$p_status=$agreement_info->status;
	
	if ($agreement_info->svc_proposals_id >0) {
		$p_info=getone("select * from svc_proposals where svc_proposals_id = '$agreement_info->svc_proposals_id'");
		$p_proposal_date=$p_info->proposal_date;
		$p_proposal_daily_count=$p_info->proposal_daily_count;
		} else {
		$p_proposal_date=date('Y-m-d');
		$days_proposals_info=getone("select ifnull(max(proposal_daily_count),0) as daily_total from svc_proposals where sales_person = '$p_sales_person' and proposal_date = '$p_proposal_date'");
		$p_proposal_daily_count=$days_proposals_info->daily_total + 1;
		}
	
	if (($p_status!="Presenting")&&
		($p_status!="Lost")&&
		($p_status!="Dead")&&
		($p_status!="Won")) {
		die("ERROR: Application error! Function sync_with_proplog() called when agreement ($agreement_id) has invalid status for sync! Please contact your system administrator!");
		}
	
	if ($agreement_info->svc_proposals_id >0) {
		$query_start="update svc_proposals set ";
		$query_end=" where svc_proposals_id = '$agreement_info->svc_proposals_id'";
		} else {
		$query_start="insert into svc_proposals set ";
		$query_end="";
		}
	$query_middle="
	sales_person = '$p_sales_person',
	proposal_date = '$p_proposal_date',
	proposal_daily_count = '$p_proposal_daily_count',
	customer = '" . mysql_real_escape_string($p_customer) . "',
	description = '" . mysql_real_escape_string($p_description) . "',
	type = '$p_type',
	proposal_amount = '$p_proposal_amount',
	projected_bill_date = '$p_projected_bill_date',
	probability = '$p_probability',
	estimated_hours = '$p_estimated_hours',
	estimated_margin_percent = '$p_estimated_margin_percent',
	status = '$p_status' 
	";
	
	
	
	$full_query = $query_start . $query_middle . $query_end;
	$res=@mysql_query($full_query);
	if (!($res)) die("error with query: $full_query");
	if ($agreement_info->svc_proposals_id <=0) {
		$id=@mysql_insert_id();
		@mysql_query("update svc_agreement_index set svc_proposals_id = '$id' where agreement_id = '$agreement_info->agreement_id'");
		$agreement_info->svc_proposals_id=$id;
		}
	return($agreement_info->svc_proposals_id);
	}

function sync_psp_task($task_id,$to_type="ESP") {
	return TRUE;
	$psp_task=load_task_info($task_id);
	$esptmp=getone("select agreement_type_id from svc_agreement_types where agreement_type = '$to_type'");
	$psptmp=getone("select agreement_type_id from svc_agreement_types where agreement_type = 'PSP'");

	$espid=$esptmp->agreement_type_id;
	$pspid=$psptmp->agreement_type_id;

	if ($psp_task->agreement_type_id!=$pspid) return TRUE;

	if ($psp_task->export_psp_to_esp!='Y') return TRUE;

	$esp_task=getoneb("select task_id from svc_task_items where equip_type_id = '$psp_task->equip_type_id' and agreement_type_id = '$espid' and maint_type_id = '$psp_task->maint_type_id'");

	if ($to_type=="ISP") {
		$psp_task->required_times_a_year=0;
		}

	if (!($esp_task)) {
		$query="insert into svc_task_items set 
		equip_type_id = '$psp_task->equip_type_id', 
		agreement_type_id = '$espid', 
		maint_type_id = '$psp_task->maint_type_id', 
		recommended_times_a_year = '$psp_task->recommended_times_a_year',
		required_times_a_year = '$psp_task->required_times_a_year',
		approval_required_age = '$psp_task->approval_required_age',
		subs_required = '$psp_task->subs_required',
		predictive_svc_avail = '$psp_task->predictive_svc_avail',
		misc_fees = '$psp_task->misc_fees',
		export_psp_to_esp = '$psp_task->export_psp_to_esp',
		export_psp_to_esp_force = '$psp_task->export_psp_to_esp_force'";

		$res=@mysql_query($query);
		if (!($res)) die ('ERROR creating missing $to_type info! <p>Failed Query:<hr>' . $query);
		$new_esp_task_id=@mysql_insert_id();
		$esp_task=load_task_info($new_esp_task_id);
		$psp_task->export_psp_to_esp_force='Y';
		}

	if ($psp_task->export_psp_to_esp_force!='Y') return TRUE;
	
	if (!(compare_task_items($psp_task->task_id,$esp_task->task_id))) {
		$query="update svc_task_items set 
		equip_type_id = '$psp_task->equip_type_id', 
		agreement_type_id = '$espid', 
		maint_type_id = '$psp_task->maint_type_id', 
		recommended_times_a_year = '$psp_task->recommended_times_a_year',
		required_times_a_year = '$psp_task->required_times_a_year',
		approval_required_age = '$psp_task->approval_required_age',
		subs_required = '$psp_task->subs_required',
		predictive_svc_avail = '$psp_task->predictive_svc_avail',
		misc_fees = '$psp_task->misc_fees',
		export_psp_to_esp = '$psp_task->export_psp_to_esp',
		export_psp_to_esp_force = '$psp_task->export_psp_to_esp_force'
		
		where task_id = '$esp_task->task_id'
		";
		$res=@mysql_query($query);
		if (!($res)) die("ERROR: Error updating the $to_type record for this equipment!<p>Failed query:<hr>$query");
		$res=@mysql_query("delete from svc_task_hrs where task_id = '$esp_task->task_id'");
		if (!($res)) die("ERROR: Error deleting task hours from $to_type version during update!");
		$res=@mysql_query("select * from svc_task_hrs where task_id = '$psp_task->task_id'");
		if (!($res)) die("ERROR: Error loading task hours from PSP version for copying to $to_type version.");
		while ($row=@mysql_fetch_object($res)) {
			$ires=@mysql_query("insert into svc_task_hrs set task_id = '$esp_task->task_id', labor_type_id = '$row->labor_type_id', hrs = '$row->hrs'");
			if (!($ires)) die("ERROR: Error inserting new hrs into $to_type from PSP!");
			}
		}
	return TRUE;
	}

function compare_task_items($task_id_1,$task_id_2) {
	$task1=load_task_info($task_id_1);
	$task2=load_task_info($task_id_2);
	
	$same=TRUE;
	
	if ($task1->recommended_times_a_year!=$task2->recommended_times_a_year) $same=FALSE;
	if ($task1->required_times_a_year!=$task2->required_times_a_year) $same=FALSE;
	if ($task1->approval_required_age!=$task2->approval_required_age) $same=FALSE;
	if ($task1->subs_required!=$task2->subs_required) $same=FALSE;
	if ($task1->predictive_svc_avail!=$task2->predictive_svc_avail) $same=FALSE;
	if ($task1->misc_fees!=$task2->misc_fees) $same=FALSE;

	if ($task1->labor_type_1!=$task2->labor_type_1) $same=FALSE;
	if ($task1->labor_type_2!=$task2->labor_type_2) $same=FALSE;
	if ($task1->labor_type_3!=$task2->labor_type_3) $same=FALSE;
	if ($task1->labor_type_4!=$task2->labor_type_4) $same=FALSE;
	if ($task1->labor_type_5!=$task2->labor_type_5) $same=FALSE;
	if ($task1->labor_type_6!=$task2->labor_type_6) $same=FALSE;
	if ($task1->labor_type_7!=$task2->labor_type_7) $same=FALSE;
	if ($task1->labor_type_8!=$task2->labor_type_8) $same=FALSE;
	if ($task1->labor_type_9!=$task2->labor_type_9) $same=FALSE;
	if ($task1->labor_type_10!=$task2->labor_type_10) $same=FALSE;
	if ($task1->labor_type_11!=$task2->labor_type_11) $same=FALSE;
	if ($task1->labor_type_12!=$task2->labor_type_12) $same=FALSE;
	if ($task1->labor_type_13!=$task2->labor_type_13) $same=FALSE;
	if ($task1->labor_type_14!=$task2->labor_type_14) $same=FALSE;
	if ($task1->labor_type_15!=$task2->labor_type_15) $same=FALSE;
	if ($task1->labor_type_16!=$task2->labor_type_16) $same=FALSE;
	if ($task1->labor_type_17!=$task2->labor_type_17) $same=FALSE;
	if ($task1->labor_type_18!=$task2->labor_type_18) $same=FALSE;
	if ($task1->labor_type_19!=$task2->labor_type_19) $same=FALSE;
	if ($task1->labor_type_20!=$task2->labor_type_20) $same=FALSE;
	return ($same);
	}

function copy_task_list($source,$target) {
	if ($target=="") die("ERROR: copy_task_list failed: not \$target!");
	if ($source=="") die("ERROR: copy_task_list failed: not \$source!");
	$target=addslashes($target);
	$source=addslashes($source);
	delete_task_list($target);
	$query="select * from svc_task_lists where task_id = '$source'";
	$res=@mysql_query($query);
	while ($row=@mysql_fetch_object($res)) {
		$task_text=addslashes($row->task_text);
		mysql_query("insert into svc_task_lists set task_id = '$target', task_list_number = '$row->task_list_number', task_text = '$task_text'");
		}
	}

function delete_task_list($task_id) {
	if ($task_id=="") die("ERROR: invalid task_id used with delete_task_list() function");
	$task_id=addslashes($task_id);
	@mysql_query("delete from svc_task_lists where task_id = '$task_id'");
	}

function predict_nice_name($ugly) {
	$pdta=array(
	'predict_vibration'=>'Vibration Analysis',
	'predict_refrigerant_lp'=>'Refrigerant Analysis (LP)',
	'predict_refrigerant_hp'=>'Refrigerant Analysis (HP)',
	'predict_laser_alignment'=>'Laser Alignment',
	'predict_eddy_current_tube_anal'=>'Eddy Current Tube Analysis',
	'predict_thermography'=>'Thermography',
	'predict_combustion'=>'Combustion Gas Analysis',
	'predict_oil'=>'Oil Analysis'
	);
	$nice_name=$pdta[$ugly];
	if ($nice_name=="") $nice_name="UNKNOWN";
	
	return ($nice_name);
	}

function agreement_event_dates_by_month ($agreement_id,$start_date="",$months=0) {
	$ag_info=load_agreement_info($agreement_id);
	if ($start_date=="") $start_date=vali_date($ag_info->expected_start_date);
	$events=0;
	$event_dates=array();
	while(agreement_which_year($agreement_id,invali_date($start_date))) {
		$event_dates[$events]=$start_date;
		if ($months==0) return($event_dates);
		$start_date=date("Y-m-d",strtotime("$start_date +$months months"));
		$events++;
		}
	if ($events<1) return FALSE;
	return $event_dates;
	}

function copy_equipment($el_id,$copies=2,$new_agreement_id=0) {
	if ($copies<=1) return 0;
	//////////////////////////////////////////
	//////////////////////////////////////////
	// First the main equipment index
	//////////////////////////////////////////
	//////////////////////////////////////////
	$svc_agreement_equip_list=getone("select * from svc_agreement_equip_list where el_id = '$el_id'");
	if ($new_agreement_id==0) $target_agreement_id=$svc_agreement_equip_list->agreement_id;
	else $target_agreement_id=addslashes($new_agreement_id);
	$svc_agreement_equip_list->tag_num=addslashes($svc_agreement_equip_list->tag_num);
	$svc_agreement_equip_list->serial_num=addslashes($svc_agreement_equip_list->serial_num);
	$svc_agreement_equip_list->model_number=addslashes($svc_agreement_equip_list->model_number);
	$svc_agreement_equip_list->location=addslashes($svc_agreement_equip_list->location);
	@mysql_query("insert into svc_agreement_equip_list set 
				agreement_id = '$target_agreement_id',
				category_id = '$svc_agreement_equip_list->category_id',
				equip_type_id = '$svc_agreement_equip_list->equip_type_id',
				mfg = '$svc_agreement_equip_list->mfg',
				age = '$svc_agreement_equip_list->age',
				model_number = '$svc_agreement_equip_list->model_number',
				serial_num = '$svc_agreement_equip_list->serial_num',
				tag_num = '$svc_agreement_equip_list->tag_num',
				location = '$svc_agreement_equip_list->location'");
	$new_el_id=@mysql_insert_id();
	//////////////////////////////////////////
	//////////////////////////////////////////
	// Now I want to copy things from the
	// "equipment events" table...
	//////////////////////////////////////////
	//////////////////////////////////////////
	$events_list=@mysql_query("select * from svc_agreement_equipment_events where el_id = '$el_id'");
	while ($event_row=@mysql_fetch_object($events_list)) {
		$new_event_query="insert into svc_agreement_equipment_events set
			el_id = '$new_el_id',
			event_type = '$event_row->event_type',
			material_list_id = '$event_row->material_list_id',
			qty = '$event_row->qty',
			event_date = '$event_row->event_date',
			cost = '$event_row->cost',
			value = '$event_row->value',
			inflation = '$event_row->inflation',
			description = '$event_row->description',
			show_in_summary = '$event_row->show_in_summary'";
		@mysql_query($new_event_query);
		}
	//////////////////////////////////////////
	//////////////////////////////////////////
	// Next add the schedule indexes
	//////////////////////////////////////////
	//////////////////////////////////////////
	$sched_index_res=@mysql_query("select * from svc_equip_sched_index where el_id = '$el_id'");
	while ($row=@mysql_fetch_object($sched_index_res)) {
		$new_task_id=check_task_id($row->task_id,$target_agreement_id);
		$row->special_notes=addslashes($row->special_notes);
		@mysql_query("insert into svc_equip_sched_index set 
				el_id = '$new_el_id',
				task_id = '$new_task_id',
				start_date = '$row->start_date',
				times_a_year = '$row->times_a_year',
				special_notes = '$row->special_notes',
				base_cost = '$row->base_cost'");
		$new_sched_index_id=@mysql_insert_id();
		//////////////////////////////////////////
		//////////////////////////////////////////
		// Next add the svc_equip_sched_items
		//////////////////////////////////////////
		//////////////////////////////////////////
		$items_res=@mysql_query("select * from svc_equip_sched_items where sched_index_id = '$row->sched_index_id'");
		while ($item_info=@mysql_fetch_object($items_res)) {
			$item_info->maint_notes=addslashes($item_info->maint_notes);
			@mysql_query("insert into svc_equip_sched_items set
					sched_index_id = '$new_sched_index_id',
					maint_date = '$item_info->maint_date',
					maint_notes = '$item_info->maint_notes',
					cost = '$item_info->cost'");
			}
		//////////////////////////////////////////
		//////////////////////////////////////////
		// Next copy the materials for this index
		//////////////////////////////////////////
		//////////////////////////////////////////
		$materials_res=@mysql_query("select * from svc_agreement_materials where sched_index_id = '$row->sched_index_id'");
		while($material_info=@mysql_fetch_object($materials_res)) {
			$material_info->name=addslashes($material_info->name);
			$material_info->qty_name=addslashes($material_info->qty_name);
			@mysql_query("insert into svc_agreement_materials set
					sched_index_id = '$new_sched_index_id',
					material_list_id = '$material_info->material_list_id',
					name = '$material_info->name',
					qty_name = '$material_info->qty_name',
					qty = '$material_info->qty',
					cost = '$material_info->cost'");
			}
		}
	$copies=$copies - 1;
	if ($copies>1) copy_equipment($el_id,$copies,$new_agreement_id);
	return 0;
	}

function check_task_id($task_id,$agreement_id) {
	$ag_info=getone("select * from svc_agreement_index where agreement_id = '$agreement_id'");
	$task=getone("select * from svc_task_items where task_id = '$task_id'");
	$new_task_info=getone("select * from svc_task_items where
	equip_type_id = '$task->equip_type_id' and
	maint_type_id = '$task->maint_type_id' and
	agreement_type_id = '$ag_info->agreement_type_id'");
	return ($new_task_info->task_id);
	}


function delete_equipment($el_id) {
	//////////////////////////////////////////
	//////////////////////////////////////////
	// First the main equipment index
	//////////////////////////////////////////
	//////////////////////////////////////////
	$svc_agreement_equip_list=getone("select * from svc_agreement_equip_list where el_id = '$el_id'");
	@mysql_query("delete from svc_agreement_equip_list where el_id = '$el_id'");
	//////////////////////////////////////////
	// Now whack the "equipment events"
	//////////////////////////////////////////
	@mysql_query("delete from svc_agreement_equipment_events where el_id = '$el_id'");
	//////////////////////////////////////////
	// Next find the schedule indexes
	//////////////////////////////////////////
	$sched_index_res=@mysql_query("select * from svc_equip_sched_index where el_id = '$el_id'");
	while ($row=@mysql_fetch_object($sched_index_res)) {
		//////////////////////////////////////////
		// Delete materials and scheduled items
		//////////////////////////////////////////
		@mysql_query("delete from svc_equip_sched_items where sched_index_id = '$row->sched_index_id'");
		@mysql_query("delete from svc_agreement_materials where sched_index_id = '$row->sched_index_id'");
		}
	/////////////////////////////////////////
	// Delete the schedule index
	/////////////////////////////////////////
	@mysql_query("delete from svc_equip_sched_index where el_id = '$el_id'");
	return 0;
	}

function is_final_filter($material_list_id) {
	$material_info=getoneb("select * from svc_materials_list where material_list_id = '$material_list_id'"); 
	$limit_count=100;
	if 	(($material_info->type=="cat") && ($material_info->name=="Final Filter")) return TRUE;
	while (($material_info->material_list_id!=$material_info->material_list_parent)&&($limit_count > 0)) {
		$material_info=getoneb("select * from svc_materials_list where material_list_id = '$material_info->material_list_parent'");
		if 	(($material_info->type=="cat") && ($material_info->name=="Final Filter")) return TRUE;
		$limit_count=$limit_count - 1;
		}
	return FALSE;
	}

function is_filter($material_list_id) {
	$material_info=getoneb("select * from svc_materials_list where material_list_id = '$material_list_id'"); 
	$limit_count=100;
	if (is_final_filter($material_info->material_list_id)) return FALSE;
	
	while (($material_info->material_list_id!=$material_info->material_list_parent)&&($limit_count > 0)) {
		$material_info=getoneb("select * from svc_materials_list where material_list_id = '$material_info->material_list_parent'");
		$limit_count=$limit_count - 1;
		}
	if 	(($material_info->type=="cat") && ($material_info->name=="Filters")) return TRUE;
		else return FALSE;
	}


function is_belt($material_list_id) {
	$material_info=getoneb("select * from svc_materials_list where material_list_id = '$material_list_id'"); 
	$limit_count=100;
	
	while (($material_info->material_list_id!=$material_info->material_list_parent)&&($limit_count > 0)) {
		$material_info=getoneb("select * from svc_materials_list where material_list_id = '$material_info->material_list_parent'");
		$limit_count=$limit_count - 1;
		}
	if 	(($material_info->type=="cat") && ($material_info->name=="Belts")) return TRUE;
		else return FALSE;
	}


function agreement_tasks_query ($agreement_id) {
	return ("select task_id from svc_equip_sched_index left join svc_agreement_equip_list using (el_id) left join svc_equip_categories using (category_id) left join svc_equip_types on (svc_agreement_equip_list.equip_type_id = svc_equip_types.equip_type_id) 
	where 
	agreement_id = '$agreement_id' group by task_id order by category,type");
	}


function agreement_schedule_detail_query ($agreement_id) {
	return ("
	select * from svc_task_hrs,svc_equip_sched_index 
		left join svc_agreement_equip_list using (el_id) 
		left join svc_equip_categories using (category_id) 
		left join svc_equip_types on (svc_agreement_equip_list.equip_type_id = svc_equip_types.equip_type_id) 
		left join svc_equip_sched_items on (svc_equip_sched_index.sched_index_id = svc_equip_sched_items.sched_index_id)
		left join svc_task_items on (svc_task_items.task_id = svc_equip_sched_index.task_id)
		left join svc_agreement_types on (svc_agreement_types.agreement_type_id = svc_task_items.agreement_type_id)
		left join svc_maint_types on (svc_maint_types.maint_type_id = svc_task_items.maint_type_id)
		left join svc_labor_types on (svc_task_hrs.labor_type_id = svc_labor_types.labor_type_id)
		
	where agreement_id = '$agreement_id' and svc_task_hrs.task_id = svc_equip_sched_index.task_id
	order by category,type,svc_equip_sched_index.task_id ");
	}

function agreement_yearly_visits_based_on_schedule ($agreement_id) {
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	// This tries to estimate how many times any one 
	// piece of equipment will make us go out there in
	// a year. It finds the equipment with the highest
	// number of visits, and then returns that number.
	// It only really looks at operational plus annual
	// visits for now, but will probably need to be
	// modified later for other agreement types.
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	$ag_info=load_agreement_info($agreement_id);
	$visits=getoneb("
	select sum(1) as total from svc_equip_sched_index 
		left join svc_agreement_equip_list using (el_id) 
		left join svc_equip_categories using (category_id) 
		left join svc_equip_types on (svc_agreement_equip_list.equip_type_id = svc_equip_types.equip_type_id) 
		left join svc_equip_sched_items on (svc_equip_sched_index.sched_index_id = svc_equip_sched_items.sched_index_id)
		left join svc_task_items on (svc_task_items.task_id = svc_equip_sched_index.task_id)
		left join svc_agreement_types on (svc_agreement_types.agreement_type_id = svc_task_items.agreement_type_id)
		left join svc_maint_types on (svc_maint_types.maint_type_id = svc_task_items.maint_type_id)
		
	where agreement_id = '$agreement_id' and (maint_type = 'Annual' or maint_type = 'Operational')
	group by svc_equip_sched_index.el_id
	order by total desc,category,type,svc_equip_sched_index.task_id 
	limit 1");
	if (!($visits)) return FALSE;
	$actual_yearly=$visits->total / $ag_info->years;
	
	$rounded_yearly=round($actual_yearly);
	if ($rounded_yearly < $actual_yearly) return ($rounded_yearly + 1);
	else return ($rounded_yearly);
	}


function agreement_yearly_visits_based_on_hours ($agreement_id) {
	$ag_info=load_agreement_info($agreement_id);
	$agreement_hours=agreement_total_man_hours($ag_info->agreement_id);
	$actual_yearly_hours=$agreement_hours / $ag_info->years / 8;
	$rounded_yearly_hours=round($actual_yearly_hours);
	if ($rounded_yearly_hours < $actual_yearly_hours) return ($rounded_yearly_hours + 1);
	else return ($rounded_yearly_hours);
	}

function yearly_visits($agreement_id) {
	$visits_hours=agreement_yearly_visits_based_on_hours($agreement_id);
	$visits_schedule=agreement_yearly_visits_based_on_schedule($agreement_id);
	if ($visits_hours>$visits_schedule) return ($visits_hours);
	else return ($visits_schedule);
	}

function hours_per_visit($agreement_id) {
	$yearly_visits=yearly_visits($agreement_id);
	$agreement_info=load_agreement_info($agreement_id);
	$total_hours=agreement_total_man_hours($agreement_id);
	$yearly_hours=round($total_hours / $agreement_info->years);
	$hours_per_visit=@round($yearly_hours / $yearly_visits);
	return($hours_per_visit);
	}

function agreement_size_efficiency_multiplier($agreement_id,$summary=0) {
	global $worst_efficiency;
	$hpv=hours_per_visit($agreement_id);
	$remainder=$worst_efficiency - 1;
	$slices=$remainder / 7;
	$discount=$slices * ($hpv - 1);
	$mult=$worst_efficiency - $discount;
	if ($mult>$worst_efficiency) $mult=$worst_efficiency;
	if ($summary!=0) {
		$mult_rounded=(round($mult*1000)/1000);
		echo "<tr><td>Efficiency Factor:</td><td><font title=\"Based on $hpv hours per visit\">$mult_rounded</font></td></tr>";
		}
	if ($hpv>=8) return ("1.00");
	return ($mult);
	}

function agreement_total_man_hours ($agreement_id) {
	$total=getone("
	select sum(hrs) as total_hours from svc_task_hrs,svc_equip_sched_index 
		left join svc_agreement_equip_list using (el_id) 
		left join svc_equip_categories using (category_id) 
		left join svc_equip_types on (svc_agreement_equip_list.equip_type_id = svc_equip_types.equip_type_id) 
		left join svc_equip_sched_items on (svc_equip_sched_index.sched_index_id = svc_equip_sched_items.sched_index_id)
	where agreement_id = '$agreement_id' and svc_task_hrs.task_id = svc_equip_sched_index.task_id
	order by category,type,svc_equip_sched_index.task_id ");
	return ($total->total_hours);
	}

function agreement_customer_cost($agreement_id) {
	$ag_info=load_agreement_info($agreement_id);
	$umc_cost=agreement_cost($agreement_id);
	$customer_cost=clean_float(round($umc_cost * 100 * $ag_info->markup_percent) / 100);
	return($customer_cost);
	}

function agreement_cost($agreement_id,$summary=0) {
	global $show_totals;
	$cost="0.00";
	$el_list=@mysql_query("select * from svc_agreement_equip_list where agreement_id = '$agreement_id'");
	while($el_row=@mysql_fetch_object($el_list)) {
		$cost=equip_list_item_total_cost($el_row->el_id) + $cost;
		}
	if ($summary!=0) echo "<table border=1><tr><td>Total Equipment Costs:</td><td>$cost</td></tr>";
	$efficiency=agreement_size_efficiency_multiplier($agreement_id,$summary);
	$cost=$cost * $efficiency;
	$cost=(round($cost * 100))/100;
	if ($summary!=0) echo "<tr><td>Total after efficiency:</td><td>$cost</td></tr></table>";
	if ($show_totals=='Y') echo "<br><b>(in)efficiency factor:</b> $efficiency<br>";
	return ($cost);
	}

function equip_list_item_total_cost ($el_id,$summary=0) {
	global $esp_fudge_factor;
	$cost="0.00";
	if ($summary!=0) {
		$el_info=load_equip_info($el_id);
		echo "<b>$el_info->description: Tag #: $el_row->tag_num</b><hr>";
		echo "<table border=1 cellspacing=0 cellpadding=3>
		";
		}
	$el_list=@mysql_query("select * from svc_equip_sched_index where el_id = '$el_id'");
	while ($el_row=@mysql_fetch_object($el_list)) {
		if ($summary!=0) {
			$task_info=load_task_info($el_row->task_id);
			echo "
			<tr><td colspan=7 align=center><b><a target=_task_modify_win href=\"$pagename?mode=task_item_edit&task_id=$task_info->task_id\"><font color=black>$task_info->maint_type</font></a></b></td></tr>
			<tr bgcolor=#eeeeee><td>Labor</td><td>Hours</td><td>Misc</td><td>Materials</td><td>Inflation</td><td>Maint Date</td><td>Total</td></tr>
			";
			}
		$cost=equip_sched_index_cost($el_row->sched_index_id,$summary) + $cost;
		$task_id=$el_row->task_id;
		}
	$el_list=@mysql_query("select * from svc_agreement_equipment_events where el_id = '$el_id'");
	if ($summary!=0) {
		if (@mysql_num_rows($el_list)>0) {
			echo "<tr><td colspan=7 align=center><b>Misc</b></td></tr>
			<tr bgcolor=#eeeeee><td colspan=3>Description</td><td>Qty/Price</td><td>Inflation</td><td>Maint Date</td><td>Total</td></tr>
			";
			}
		}
	$event_costs=0;
	while ($el_row=@mysql_fetch_object($el_list)) {
		//$cost=equip_event_cost($el_row->el_ev_id,$summary) + $cost;
		$event_costs=equip_event_cost($el_row->el_ev_id,$summary) + $event_costs;
		}
	$cost=$cost + $event_costs;
	if ($summary!=0) echo "<tr><td colspan=6 align=right>Total</td><td>$event_costs</td></tr></table>";
	/////////////////////////////////////////////////////
	/////////////////////////////////////////////////////
	// Crank up the cost for ESP agreements
	/////////////////////////////////////////////////////
	/////////////////////////////////////////////////////
	$ag_type=el_agreement_type($el_id);
	$non_esp_cost=$cost;
	if ($summary!=0) {
		echo "<table border=1><tr><td>Base Cost:</td><td>$cost</td></tr>";
		}
	if ($ag_type=="ESP") {
		$cost=(round($cost * $esp_fudge_factor * 100)/100);
		if ($summary!=0) {
			echo "<tr><td>ESP Fudge Factor:</td><td>$esp_fudge_factor</td></tr>
			<tr><td>New Esp Cost:</td><td>$cost</td></tr>";
			}
		$eq_info=getone("select * from svc_agreement_equip_list where el_id = '$el_id'");
		$age=$eq_info->age;
		
		$allow='N';
		if ($eq_info->supervisor_approved=='Y') {
			$allow='Y';
			} else {
			$t_info=getone("select * from svc_task_items where task_id = '$task_id'");
			if ($t_info->approval_required_age > $age) $allow='Y';
			}
		if ($allow!='Y') die ("<p>ERROR: I'm sorry, but you need approval for some of the equipment on this agreement. Please talk to your administrator to get this agreement approved.<hr>Click <a href=javascript:history.go(-1)><font color=blue>here to go back</font></a> and try again.");
		
		$factor=$age - 4;
		if ($factor < 0) $factor=0;
		$mult=(.03 * $factor) + 1.01;
		if ("$mult"=="1.01") $mult=1.00;
		$cost=(round($cost * $mult * 100)/100);
		if ($summary!=0) {
			echo "<tr><td>Equipment Age:</td><td>$age</td></tr>
			<tr><td>ESP Age Multiplier:</td><td>$mult</td></tr>
			<tr><td>ESP aged cost:</td><td>$cost</td></tr>";
			}
		}
	if ($summary!=0) echo "<tr><td>Total</td><td>$cost</td></tr></table>";
	return ($cost);
	}

function el_agreement_type ($el_id) {
	$tmp=getoneb("select agreement_type from svc_agreement_equip_list left join svc_agreement_index using (agreement_id) left join svc_agreement_types using (agreement_type_id) where el_id = '$el_id'");
	return ($tmp->agreement_type);
	}

function equip_sched_index_cost($sched_index_id,$summary=0) {
	$cost="0.00";
	$query="select * from svc_equip_sched_items where sched_index_id = '$sched_index_id' group by sched_item_id";
	
	$res=@mysql_query($query);
	while ($row=@mysql_fetch_object($res)) {
		$newcost=equip_sched_cost($row->sched_item_id,$summary);
		$cost=$newcost + $cost;
		if ($summary!=0) {
			echo "<td>$row->maint_date</td><td>$newcost</td></tr>";
			}
		}
	if ($summary!=0) echo "<tr><td colspan=6 align=right>Total</td><td>$cost</td></tr>";
	return ($cost);
	}

function equip_sched_cost($sched_item_id,$summary=0) {
	global $inflation;
	$cost="0.00";
	$sched_item_info=getone("select * from svc_equip_sched_items where sched_item_id = '$sched_item_id'");
	$sched_index_info=getone("select * from svc_equip_sched_index where sched_index_id = '$sched_item_info->sched_index_id'");
	// Most of what we need is actually here, or attached to it..
	$task_info=getone("select * from svc_task_items where task_id = '$sched_index_info->task_id'");
	// Need this to get agreement_id
	$eq_info=getone("select * from svc_agreement_equip_list where el_id = '$sched_index_info->el_id'");
	$cost=$cost + $task_info->misc_fees;
	$labor_query="select 
		sum(hrs * st_rate) as total_st, 
		sum(hrs * ot_rate) as total_ot, 
		sum(hrs * dt_rate) as total_dt,
		sum(hrs) as total_hrs
	from svc_task_hrs left join svc_labor_types using (labor_type_id) 
		where task_id = '$task_info->task_id'
	group by task_id";
	$labor_cost=getoneb($labor_query);
	
	if ($labor_cost) $cost=$cost + $labor_cost->total_st;
	else die ("ERROR: labor_cost error. No labor!?!?! That must be a mistake.. Go get Rich or Jeff (debug for jeff: sched_item_id = $sched_item_id..");
	$materials_query="select sum(qty * cost) as total from svc_agreement_materials where sched_index_id = '$sched_item_info->sched_index_id' group by sched_index_id";
	$materials=getoneb($materials_query);
	if ($materials) $cost=$cost + $materials->total;
	if ($summary!=0) {
		echo "<tr><td>$labor_cost->total_st</td><td>$labor_cost->total_hrs</td>";
		if ($task_info->misc_fees!=0) echo "<td>$task_info->misc_fees</td>";
		else echo "<td>&nbsp;</td>";
		if ($materials->total!="") echo "<td>$materials->total</td>";
		else echo "<td>&nbsp;</td>";
		}
	/////////////////////////////////////////////
	// Add inflation based on what year it is.
	/////////////////////////////////////////////
	$cost_bi=$cost;
	$year=agreement_which_year($eq_info->agreement_id,$sched_item_info->maint_date);
	if ($year) {
		while ($year > 1) {
			$cost=round($cost * $inflation * 100) / 100;
			$year=$year - 1;
			}
		}
	if ($summary!=0) {
		$inflation_diff=$cost - $cost_bi;
		if ($inflation_diff!=0) echo "<td>$inflation_diff</td>";
		else echo "<td>&nbsp;</td>";
		//echo "<td>$cost</td>";
		}
	return ($cost);
	}

function equip_sched_hrs($sched_item_id) {
	global $inflation;
	$cost="0.00";
	$sched_item_info=getone("select * from svc_equip_sched_items where sched_item_id = '$sched_item_id'");
	$sched_index_info=getone("select * from svc_equip_sched_index where sched_index_id = '$sched_item_info->sched_index_id'");
	$task_info=getone("select * from svc_task_items where task_id = '$sched_index_info->task_id'");
	$eq_info=getone("select * from svc_agreement_equip_list where el_id = '$sched_index_info->el_id'");
	$cost=$cost + $task_info->misc_fees;

	$labor_query="select 
		sum(hrs) as total_hrs, hrs as task_hrs
	from svc_task_hrs where task_id = '$task_info->task_id'
	group by task_id";
	$labor_hours=getoneb($labor_query);
	
	//if ($labor_hours) return (ereg_replace('.00$','',$labor_hours->total_hrs));
	if ($labor_hours) return (ereg_replace('.00$','',$labor_hours->task_hrs));
	else die ("ERROR: labor_hours error. No labor hours!?!?! That must be a mistake.. Go get Rich or Jeff..");
	}

function equip_sched_materials_only_cost($sched_item_id) {
	global $inflation;
	$cost="0.00";
	$sched_item_info=getone("select * from svc_equip_sched_items where sched_item_id = '$sched_item_id'");
	$sched_index_info=getone("select * from svc_equip_sched_index where sched_index_id = '$sched_item_info->sched_index_id'");
	$eq_info=getone("select * from svc_agreement_equip_list where el_id = '$sched_index_info->el_id'");

	$materials_query="select sum(qty * cost) as total from svc_agreement_materials where sched_index_id = '$sched_item_info->sched_index_id' group by sched_index_id";
	$materials=getoneb($materials_query);
	if ($materials->total=="") $materials->total=0.00;
	$cost=$materials->total;

	/////////////////////////////////////////////
	// Add inflation based on what year it is.
	/////////////////////////////////////////////
	$year=agreement_which_year($eq_info->agreement_id,$sched_item_info->maint_date);
	if ($year) {
		while ($year > 1) {
			$cost=round($cost * $inflation * 100) / 100;
			$year=$year - 1;
			}
		}
	return ($cost);
	}

function equip_sched_materials_and_misc_only_cost($sched_item_id) {
	global $inflation;
	$cost="0.00";
	$sched_item_info=getone("select * from svc_equip_sched_items where sched_item_id = '$sched_item_id'");
	$sched_index_info=getone("select * from svc_equip_sched_index where sched_index_id = '$sched_item_info->sched_index_id'");
	$eq_info=getone("select * from svc_agreement_equip_list where el_id = '$sched_index_info->el_id'");

	$materials_query="select sum(qty * cost) as total from svc_agreement_materials where sched_index_id = '$sched_item_info->sched_index_id' group by sched_index_id";
	$materials=getoneb($materials_query);
	if ($materials->total=="") $materials->total=0;
	$cost=$materials->total;

	
	
	return ($cost);
	}

function equip_event_cost($el_ev_id,$summary=0) {
	global $inflation;
	$el_ev_info=getone("select * from svc_agreement_equipment_events where el_ev_id = '$el_ev_id'");
	$cost=$el_ev_info->cost;
	
	$cost_bq=$cost;
	if ($el_ev_info->event_type=="materials") {
		$cost=$cost * $el_ev_info->qty;
		}
	$cost_bi=$cost;
	if ($el_ev_info->inflation=='Y') {
		$eq_info=getone("select * from svc_agreement_equip_list where el_id = '$el_ev_info->el_id'");
		$year=agreement_which_year($eq_info->agreement_id,$el_ev_info->event_date);
		if ($year) {
			while ($year > 1) {
				$cost=round($cost * $inflation * 100) / 100;
				$year=$year - 1;
				}
			}
		}
	$inflation_diff=$cost - $cost_bi;
	@mysql_query("update svc_agreement_equipment_events set value = '$cost' where el_ev_id = '$el_ev_id'");
	if ($summary!=0) {
		$tmpqty=$el_ev_info->qty;
		if (($tmpqty=="")||($tmpqty<1)) $tmpqty=1;
		echo "<tr><td colspan=3 bgcolor=#eeeeee>$el_ev_info->description</td>
		<td>$tmpqty @ $el_ev_info->cost</td>
		<td>$inflation_diff</td>
		<td>$el_ev_info->event_date</td>
		<td>$cost</td>
		</tr>";
		}
	return ($cost);
	}

function agreement_which_year($agreement_id,$date) {
	$agreement_info=getone("select * from svc_agreement_index where agreement_id = '$agreement_id'");
	$date=vali_date($date);
	$ag_date=vali_date($agreement_info->expected_start_date);
	$term=$agreement_info->years;
	while ($term > 0) {
		$end_of_year=date('Y-m-d',strtotime("$ag_date + $term years"));
		$term=$term - 1;
		$start_of_year=date('Y-m-d',strtotime("$ag_date + $term years"));
		if (($date >= $start_of_year) && ($date < $end_of_year)) return ($term +1);
		}
	return FALSE;
	}

function agreement_year_dates($agreement_id,$year=1) {
	$agreement_info=load_agreement_info($agreement_id);
	$agreement_info->expected_start_date=vali_date($agreement_info->expected_start_date);
	$year=$year - 1;
	if ($year >= $agreement_info->years) return FALSE;
	if ($year < 0) return FALSE;
	$year_info=new bsclass;
	$year_info->start=date('Y-m-d',strtotime("$agreement_info->expected_start_date +$year years"));
	$year=$year + 1;
	$end=date('Y-m-d',strtotime("$agreement_info->expected_start_date +$year years"));
	$year_info->end=date('Y-m-d',strtotime("$end -1 days"));
	return ($year_info);
	}

function agreement_summary_box($agreement_id,$ro=0) {
	global $fd_color_3;
	$agreement_info=load_agreement_info($agreement_id);
	if ($ro!=0) $bit_add=0;
	else $bit_add=0;
	echo "
	<table align=center border=1 cellspacing=0 cellpadding=2><tr><td bgcolor=#aaccff>
	<table border=1 cellspacing=0 cellpadding=2 bgcolor=#eeeeee>
	<tr><td colspan=2 bgcolor=#ffffff>
		<b>$agreement_info->agreement_name</b>
	</td></tr>";
	
	if ($agreement_info->agreement_printed_title!="") {
		echo "<tr bgcolor=$fd_color_3><td colspan=2>
				<b>Printed Title: $agreement_info->agreement_printed_title</b>
			</td></tr>";
		}
	
	echo "
	<tr><td bgcolor=#aaccff>
		Status:&nbsp;&nbsp;<b>$agreement_info->status</b>
	</td><td align=center>
		Type:&nbsp;&nbsp;&nbsp;$agreement_info->agreement_type
	</td></tr>
	
	<tr><td>
		Company
	</td><td>";
		contactsbox("",$agreement_info->company_id,"company_id","",1);echo "
	</td></tr>
	
	<tr><td>
		Site
	</td><td>";
		contactsbox("",$agreement_info->site_id,"site_id","",1);echo "
	</td></tr>
	
	<tr><td>
		Contact
	</td><td>";
		contactsbox("",$agreement_info->customer_contact_id,"cust_contact_id","",1);echo "
	</td></tr>
	
	<tr><td>
		Term:&nbsp;$agreement_info->years
	</td><td align=right>
		Equipment&nbsp;Count:&nbsp;&nbsp;&nbsp;$agreement_info->equipment_count
	</td></tr>
	
	</table>
	</td></tr></table>
	";
	}

function materials_item_info ($materials_id) {
	$materials_info=getone("select * from svc_agreement_materials where materials_id = '$materials_id'");
	$materials_info->total_cost=$materials_info->cost * $materials_info->qty;
	return ($materials_info);	
	}

function material_info ($material_list_id,$event_interface=0) {
	global $materials_id,$sched_index_id,$el_id;
	if ($material_list_id=="") return "";
	$mat_info=getone("select * from svc_materials_list where material_list_id = '$material_list_id'");
	$mat_info->parents_link="";
	$added="";
	while($tmpmat_info=getoneb("select * from svc_materials_list where material_list_id = '$material_list_id'")) {
		if ($mat_info->type=="cat") {
			if ($event_interface==0) {
				$mat_info->parents_link="<a href=$pagename?mode=lists_material_types&el_id=$el_id&material_list_id=$tmpmat_info->material_list_id><font color=blue>$tmpmat_info->name</font></a>$added$mat_info->parents_link";
				$mat_info->materials_browse_link="<a href=$pagename?mode=svc_agreement_material_edit&el_id=$el_id&sched_index_id=$sched_index_id&materials_id=$materials_id&material_list_id=$tmpmat_info->material_list_id><font color=blue>$tmpmat_info->name</font></a>$added$mat_info->materials_browse_link";
				} else {
				$mat_info->parents_link="<a href=$pagename?mode=svc_agreement_equipment_event_add_materials&el_id=$el_id&material_list_id=$tmpmat_info->material_list_id><font color=blue>$tmpmat_info->name</font></a>$added$mat_info->parents_link";
				$mat_info->materials_browse_link="<a href=$pagename?mode=svc_agreement_equipment_event_add_materials&el_id=$el_id&sched_index_id=$sched_index_id&materials_id=$materials_id&material_list_id=$tmpmat_info->material_list_id><font color=blue>$tmpmat_info->name</font></a>$added$mat_info->materials_browse_link";
				}
		} else {
			if ($event_interface==0) {
				$mat_info->parents_link="$tmpmat_info->name$added$mat_info->parents_link";
				$mat_info->materials_browse_link="<a href=$pagename?mode=svc_agreement_equipment_event_add_materials&el_id=$el_id&sched_index_id=$sched_index_id&materials_id=$materials_id&material_list_id=$tmpmat_info->material_list_id><font color=blue>$tmpmat_info->name</font></a>$added$mat_info->materials_browse_link";
				} else {
				$mat_info->parents_link="$tmpmat_info->name$added$mat_info->parents_link";
				$mat_info->materials_browse_link="<a href=$pagename?mode=svc_agreement_equipment_event_add_materials&el_id=$el_id&sched_index_id=$sched_index_id&materials_id=$materials_id&material_list_id=$tmpmat_info->material_list_id><font color=blue>$tmpmat_info->name</font></a>$added$mat_info->materials_browse_link";
				}
		}
		$added=":";
		if ($tmpmat_info->material_list_id==$tmpmat_info->material_list_parent) break;
		$mat_info->root_name="$tmpmat_info->name";

		$material_list_id=$tmpmat_info->material_list_parent;
		}
	if ($event_interface==0) $mat_info->parents_link="<a href=$pagename?mode=lists_material_types><font color=blue>Top</font></a>$added$mat_info->parents_link";
	else $mat_info->parents_link="<a href=$pagename?mode=svc_agreement_equipment_event_add_materials&el_id=$el_id><font color=blue>Top</font></a>$added$mat_info->parents_link";
	
	if ($event_interface==0) $mat_info->materials_browse_link="<a href=$pagename?mode=svc_agreement_material_edit&el_id=$el_id&materials_id=$materials_id&sched_index_id=$sched_index_id&clear_materials_list_id=Y><font color=blue>Top</font></a>$added$mat_info->materials_browse_link";
	else $mat_info->materials_browse_link="<a href=$pagename?mode=svc_agreement_equipment_event_add_materials&el_id=$el_id&materials_id=$materials_id&sched_index_id=$sched_index_id&clear_materials_list_id=Y><font color=blue>Top</font></a>$added$mat_info->materials_browse_link";
	return($mat_info);
	}


function pdf_scope_of_service($agreement_type_id,$pdf_object) {
	switch ($agreement_type_id) {
		case 1:  return pdf_scope_of_service_1($pdf_object); break;
		case 2:  return pdf_scope_of_service_2($pdf_object); break;
		case 3:  return pdf_scope_of_service_3($pdf_object); break;
		default: return pdf_scope_of_service_0($pdf_object); break;
		}
	}

function pdf_title_side_text($title,$text,$pdf_object,$titlesize="14",$textsize="8") {
	$tablewidth=$pdf_object->ez['pageWidth'] - $pdf_object->ez['rightMargin'] - $pdf_object->ez['leftMargin'];
	$data=array(array('one'=>"<b>$title</b>",'two'=>"$text"));
	$pdf_object->ezTable($data,"","",array('showLines'=>"0",'showHeadings'=>'0','width'=>"$tablewidth",'cols'=>array('one'=>array('width'=>"175",'fontSize'=>"$titlesize"),'two'=>array('fontsize'=>'$textsize'))));
	return($pdf_object);
	}

function pdf_scope_of_service_0($pdf_object) {
	///////////////////////////
	// No Scope Service Plan //
	///////////////////////////
	$pdf_object->ezNewPage();
	$pdf_object->ezSetMargins(72,72,72,72);
	$pdf_object=pdf_page_title("SCOPE OF SERVICE",$pdf_object);
	$pdf_object->ezText("\n<UNDEFINED> SERVICE PROGRAM",16,array('justification'=>'center'));
	$pdf_object->ezText("\nScope of service text goes here...\n\n",10,array('justification'=>'center'));
	return ($pdf_object);
	}

function pdf_scope_of_service_1($pdf_object) {
	//////////////////////
	// PSP Service Plan //
	//////////////////////
	$pdf_object->ezNewPage();
	$pdf_object->ezSetMargins(72,72,72,72);
	$pdf_object=pdf_page_title("SCOPE OF SERVICE",$pdf_object);
	$pdf_object->ezText("\nPREVENTATIVE SERVICE PROGRAM",16,array('justification'=>'center'));
	
	$pdf_object->ezText("\nUniversity Mechanical Contractors - Service Group will perform the following services:\n\n",10,array('justification'=>'center'));
	$pdf_object->ezHR();
	$pdf_object=pdf_title_side_text("Scheduled Maintenance:","Preventative maintenance services will be provided as recommended by the original equipment manufacturer and as outlined in the attached tasking.",$pdf_object);
	$pdf_object->ezHR();
	$pdf_object=pdf_title_side_text("Replacement Parts:","Replacement parts & material will be provided as authorized and invoiced in addition to the agreement. All parts will be shipped regular freight unless customer authorizes premium freight.",$pdf_object);
	$pdf_object->ezHR();
	$pdf_object=pdf_title_side_text("Emergency Service:","Emergency service is available 24 hours a day, 365 days per year and will be invoiced in addition to the agreement. Normal business hours are 7:00 a.m. to 3:30 p.m. Calls responded to outside of these hours will be invoiced at overtime rates.",$pdf_object);
	$pdf_object->ezHR();
	$pdf_object=pdf_title_side_text("Preferred Rates:","This agreement provides for preferred customer rates for any additional repair work performed on covered equipment.",$pdf_object);
	$pdf_object->ezHR();
	$pdf_object=pdf_title_side_text("Preferential Services:","This agreement provides preferential service and dispatch priority over non-agreement customers.\n\nThis agreement also provides you with preferred customer rates for the following additional services:\n   *   Plumbing\n   *   Drain Cleaning\n   *   Backflow Certification",$pdf_object);

	return ($pdf_object);
	}

function pdf_scope_of_service_2($pdf_object) {
	//////////////////////
	// ESP Service Plan //
	//////////////////////
	$pdf_object->ezNewPage();
	$pdf_object->ezSetMargins(72,72,72,72);
	$pdf_object=pdf_page_title("SCOPE OF SERVICE",$pdf_object);
	$pdf_object->ezText("\nEXTENDED SERVICE PROGRAM",16,array('justification'=>'center'));
	
	$pdf_object->ezText("\nUniversity Mechanical Contractors - Service Group will perform the following services:\n\n",10,array('justification'=>'center'));
	$pdf_object->ezHR();
	$pdf_object=pdf_title_side_text("Scheduled Maintenance:","Preventative maintenance services will be provided as recommended by the original equipment manufacturer and as outlined in the attached tasking.",$pdf_object);
	$pdf_object->ezHR();
	$pdf_object=pdf_title_side_text("Replacement Parts:","The Extended Service Program provides for replacement parts on all covered equipment. Regular freight is included in the agreement.",$pdf_object);
	$pdf_object->ezHR();
	$pdf_object=pdf_title_side_text("Repair Labor:","The Extended Service Program provides for repair labor for all covered equipment.",$pdf_object);
	$pdf_object->ezHR();
	$pdf_object=pdf_title_side_text("Emergency Service:","The Extended Service Program provides for emergency service 24 hours a day, 365 days per year. Extended Service Program customers are responsible for only the differential between straight time and overtime if a call is required during hours other than UMC's normal working hours.",$pdf_object);
	$pdf_object->ezHR();
	$pdf_object=pdf_title_side_text("Preferred Rates:","This agreement provides for preferred customer rates for additional repair work performed on covered equipment.",$pdf_object);
	$pdf_object->ezHR();
	$pdf_object=pdf_title_side_text("Preferential Services:","This agreement provides preferential service and dispatch priority over non-agreement customers, and other customers with less than an Extended Service Program.\n\nThis agreement also provides you with preferred customer rates for the following additional services:\n   *   Plumbing\n   *   Drain Cleaning\n   *   Backflow Certification",$pdf_object);

	$pdf_object->ezHR();
	$pdf_object=pdf_title_side_text("Refrigerant:","The Extended Service Program provides for refrigerant for covered equipment. The refrigerant provided is up to 100% of the original design charge.",$pdf_object);

	// This line swapped out by Jeff Buck specifically for the BCH proposal on 01-16-2007
	// ...this is a temporary change requested by Rich Happel
	//$pdf_object->ezHR();
	//$pdf_object=pdf_title_side_text("Replacement Equipment:","Not included.",$pdf_object);

	//$pdf_object->ezHR();
	//$pdf_object=pdf_title_side_text("Replacement Equipment:","The Extended Service Program provides for replacement of covered equipment should it be determined by UMC that any covered equipment is no longer maintainable or repairable.",$pdf_object);

	// New POST BCH line
	$pdf_object->ezHR();
	$pdf_object=pdf_title_side_text("Replacement Equipment:","The Extended Service Program does not provide for replacement equipment.",$pdf_object);

	return ($pdf_object);
	}

function pdf_scope_of_service_3($pdf_object) {
	//////////////////////
	// ISP Service Plan //
	//////////////////////
	$pdf_object->ezNewPage();
	$pdf_object->ezSetMargins(72,72,72,72);
	$pdf_object=pdf_page_title("SCOPE OF SERVICE",$pdf_object);


	$pdf_object->ezText("\nINSPECTION SERVICE PROGRAM",16,array('justification'=>'center'));

	$pdf_object->ezText("\nUniversity Mechanical Contractors - Service Group will perform the following services:\n\n",10,array('justification'=>'center'));
	$pdf_object->ezHR();
	$pdf_object=pdf_title_side_text("Scheduled Maintenance:","Preventative maintenance services will be provided as requested by the customer and as outlined in the attached tasking.",$pdf_object);
	$pdf_object->ezHR();
	$pdf_object=pdf_title_side_text("Replacement Parts:","Replacement parts & material will be provided as authorized and invoiced in addition to the agreement. All parts will be shipped regular freight unless customer authorizes premium freight.",$pdf_object);
	$pdf_object->ezHR();
	$pdf_object=pdf_title_side_text("Emergency Service:","Emergency service is available 24 hours a day, 365 days per year. Normal business hours are 7:00 a.m. to 3:30 p.m. Calls responded to outside of these hours are billed at overtime rates.",$pdf_object);
	$pdf_object->ezHR();
	$pdf_object=pdf_title_side_text("Preferred Rates:","This agreement provides for preferred customer rates for additional repair work performed on covered equipment.",$pdf_object);
	$pdf_object->ezHR();
	$pdf_object=pdf_title_side_text("Preferential Services:","This agreement provides preferential service and dispatch priority over non-agreement customers.\n\nThis agreement also provides you with preferred customer rates for the following additional services:\n   *   Plumbing\n   *   Drain Cleaning\n   *   Backflow Certification",$pdf_object);

	return ($pdf_object);
	}

function pdf_equip_list_year_header($pdf_object,$added_text="",$size=10) {
	$pdf_object->ezText("$added_text",$size);
	$current_y=$pdf_object->y;
	$current_year=1;
	while($current_year <= 5) {
		$pdf_object->addText((($pdf_object->sam_yearwidth * ($current_year - 1)) + $pdf_object->sam_yeartab),$current_y,$pdf_object->sam_yearsize,"Y$current_year");
		$current_year++;
		}
	$pdf_object->ezHR();
	return ($pdf_object);
	}

function pdf_equip_list_type_show($agreement_info,$pdf_object,$type_object) {
		$pdf_object->sam_yeartab=482;
		$pdf_object->sam_yearwidth=20;
		$pdf_object->sam_yearsize=10;
		$pdf_object->sam_spacing=".8";
		$pdf_object->setStrokeColor(0,0,0);
		$pdf_object=pdf_equip_list_year_header($pdf_object,"<b>($type_object->total_count) $type_object->type</b>");
		//$typelist=@mysql_query("
		//select * from svc_agreement_equip_list 
		//left join svc_equip_types 
		//	using(equip_type_id) 
		//left join svc_equip_categories 
		//	using (category_id) 
		//where agreement_id = '$agreement_info->agreement_id' and svc_agreement_equip_list.equip_type_id = '$type_object->equip_type_id' order by tag_num");
		
		//$typelist=@mysql_query("
		//select * from svc_agreement_equip_list 
		//left join svc_equip_types 
		//	using(svc_agreement_equip_list.equip_type_id = svc_equip_types.equip_type_id) 
		//left join svc_equip_categories 
		//	using (svc_agreement_equip_list.category_id = svc_equip_categories.category_id) 
		//where agreement_id = '$agreement_info->agreement_id' and svc_agreement_equip_list.equip_type_id = '$type_object->equip_type_id' order by tag_num");
		
		$typelist=@mysql_query("
		select * from svc_agreement_equip_list 
		left join (svc_equip_types,svc_equip_categories) 
			on (svc_agreement_equip_list.equip_type_id = svc_equip_types.equip_type_id and svc_agreement_equip_list.category_id = svc_equip_categories.category_id) 
		where agreement_id = '$agreement_info->agreement_id' and svc_agreement_equip_list.equip_type_id = '$type_object->equip_type_id' order by tag_num");
		

		$shaded=1;
		while($typerow=@mysql_fetch_object($typelist)) {
			$pdf_object=pdf_equip_list_detail_show($agreement_info,$pdf_object,$typerow);
			}
	return ($pdf_object);
	}

function pdf_equip_item_shade_box_if_required($pdf_object) {
	$unrounded=$pdf_object->right_side_row / 2;
	$rounded=round($unrounded);
	if ($rounded==$unrounded) $even=TRUE;
	if (($even==TRUE)&&($pdf_object->right_side_row>3)) $pdf_object=pdf_equip_item_shade_box($pdf_object);
	$pdf_object->right_side_row++;
	return ($pdf_object);
	}

function pdf_equip_item_shade_box($pdf_object) {
	$tempy=$pdf_object->y;
	$pdf_object->ezText(" ");
	$pdf_object->setColor(.9,.9,.9);
	$pdf_object->filledRectangle(
	($pdf_object->ez['leftMargin'] + 5),
	($pdf_object->y + 1),
	($pdf_object->ez['pageWidth'] - $pdf_object->ez['leftMargin'] - ($pdf_object->ez['rightMargin'] + 5)
	),10);
	$pdf_object->setColor(0,0,0);
	$pdf_object->ezSetY($tempy);
	return ($pdf_object);
	}

function pdf_equip_list_detail_show($agreement_info,$pdf_object,$typerow) {
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	// Print the main equipment info on the left side
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	$pdf_object->sam_filtercount=array();
	$pdf_object->sam_filtercount_misc=array();
	$pdf_object->sam_beltcount=array();
	$starting_y=$pdf_object->y;
	$current_page=$pdf_object->ezPageCount;
	$pdf_object->transaction('start');
	
	$mfg_info=getoneb("select display_name from contacts where contacts_id = '$typerow->mfg'");
	
	$pdf_object=pdf_equip_item_shade_box($pdf_object);
	$pdf_object->ezText("  Mfr/Model:","",array("spacing"=>"$pdf_object->sam_spacing"));
	$pdf_object->addText(($pdf_object->ez['leftMargin'] + 70),$pdf_object->y,$pdf_object->sam_yearsize,"$mfg_info->display_name - $typerow->model_number",0,-2);
	
	$pdf_object->ezText("  Serial #:","",array("spacing"=>"$pdf_object->sam_spacing"));
	$pdf_object->addText(($pdf_object->ez['leftMargin'] + 70),$pdf_object->y,$pdf_object->sam_yearsize,"$typerow->serial_num",0,-2);

	$pdf_object=pdf_equip_item_shade_box($pdf_object);
	$pdf_object->ezText("  Tag #:","",array("spacing"=>"$pdf_object->sam_spacing"));
	$pdf_object->addText(($pdf_object->ez['leftMargin'] + 70),$pdf_object->y,$pdf_object->sam_yearsize,"$typerow->tag_num",0,-2);
	
	$pdf_object->ezText("  Site/Location:","",array("spacing"=>"$pdf_object->sam_spacing"));
	$pdf_object->addText(($pdf_object->ez['leftMargin'] + 70),$pdf_object->y,$pdf_object->sam_yearsize,"$typerow->location",0,-2);
	
	$pdf_object->ezText(" ");
	$min_end_y=$pdf_object->y;

	$current_page_new=$pdf_object->ezPageCount;
	if ($current_page==$pdf_object->ezPageCount) {
		$pdf_object->transaction('commit');
		} else {
		$pdf_object->transaction('abort');
		$pdf_object->ezNewPage();
		$pdf_object=pdf_equip_list_year_header($pdf_object,"$typerow->type continued...");
		$pdf_object=pdf_equip_list_detail_show($agreement_info,$pdf_object,$typerow);
		return ($pdf_object);
		}
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	// Print the yearly maintenance type visit counts
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	$pdf_object->ezSetY($starting_y);
	$pdf_object->right_side_row=0;
	$the_query="select svc_maint_types.maint_type,svc_maint_types.maint_type_id,svc_equip_sched_index.task_id,svc_equip_sched_index.sched_index_id,svc_agreement_equip_list.el_id from svc_agreement_equip_list,svc_equip_sched_index,svc_task_items,svc_maint_types where svc_agreement_equip_list.el_id = svc_equip_sched_index.el_id and agreement_id = '$agreement_info->agreement_id' and svc_task_items.task_id = svc_equip_sched_index.task_id and svc_maint_types.maint_type_id = svc_task_items.maint_type_id and svc_agreement_equip_list.el_id = '$typerow->el_id' group by svc_equip_sched_index.task_id order by maint_type";
	$maint_types_res=@mysql_query($the_query);
	while($maint_type=@mysql_fetch_object($maint_types_res)) {
		$pdf_object=pdf_equip_list_maint_row_show($agreement_info,$pdf_object,$maint_type);
		$last_el_id=$maint_type->el_id;
		}
	if ($pdf_object->sam_filtercount['found']) {
		$pdf_object=pdf_equip_list_filter_row_show($agreement_info,$pdf_object,$last_el_id);
		}
	if ($pdf_object->sam_beltcount['found']) {
		$pdf_object=pdf_equip_list_belt_row_show($agreement_info,$pdf_object,$last_el_id);
		}
	
	////////////////////////////////////////////////////
	// Misc Items are going to go here.. 
	////////////////////////////////////////////////////
	$pdf_object=pdf_equip_list_misc_events($agreement_info,$pdf_object,$typerow->el_id);

	if ($pdf_object->y > ($min_end_y+11)) $pdf_object->ezSetY($min_end_y +11);
	$ending_y=$pdf_object->y;
	if ($pdf_object->right_side_row<4) $pdf_object->ezSetY($min_end_y - $pdf_object->yearsize);
	else {
		$pdf_object->ezText(" ","",array("spacing"=>"$pdf_object->sam_spacing"));
		///////////////////////////////////////////////////////////////
		// If we ended the misc events stuff on a row that was shaded
		// dark, then add a little more space down, just to make it
		// look a little better, and more seperate from the next item
		///////////////////////////////////////////////////////////////
		$unrounded=$pdf_object->right_side_row / 2;
		$rounded=round($unrounded);
		if ($rounded!=$unrounded) $pdf_object->ezSetDY(-3);
		}
	$space_width=$pdf_object->getTextWidth($pdf_object->ez['fontSize'],"  ");
	$height=$starting_y - ($ending_y - 2);
	$width=$pdf_object->ez['pageWidth'] - ($pdf_object->ez['rightMargin'] + $pdf_object->ez['leftMargin'] + $space_width) + 1;
	$pdf_object->setStrokeColor(0,0,0);
	
	if ($pdf_object->sam_executive_summary) {
		$total_cost=equip_list_item_total_cost($typerow->el_id);
		$pdf_object->setColor(1,0,0);
		$pdf_object->addText($pdf_object->ez['leftMargin'],$pdf_object->y - 2,$pdf_object->ez['fontSize'],"Total Cost (full agreement): $total_cost\r\n\r\n");
		$pdf_object->setColor(0,0,0);
		$pdf_object->ezSetDY(-5);
		}
	return ($pdf_object);
	}

function pdf_equip_list_maint_row_show($agreement_info,$pdf_object,$maint_type) {
	$pdf_object=pdf_equip_item_shade_box_if_required($pdf_object);
	$pdf_object->ezText(" ","",array("spacing"=>"$pdf_object->sam_spacing"));
	$current_y=$pdf_object->y;
	$current_year=1;
	
	if ($pdf_object->sam_beltcount[$current_year]=="") $pdf_object->sam_beltcount[$current_year]=0;
	if ($pdf_object->sam_filtercount[$current_year]=="") $pdf_object->sam_filtercount[$current_year]=0;
	if ($pdf_object->sam_filtercount_misc[$current_year]=="") $pdf_object->sam_filtercount_misc[$current_year]=0;
	while($current_year <= $agreement_info->years) {
		$lr_loc=$pdf_object->sam_yeartab - 10 - ($pdf_object->getTextWidth($pdf_object->sam_yearsize,$maint_type->maint_type));
		$pdf_object->addText($lr_loc,$current_y,$pdf_object->sam_yearsize,"$maint_type->maint_type",0,-2);
		$year_dates=agreement_year_dates($agreement_info->agreement_id,$current_year);
		$visit_count=getoneb("select sum(1) as total from svc_equip_sched_items where sched_index_id = '$maint_type->sched_index_id' and maint_date >= '$year_dates->start' and maint_date <= '$year_dates->end'");
		if ($visit_count->total > 0) {
			$res=@mysql_query("select * from svc_agreement_materials where sched_index_id = '$maint_type->sched_index_id' group by materials_id order by material_list_id");
			$beltfound=0;
			$filterfound=0;
			while ($row=@mysql_fetch_object($res)) {
				if (is_filter($row->material_list_id)) {
					if ($filterfound==0) $pdf_object->sam_filtercount[$current_year]=$pdf_object->sam_filtercount[$current_year] + $visit_count->total;
					$pdf_object->sam_filtercount['found']=TRUE;
					$filterfound=1;
					}
				if (is_belt($row->material_list_id)) {
					if ($beltfound==0) $pdf_object->sam_beltcount[$current_year]=1;
					$pdf_object->sam_beltcount['found']=TRUE;
					$beltfound=1;
					}
				}
			//////////////////////////////////////////////////////
			// Check for belts in the other tables too,
			//////////////////////////////////////////////////////
			$mbeltqry="select * from svc_agreement_equipment_events where material_list_id > 0 and qty > 0 and is_a_belt = 'Y' and event_date >= '$year_dates->start' and event_date <= '$year_dates->end' and el_id = '$maint_type->el_id' and show_in_summary = 'Y' and suppliment_maint_type_count = 'Y' limit 1";
			if (getoneb($mbeltqry)) {
				$pdf_object->sam_beltcount["$current_year"]=1;
				$pdf_object->sam_beltcount['found']=TRUE;
				$beltfound=1;
				}
			$mfilterqry="select *,sum(1) as filter_count from svc_agreement_equipment_events where material_list_id > 0 and qty > 0 and is_a_filter = 'Y' and event_date >= '$year_dates->start' and event_date <= '$year_dates->end' and el_id = '$maint_type->el_id' and show_in_summary = 'Y' and suppliment_maint_type_count = 'Y' group by description order by filter_count desc limit 1";
			if ($misc_filter_count=getoneb($mfilterqry)) {
				$pdf_object->sam_filtercount_misc["$current_year"]=$misc_filter_count->filter_count;
				$pdf_object->sam_filtercount['found']=TRUE;
				$filterfound=1;
				}
			}
		if (($visit_count->total=="")||($visit_count->total=="0")) $visit_count->total=' ';
		$pdf_object->addText((($pdf_object->sam_yearwidth * ($current_year - 1)) + $pdf_object->sam_yeartab),$current_y,$pdf_object->sam_yearsize,"$visit_count->total",0,-2);
		$current_year++;
		}
	
	if ($pdf_object->sam_executive_summary) {
		$fv_query="select * from svc_equip_sched_items where sched_index_id = '$maint_type->sched_index_id' order by maint_date limit 1";
		$first_visit=getoneb($fv_query);
		//if (!($first_visit)) die ("First visit didn't come back right!!<hr>$fv_query");
		if ($first_visit) {
			$visit_cost=equip_sched_cost($first_visit->sched_item_id);
			//$mat_cost=equip_sched_materials_only_cost($first_visit->sched_item_id);
			$mat_cost=equip_sched_materials_and_misc_only_cost($first_visit->sched_item_id);
			$mat_cost=$mat_cost + 1;
			$mat_cost=$mat_cost - 1;
			$visit_hours=equip_sched_hrs($first_visit->sched_item_id);
		
			$pdf_object->setColor(1,0,0);
			$pdf_object->addText(($pdf_object->ez['pageWidth']/2) - 30 ,$current_y,$pdf_object->sam_yearsize,"Mat\$:$mat_cost Hours:$visit_hours",0);
			$pdf_object->setColor(0,0,0);
			}
		}
	return($pdf_object);	
	}

function pdf_equip_list_filter_row_show($agreement_info,$pdf_object,$el_id) {
	$pdf_object=pdf_equip_item_shade_box_if_required($pdf_object);
	$pdf_object->ezText(" ","",array("spacing"=>"$pdf_object->sam_spacing"));
	$current_y=$pdf_object->y;
	$lr_loc=$pdf_object->sam_yeartab - 10 - ($pdf_object->getTextWidth($pdf_object->sam_yearsize,"Filters"));
	$pdf_object->addText($lr_loc,$current_y,$pdf_object->sam_yearsize,"Filters",0,-2);
	$current_year=1;
	while($current_year <= $agreement_info->years) {
		if ($pdf_object->sam_filtercount[$current_year] < $pdf_object->sam_filtercount_misc[$current_year]) {
			////////////////////////////////////
			// Sidebar to determine true max...
			////////////////////////////////////
			$year_dates=agreement_year_dates($agreement_info->agreement_id,$current_year);
			$maxvisqry="select sum(1) as total from svc_equip_sched_items left join svc_equip_sched_index using (sched_index_id) left join svc_task_items using (task_id) left join svc_maint_types using (maint_type_id) where el_id = '$el_id' and ( maint_type = 'Operational' or maint_type = 'Annual') and maint_date >= '$year_dates->start' and maint_date <= '$year_dates->end' group by el_id";
			$this_year_visits=getoneb($maxvisqry);
			if ($this_year_visits->total < $pdf_object->sam_filtercount_misc[$current_year] ) 
				$pdf_object->sam_filtercount_misc[$current_year]=$this_year_visits->total;
			$pdf_object->sam_filtercount[$current_year]=$pdf_object->sam_filtercount_misc[$current_year];
			}
		if (($pdf_object->sam_filtercount[$current_year]=="")||($pdf_object->sam_filtercount[$current_year]=="0")) $pdf_object->sam_filtercount[$current_year]=' ';
		$pdf_object->addText((($pdf_object->sam_yearwidth * ($current_year - 1)) + $pdf_object->sam_yeartab),$current_y,$pdf_object->sam_yearsize,$pdf_object->sam_filtercount[$current_year],0,-2);
		$current_year++;
		}
	return($pdf_object);	
	}

function pdf_equip_list_belt_row_show($agreement_info,$pdf_object,$el_id) {
	$pdf_object=pdf_equip_item_shade_box_if_required($pdf_object);
	$pdf_object->ezText(" ","",array("spacing"=>"$pdf_object->sam_spacing"));
	$current_y=$pdf_object->y;
	$lr_loc=$pdf_object->sam_yeartab - 10 - ($pdf_object->getTextWidth($pdf_object->sam_yearsize,"Belts"));
	$pdf_object->addText(($lr_loc),$current_y,$pdf_object->sam_yearsize,"Belts",0,-2);
	$current_year=1;
	while($current_year <= $agreement_info->years) {
		if (($pdf_object->sam_beltcount[$current_year]=="")||($pdf_object->sam_beltcount[$current_year]==0)) $pdf_object->sam_beltcount[$current_year]=' ';
		$pdf_object->addText((($pdf_object->sam_yearwidth * ($current_year - 1)) + $pdf_object->sam_yeartab),$current_y,$pdf_object->sam_yearsize,$pdf_object->sam_beltcount[$current_year],0,-2);
		$current_year++;
		}
	return($pdf_object);	
	}

function pdf_equip_list_misc_events($agreement_info,$pdf_object,$el_id) {
	$query="select * from svc_agreement_equipment_events where el_id = '$el_id' and show_in_summary = 'Y' group by description order by description";
	$res=@mysql_query($query);
	while ($row=@mysql_fetch_object($res)) {
		$pdf_object=pdf_equip_list_misc_event_show($agreement_info,$pdf_object,$el_id,$row);
		}
	return ($pdf_object);
	}

function pdf_equip_list_misc_event_show($agreement_info,$pdf_object,$el_id,$event_row) {
	if ((($event_row->is_a_belt=='Y')||($event_row->is_a_filter=='Y')) && ($event_row->suppliment_maint_type_count=='Y')) return ($pdf_object);
	$pdf_object=pdf_equip_item_shade_box_if_required($pdf_object);
	$pdf_object->ezText(" ","",array("spacing"=>"$pdf_object->sam_spacing"));
	$current_y=$pdf_object->y;
	$type_tag="";
	if ($event_row->is_a_filter=='Y') $type_tag="Filters - ";
	if ($event_row->is_a_belt=='Y') $type_tag="Belts - ";
	$event_row->description=addslashes($event_row->description);
	
	$lr_loc=$pdf_object->sam_yeartab - 10 - ($pdf_object->getTextWidth($pdf_object->sam_yearsize,$type_tag . $event_row->description));
	$pdf_object->addText($lr_loc,$current_y,$pdf_object->sam_yearsize,"$type_tag$event_row->description",0);
	
	$current_year=1;
	while($current_year <= $agreement_info->years) {
		$year_dates=agreement_year_dates($agreement_info->agreement_id,$current_year);
		$event_count=getoneb("select sum(1) as total from svc_agreement_equipment_events where el_id = '$el_id' and description = '$event_row->description' and event_date >= '$year_dates->start' and event_date <= '$year_dates->end'");
		if ($event_count->total == 13) {
			echo "select sum(1) as total from svc_agreement_equipment_events where description = '$event_row->description' and event_date >= '$year_dates->start' and event_date <= '$year_dates->end'<br>";
			exit;
			}
		if ($event_count->total==0) $event_count->total=" ";
		$pdf_object->addText((($pdf_object->sam_yearwidth * ($current_year - 1)) + $pdf_object->sam_yeartab),$current_y,$pdf_object->sam_yearsize,$event_count->total,0,-2);
		$current_year++;
		}
	return($pdf_object);	
	}

function pdf_equipment_list ($agreement_id,$pdf_object,$show_exec_summary=0) {
	if ($pdf_object->been_to_new_page==1) $pdf_object->ezNewPage();
	$pdf_object->ezSetMargins(36,44,36,36);
	$agreement_info=load_agreement_info($agreement_id);
	if ($show_exec_summary==0) {
		$pdf_object->sam_executive_summary=FALSE;
		} else {
		$pdf_object->sam_executive_summary=TRUE;
		}

	//$query="select *,sum(1) as total_count 
	//	from svc_agreement_equip_list 
	//	left join svc_equip_types using(equip_type_id) 
	//	left join svc_equip_categories using (category_id) 
	//	where 
	//	agreement_id = '$agreement_id' 
	//	group by svc_agreement_equip_list.equip_type_id
	//	order by svc_equip_categories.category,svc_equip_types.type";

	$query="select *,sum(1) as total_count 
		from svc_agreement_equip_list
		left join (svc_equip_types, svc_equip_categories)
		on 
		(svc_agreement_equip_list.equip_type_id = svc_equip_types.equip_type_id and
		svc_agreement_equip_list.category_id = svc_equip_categories.category_id)
		where
		agreement_id = '$agreement_id'
		group by svc_agreement_equip_list.equip_type_id
		order by svc_equip_categories.category,svc_equip_types.type,svc_agreement_equip_list.tag_num";


	$res=@mysql_query($query);
	$pdf_object->ezText("EQUIPMENT LIST AND INSPECTION SUMMARY\r\n",15,array("justification"=>"center"));
	while ($row=@mysql_fetch_object($res)) {
		$pdf_object=pdf_equip_list_type_show($agreement_info,$pdf_object,$row);
		}
	if ($pdf_object->sam_executive_summary) {
		$total_hours=agreement_total_man_hours ($agreement_id);
		$pdf_object->setColor(1,0,0);
		$pdf_object->ezText("Total Hours: " . $total_hours);
		$pdf_object->setColor(0,0,0);
		}
	return ($pdf_object);
	}

function pdf_tac_pages ($pdf_object) {
	$pdf_object->ezNewPage();
	$pdf_object->ezSetMargins(72,72,72,72);
	$pdf_object=pdf_page_title("SERVICE AGREEMENT TERMS AND CONDITIONS",$pdf_object,14,"",1);
	
	$pdf_object->ezText("ACCEPTANCE",10);
	$pdf_object->ezText("When this Proposal, herein referred to as \"The Service Agreement\", is executed by an officer or duly authorized representative of UMC, the Service Agreement constitutes an offer to contract. Signature by Customer constitutes acceptance by Customer and creates a binding contract. Any change in the Service Agreement by Customer shall constitute a counter-offer which may only be accepted and become part of this Service Agreement by an additional signature by an officer or duly authorized representative of UMC.\n",7);
	
	$pdf_object->ezText("CUSTOMER",10);
	$pdf_object->ezText("Customer is defined as the party to whom UNIVERSITY MECHANICAL CONTRACTORS, INC. (\"UMC\") makes The Service Agreement Proposal.\n",7);

	$pdf_object->ezText("SCOPE OF UMC OBLIGATIONS",10);
	$pdf_object->ezText("UMC shall provide labor, equipment and materials for service inspections in accordance with the <i>SCOPE OF SERVICE, MAINTENANCE TASKING and EQUIPMENT LIST AND INSPECTION SUMMARY</i> attached hereto and subject and pursuant to the terms and conditions set forth herein.\n",7);

	$pdf_object->ezText("SERVICE AVAILABILITY",10);
	$pdf_object->ezText("UMC agrees to provide services in accordance with the <i>SCOPE OF SERVICE</i>.\n",7);
	
	$pdf_object->ezText("TERM",10);
	$pdf_object->ezText("This Service Agreement shall remain in effect for the term specified on the \"Service Agreement Signature Page\". Thereafter, The Service Agreement shall remain in effect from year to year until canceled by either party on 30 days written notice. UMC's prices are subject to change do to changes in labor, parts and costs.\n",7);
	
	$pdf_object->ezText("EXCLUSIONS",10);
	$pdf_object->ezText("Unless otherwise provided in writing herein, The Service Agreement assumes the systems covered to be in maintainable condition. If repairs are found necessary upon initial inspection or initial seasonal startup, repair charges will be submitted to Customer for approval. Should these repair or restoration charges be declined, those non-maintainable items will be eliminated from The Service Agreement and the maintenance price adjusted accordingly or, at UMC's option, The Service Agreement canceled. Maintenance and repair services do not include: (a) non-maintainable components, e.g., castings, heat exchanger shells, ductwork, boiler shell and tubes, chiller shell and tubes, equipment enclosures, boiler refractory material, piping, tube bundles, valve bodies, coils, structural supports, oil storage tanks and other similar items; (b) water supply and drain beyond the subject equipment; (c) electrical services beyond the equipment disconnect switch, or service requirements made necessary as a result of electrical power failure, low voltage, burned out main or branch fuses; (d) motor starting equipment and interconnecting power wiring; (e) repair of damage or increase in service time resulting from accident, transportation or relocation, low water pressure, misuse or other than ordinary use or negligence of Customer or parties other than UMC, unauthorized alteration of equipment, vandalism, theft, labor disputes, hurricane, earthquake, theft, fire, flood, freezing, weather or acts of God; (f) repair to the equipment located in an unsuitable place of installation or an unsafe or hazardous environment; (g) emergency calls, maintenance or repair resulting from or cause by system design problems unless designed by University Mechanical Contractors; (h) maintenance or repair resulting from or caused by energy management systems; (i) any work required by insurance companies, government codes, building or union regulations; (j) repair due to work performed by others than UMC; or (k) safety tests or to installation of new attachments, additional controls, or equipment as recommended or directed by any insurance company or governmental authority, or to make with parts or devices of a different design for any reason.\n",7);
	
	$pdf_object->ezText("OBLIGATIONS OF THE CUSTOMER",10);
	$pdf_object->ezText("Customer shall operate the equipment identified on the attached <i>EQUIPMENT LIST AND INSPECTION SUMMARY</i> in accordance with the instructions given by UMC and the manufacturer.\nCustomer shall maintain all work areas in safe condition and in a condition satisfactory for UMC's work.\nCustomer shall furnish UMC full, safe and free access to the premises and to the equipment identified on the attached <i>EQUIPMENT LIST AND INSPECTION SUMMARY</i>.\nCustomer shall extend reasonable cooperation requested by UMC regarding Customer's personnel, premises, and building maintenance materials, tools, and ladders and movement of items blocking access to required work areas.\nCustomer shall promptly notify UMC upon observation of any conditions adversely affecting the equipment identified on the attached <i>EQUIPMENT LIST AND INSPECTION SUMMARY</i>.\nCustomer shall furnish all power, light, heat, water and gas.\nUMC shall not be obligated to provide any services under The Service Agreement made necessary because of Customer's failure to comply with it's obligations.\nUMC is not responsible or liable for any loss, damages or delay in furnishing materials or failure to perform services when caused by fire, flood, acts of civil or military authorities or by insurrections, riot or civil disorder, or by any other cause which is unavoidable or beyond UMC's control.\n",7);
	
	$pdf_object->ezText("CUSTOMER IS OWNER OF PROPERTY AND/OR EQUIPMENT OR AGENT OF OWNER",10);
	$pdf_object->ezText("Customer represents that it is the owner of the premises and of the equipment identified on the attached <i>EQUIPMENT LIST AND INSPECTION SUMMARY</i> or, if not the owner, that it is the owner's agent and authorized representative and that it has authority to enter into The Service Agreement and cause the services under The Service Agreement to be performed.",7);
	
	$pdf_object->ezText("CHARGES AND PAYMENT TERMS",10);
	$pdf_object->ezText("Services Agreement charges will be invoiced in advance. Provided service has been properly performed as scheduled, payment will be due upon receipt of invoice. An invoice is delinquent thirty (30) days after date of billing. It is agreed that a late charge will be added to delinquent accounts at the rate of one and one half percent (1 1/2%) per month or the highest legal rate.\n",7);
	
	$pdf_object->ezText("TAXES AND OTHER GOVERNMENT CHARGES",10);
	$pdf_object->ezText("There will be added to all charges the amount of any present and future sales taxes or any other governmental charges or assessments, including environmental charges now or hereafter imposed by existing or future laws with respect to any services rendered or parts supplied.\n",7);

	$pdf_object->ezNewPage();
	$pdf_object->ezText("LIMITED WARRANTY",10);
	$pdf_object->ezText("UMC warrants new materials installed for one (1) year after installation and labor on new materials installed for ninety (90) days. In addition, UMC shall make all transferable manufacturer's warranties available to Customer (without recourse against UMC should manufacturer fail to satisfy it's warranty obligations). NO OTHER WARRANTIES EXPRESS OR IMPLIED ARE MADE WITH RESPECT TO THE MATERIALS, LABOR OR SERVICE PROVIDED PURSUANT TO THIS SERVICE AGREEMENT. THE PRECEDING WARRANTY IS IN LIEU OF AND UMC HEREBY EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES INCLUDING WARRANTIES OF FITNESS FOR A PARTICULAR PURPOSE AND OF MERCHANTABILITY. CUSTOMER'S SOLE AND EXCLUSIVE REMEDY FOR BREACH OF WARRANTY IS TO REQUIRE, AT UMC'S OPTION, REPAIR OR REPLACEMENT OF THE MATERIALS, OR REFUND OF THE AMOUNT PAID FOR SERVICE OF THE APPLICABLE EQUIPMENT DURING THE PRECEDING THREE (3) MONTH PERIOD.\n",7);

	$pdf_object->ezText("LIMITATION OF LIABILITY",10);
	$pdf_object->ezText("Customer agrees that UMC's liability hereunder for damage to equipment identified on the attached EQUIPMENT LIST AND INSPECTION SUMMARY shall not exceed the amount paid for service for the damaged equipment during the preceding three (3) months unless such damage is the result of UMC's negligence. This limitation of liability applies regardless of whether such damages or indemnification is sought based on breach of warranty, breach of contract, negligence, strict liability in tort, or any other legal theory. THE CUSTOMER AGREES THAT UMC ASSUMES NO RESPONSIBILITY FOR AND SHALL NOT BE LIABLE TO PURCHASER FOR ANY CONSEQUENTIAL DAMAGES INCURRED BY PURCHASER IN CONNECTION WITH THIS SERVICE AGREEMENT INCLUDING, WITHOUT LIMITATION, LOST PROFITS, CUSTOMER'S GOOD WILL, AND ANY INJURY TO PERSONS OR PROPERTY OF FOR ANY CLAIM OR DEMAND AGAINST THE CUSTOMER BY ANY OTHER PARTY PROXIMATELY RESULTING FROM ANY BREACH BY UMC OR BREACH OF WARRANTY BY UMC, EVEN IF UMC HAS BEEN ADVISED OF OR IS AWARE OF THE POSSIBILITY OF SUCH CONSEQUENTIAL DAMAGES.\n",7);

	$pdf_object->ezText("HAZARDOUS SUBSTANCES (ASBESTOS & TOXIC CHEMICALS AND MATERIALS)",10);
	$pdf_object->ezText("It is understood and agreed that, in seeking the work and services of UMC under The Service Agreement, you may be requesting UMC to undertake obligations for Customer's benefit involving the presence of potentially hazardous substances, chemicals or materials. Therefore, Customer agrees to hold harmless, indemnify, and defend UMC from and against any and all claims, losses, damages, liability, and costs, including but not limited to costs of defense, arising out of or in any way connected with the presence, discharge, release, or escape of any kind of hazardous contaminants, substances, chemicals or materials, excepting only such liability as might arise out of the sole negligence of UMC in the performance of services under this Service Contract.\n",7);

	$pdf_object->ezText("RESTRICTION ON HIRING",10);
	$pdf_object->ezText("If Customer hires or retains as an independent contractor any present or former employee of UMC within 180 days subsequent to termination of The Service Agreement to perform on a regular basis work within the scope of this contract, Customer agrees to pay UMC a sum equal to 6 months service charge, as a liquidated damage (estimated at this time as reasonable reimbursement to UMC for it's expenses in employee training and familiarizing the present or former employee with Customer's service requirements).\n",7);

	$pdf_object->ezText("DEFAULT",10);
	$pdf_object->ezText("If Customer materially breaches any of the terms of The Service Agreement or does not timely pay any invoice within 30 days on seven (7) days' notice, UMC may, in addition to and without prejudice to any other legal remedies it may have, refuse to continue to service hereunder or, at UMC's option, cancel The Service Agreement.\n",7);

	$pdf_object->ezText("VENUE AND CHOICE OF LAW",10);
	$pdf_object->ezText("The Service Agreement shall be interpreted and governed substantively and procedurally by the laws of the state of Washington and the venue for any lawsuit which shall be brought by or against UMC shall be in the County of King, State of Washington.\n",7);

	$pdf_object->ezText("ATTORNEY FEES",10);
	$pdf_object->ezText("In any suit arising out of or relating to The Service Agreement, the substantially prevailing party (including consideration of settlement offers) shall be awarded it's attorneys' fees, expert witness fees, and all costs (not just statutory costs), and including it's attorneys' fees and costs on appeal.\n",7);

	$pdf_object->ezText("NO WAIVER OF BREACH",10);
	$pdf_object->ezText("No waiver of any breach of any term hereunder shall be considered a waiver of any subsequent breach of the same or a different term hereunder.\n",7);

	$pdf_object->ezText("ENTIRE AGREEMENT",10);
	$pdf_object->ezText("When executed by both parties, The Service Agreement contains the entire understanding between the parties and supersedes any prior understandings, written and/or oral, respecting the same subject matter. There are not representations, agreements, arrangements or understandings, oral or written, between or among the parties relating to the subject matter of The Service Agreement which are not fully expressed herein. Any modification of The Service Agreement shall be in writing and signed by the parties hereto. The terms and conditions herein shall prevail notwithstanding any variance with the terms and conditions of any order submitted by Customer with respect to maintenance service.\n",7);

	$pdf_object->ezText("NOTICE TO CUSTOMER",10);
	$pdf_object->ezText("This contractor is registered with the state of Washington, registration no. <b>UN-IV-MC-*343N9</b>, as a general contractor and has posted with the state a bond for the purpose of satisfying claims against the contractor for negligent or improper work or breach of contract in the conduct of the contractor's business. This bond may not be sufficient to cover a claim which might arise from the work done under your contract. If any supplier of materials used in your construction project or any employee of the contractor is not paid by the contractor or subcontractor on your job, a lien may be placed against your property to force payment. If you wish additional protection, you may request the contractor to provide you with original \"lien release\" documents from each supplier or subcontractor on your project. The contractor is required to provide you with further information about lien release documents if you request it.  General information is also available from the department of labor and industries.\n",7);
	return ($pdf_object);
	}

function pdf_double_sig_line($pdf_object) {
	$tablefontsize=16;
	$blankstring="                                                  ";
	$tablewidth=$pdf_object->ez['pageWidth'] - $pdf_object->ez['rightMargin'] - $pdf_object->ez['leftMargin'];
	$colwidth=round($tablewidth/2);
	$data=array(array("one"=>"<u>$blankstring</u>","two"=>"<u>$blankstring</u>"));
	$pdf_object->ezTable($data,"","",array('showLines'=>"0",'showHeadings'=>'0','width'=>"$tablewidth",'fontSize'=>"$tablefontsize",'cols'=>array('one'=>array('width'=>"$colwidth"),'two'=>array('width'=>"$colwidth"))));
	return ($pdf_object);
	}

function pdf_undersig_text($text,$pdf_object,$pos="left") {
	$starting_y=$pdf_object->y;
	$starting_x=$pdf_object->x;
	if ($pos=="right") {
		$width=$pdf_object->ez['pageWidth'] - $pdf_object->ez['leftMargin'] - $pdf_object->ez['rightMargin'];
		$halfwidth=round($width/2);
		$xpos=$pdf_object->ez['leftMargin'] + $halfwidth;
		}
	if ($pos!="right") $xpos=$pdf_object->ez['leftMargin'];
	$pdf_object->x=$xpos;
	$pdf_object->ezSetDy(-27);
	$pdf_object->addText($xpos,$pdf_object->y,6,$text);
	$pdf_object->ezSetY($starting_y);
	$pdf_object->x=$starting_x;
	return ($pdf_object);
	}

function pdf_title_page($agreement_id,$pdf_object) {
	///////////////////////////////////////////
	//                         T   B   L   R
	$pdf_object->ezSetMargins(160,422,320,115);
	//$pdf_object->ezSetMargins(160,432,320,115);
	
	$agreement_info=load_agreement_info($agreement_id);
	
	$pdf_object->ezText("\nPrepared for:",10,array('justification'=>'center'));
	$pdf_object->ezText("<b>$agreement_info->company_name</b>",11,array('justification'=>'center'));
	$pdf_object->ezText("$agreement_info->company_address",9,array('justification'=>'center'));
	if ("$agreement_info->company_city$agreement_info->company_state"!="")
		$pdf_object->ezText("$agreement_info->company_city, $agreement_info->company_state $agreement_info->company_zip\n",9,array('justification'=>'center'));
		else
		$pdf_object->ezText("",9,array('justification'=>'center'));
	
	$pdf_object->ezText("Attention:\n<b>$agreement_info->contact_first_name $agreement_info->contact_last_name</b>\n",9,array('justification'=>'center'));
	
	if ($agreement_info->company_id != $agreement_info->site_id) {
		$pdf_object->ezText("Site Name/Address:",10,array('justification'=>'center'));
		$pdf_object->ezText("<b>$agreement_info->site_name_short</b>",11,array('justification'=>'center'));
		$pdf_object->ezText("$agreement_info->site_address",9,array('justification'=>'center'));
		if ("$agreement_info->site_city$agreement_info->site_state"!="")
			$pdf_object->ezText("$agreement_info->site_city, $agreement_info->site_state $agreement_info->site_zip\n",9,array('justification'=>'center'));
			else
			$pdf_object->ezText("",9,array('justification'=>'center'));
		}
	$pdf_object->ezText("Prepared by:\n$agreement_info->creator_first_name $agreement_info->creator_last_name\n",9,array('justification'=>'center'));
	$pdf_object->ezText(date('F d, Y'),10,array('justification'=>'center'));
	
	///////////////////////////////////////////
	//                         T   B   L   R
	//$pdf_object->ezSetMargins(160,422,72,72);
	if ($agreement_info->agreement_printed_title!="") {
		$pdf_object->ezSetMargins(72,72,72,72);
		$pdf_object->ezText("
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		$agreement_info->agreement_printed_title",12,array('justification'=>'center'));
		}
	$pdf_object->ezSetMargins(72,72,216,72);
	return ($pdf_object);
	}

function pdf_sig_page($agreement_id,$pdf_object) {
	setlocale(LC_MONETARY, 'en_US');
	$title_size="11";
	$text_size="9";
	$umc_info=getone("select * from contacts where contacts_id = '968'");
	$umc_info->address_1=ereg_replace("\n","",$umc_info->address_1);
	$agreement_info=load_agreement_info($agreement_id);
	$pdf_object->ezNewPage();
	$pdf_object->ezSetMargins(72,72,72,72);
	$pdf_object=pdf_page_title("University Mechanical Contractors Inc, - Service Group\nService Agreement Signature Page",$pdf_object,14,"center",1);
	
	$pdf_object->ezText("\nSERVICE AGREEMENT",$title_size);
	$pdf_object->ezText("This Service Agreement is entered into by and between $agreement_info->company_name located at $agreement_info->site_address, $agreement_info->site_city, $agreement_info->site_state $agreement_info->site_zip and $umc_info->display_name - Service Group (UMC) located at $umc_info->address_1, $umc_info->address_city, $umc_info->address_state $umc_info->address_zip.\n",$text_size);
	
	$pdf_object->ezText("EQUIPMENT SERVICED",$title_size);
	$pdf_object->ezText("See \"Equipment List and Inspection Summary\".\n",$text_size);
	
	$pdf_object->ezText("SERVICE FREQUENCY",$title_size);
	$pdf_object->ezText("See \"Equipment List and Inspection Summary\".\n",$text_size);
	
	if ($agreement_info->forced_total_value>0.00) $agreement_info->total_value=$agreement_info->forced_total_value;
	
	if ($agreement_info->billing_frequency=="Monthly") {
		$payments=$agreement_info->years * 12;
		$payment=(round(100 * ($agreement_info->total_value / $payments))) / 100;
		$payment=round($payment);
		$yearly_price=$payment * 12;
		$total_value_text=format_long_num(round($payment * 12 * $agreement_info->years));
		$time_period="month";
		} else {
		///////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		// At least for now, if it's not monthly, then it's quarterly.
		// ...Doh.. not any more.. not it can be semi-annually.. 
		///////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		if ($agreement_info->billing_frequency=="Semiannually") {
			$payments=$agreement_info->years * 2;
			$payment=(round(100 * ($agreement_info->total_value / $payments))) / 100;
			$payment=round($payment);
			$yearly_price=$payment * 2;
			$total_value_text=format_long_num(round($payment * 2 * $agreement_info->years));
			$time_period="every 6 month period";
			} else {
			// NOW it should be quarterly... (I think)
			$payments=$agreement_info->years * 4;
			$payment=(round(100 * ($agreement_info->total_value / $payments))) / 100;
			$payment=round($payment);
			$yearly_price=$payment * 4;
			$total_value_text=format_long_num(round($payment * 4 * $agreement_info->years));
			$time_period="quarter";
			}
		}
	$yearly_price=round($yearly_price);
	$yearly_price_text=format_long_num($yearly_price);
	$payment_text=format_long_num($payment);
	
	if ($agreement_info->billing_frequency!="Annual Sum Only") {
		$pdf_object->ezText("AGREEMENT PRICE",$title_size);
		$pdf_object->ezText("The AGREEMENT PRICE in year one shall be \$$yearly_price_text, payable \$$payment_text plus tax per $time_period.\nThe TOTAL AGREEMENT PRICE is \$$total_value_text covering a term of $agreement_info->years years.\n",$text_size);
		$pdf_object->ezText("PRE-PAYMENT",$title_size);
		$pdf_object->ezText("In the event CUSTOMER pays the annual AGREEMENT PRICE in advance, the price shall be reduced by 3.00%. In the event CUSTOMER pays the TOTAL AGREEMENT PRICE of a MULTI-YEAR AGREEMENT (minimum of three (3) years) in advance, the price shall be reduced by 5.00%.\n",$text_size);
		} else {
		$pdf_object->ezText("AGREEMENT PRICE",$title_size);
		$pdf_object->ezText("The AGREEMENT PRICE in year one shall be \$$yearly_price_text, payable upon completion. The TOTAL AGREEMENT PRICE is \$$total_value_text covering a term of $agreement_info->years years.\n",$text_size);
		}
	
	//if ($agreement_info->billing_frequency!="Annual Sum Only") {
	//	$pdf_object->ezText("AGREEMENT PRICE",$title_size);
	//	$pdf_object->ezText("The AGREEMENT PRICE in year one shall be \$$yearly_price_text.\nPayments of \$$payment_text are due ${time_period}ly.\nThe TOTAL AGREEMENT PRICE is \$$total_value_text,\nCovering a term of $agreement_info->years years.\n",$text_size);
	//	$pdf_object->ezText("PRE-PAYMENT",$title_size);
	//	$pdf_object->ezText("In the event CUSTOMER pays the annual AGREEMENT PRICE in advance, the price shall be reduced by 3.00%. In the event CUSTOMER pays the TOTAL AGREEMENT PRICE of a MULTI-YEAR AGREEMENT (minimum of three (3) years) in advance, the price shall be reduced by 5.00%.\n",$text_size);
	//	} else {
	//	$pdf_object->ezText("AGREEMENT PRICE",$title_size);
	//	//$pdf_object->ezText("The AGREEMENT PRICE in year one shall be \$$yearly_price_text, payable upon completion. The TOTAL AGREEMENT PRICE is \$$total_value_text covering a term of $agreement_info->years years.\n",$text_size);
	//	//$pdf_object->ezText("The AGREEMENT PRICE in year one shall be \$$yearly_price_text, payable upon completion. The TOTAL AGREEMENT PRICE is \$$total_value_text covering a term of $agreement_info->years years.\n",$text_size);
	//	$pdf_object->ezText("The AGREEMENT PRICE in year one shall be \$$yearly_price_text.\nPayments of \$$payment_text are due ${time_period}ly.\n\nThe TOTAL AGREEMENT PRICE is \$$total_value_text covering a term of $agreement_info->years years.\n",$text_size);
	//	}
	
	
	if ($agreement_info->expected_start_date_override!="") {
		$document_start_date=$agreement_info->expected_start_date_override;
		} else {
		$document_start_date=$agreement_info->expected_start_date;
		}
		
	$pdf_object->ezText("AGREEMENT TERM",$title_size);
	$pdf_object->ezText("This AGREEMENT shall become effective on $document_start_date and shall continue for a $agreement_info->years year term, and from year-to-year thereafter. Either party may terminate this agreement at the end of year $agreement_info->years or prior to the end of any subsequent year by giving the other party at least thirty (30) days prior written notice.\n",$text_size);
	
	$pdf_object->ezText("ACCEPTANCE AND APPROVAL",$title_size);
	$pdf_object->ezText("This shall become a valid AGREEMENT upon signature of CUSTOMER and signature by a UMC representative in the \"Approved by\" block below. The undersigned acknowledges and agrees by its signature, that the Scope of Service, Terms and Conditions of Sale and any amendment or addenda prepared by UMC with respect thereto constitutes the entire AGREEMENT.\n",$text_size);
	
	$datetxt=date('Ymd');
	$agreement_longint=str_pad($agreement_info->agreement_id,6,'0',STR_PAD_LEFT);
	$pdf_object->ezText("<b>Proposal #:</b> <i>$datetxt$agreement_longint$agreement_info->current_doc_revision</i>\n",8);
	//echo "Hello";exit;
	
	$longname=str_pad($agreement_info->creator_name,30," ",STR_PAD_RIGHT);
	$pdf_object->ezText("<b>Submitted by: $agreement_info->creator_first_name $agreement_info->creator_last_name                               Date: " . date('m/d/Y') . "</b>\n",12);
	
	$tablewidth=$pdf_object->ez['pageWidth'] - $pdf_object->ez['rightMargin'] - $pdf_object->ez['leftMargin'];
	$colwidth=round($tablewidth/2);

///////////////////////////////
/// Ah.. it works..
///////////////////////////////

	$tablefontsize=14;
	$data=array(array("one"=>"Approved By:","two"=>"Customer Acceptance:"));
	$pdf_object->ezTable($data,"","",array('showLines'=>"0",'showHeadings'=>'0','width'=>"$tablewidth",'fontSize'=>"$tablefontsize",'cols'=>array('one'=>array('width'=>"$colwidth"),'two'=>array('width'=>"$colwidth"))));

	$tablefontsize=10;
	$data=array(array("one"=>"$umc_info->display_name Contractors, Inc. - Service Group","two"=>"$agreement_info->company_name"));
	$pdf_object->ezTable($data,"","",array('showLines'=>"0",'showHeadings'=>'0','width'=>"$tablewidth",'fontSize'=>"$tablefontsize",'cols'=>array('one'=>array('width'=>"$colwidth"),'two'=>array('width'=>"$colwidth"))));

	//////////////////////////////////////////////////
	// Actual signature lines
	//////////////////////////////////////////////////
	$pdf_object->ezSetDy(+18);
	$pdf_object=pdf_undersig_text("Company Name (Customer)",$pdf_object,"right");
	$pdf_object=pdf_double_sig_line($pdf_object);
	
	$pdf_object=pdf_undersig_text("Approved by (Signature)",$pdf_object);
	$pdf_object=pdf_undersig_text("Accepted by (Signature)",$pdf_object,"right");
	$pdf_object=pdf_double_sig_line($pdf_object);

	$pdf_object=pdf_undersig_text("Approved by (Type/Printed name)",$pdf_object);
	$pdf_object=pdf_undersig_text("Accepted by (Type/Printed name)",$pdf_object,"right");
	$pdf_object=pdf_double_sig_line($pdf_object);

	$pdf_object=pdf_undersig_text("Title",$pdf_object);
	$pdf_object=pdf_undersig_text("Title",$pdf_object,"right");
	$pdf_object=pdf_double_sig_line($pdf_object);

	$pdf_object=pdf_undersig_text("UMC Approval Date",$pdf_object);
	$pdf_object=pdf_undersig_text("Customer Acceptance Date",$pdf_object,"right");
	$pdf_object=pdf_double_sig_line($pdf_object);
	return ($pdf_object);
	}

function pdf_task_list ($task_id,$pdf_object,$no_new_page=0) {
	if ($no_new_page==0) $pdf_object->ezNewPage();
	$task_info=load_task_info($task_id);
	//print_r($task_info);
	$short_query="select * from svc_task_lists_shorted where agreement_type_id = '$task_info->agreement_type_id' and maint_type_id = '$task_info->maint_type_id'";
	//echo "<hr>($task_id) $short_query";die();
	$shorted_results=getoneb($short_query);
	
	//if ($task_id<1) {
	$have_task_list_bool=getoneb("select * from svc_task_lists where task_id = '$task_id' limit 1");
	if (!($have_task_list_bool)) {
		if ($shorted_results) {
			$borrowed_task_list=getone("select task_id from svc_task_items where equip_type_id = '$task_info->equip_type_id' and agreement_type_id = '$shorted_results->agreement_type_id_target' and maint_type_id = '$task_info->maint_type_id'");
			$task_id=$borrowed_task_list->task_id;
			}
		}
		
	$pdf_object->ezSetMargins(72,56,72,72);
	$pdf_object=pdf_page_title("$task_info->equip_type\n$task_info->maint_type Inspection & Preventative Maintenance Service",$pdf_object);

	$pdf_object->ezText("\nUMC will provide the following $task_info->maint_type Inspection and preventative maintenance service.",12);
	$task_list=@mysql_query("select * from svc_task_lists where task_id = '$task_id' order by task_list_number");
	
	while($task_row=@mysql_fetch_object($task_list)) {
		
		$pdf_object->transaction('start');
		$current_page=$pdf_object->ezPageCount;
		$num=str_pad($task_row->task_list_number,2," ",STR_PAD_LEFT);
		$pdf_object->ezText("\n$num.  $task_row->task_text",12);
		
		if ($current_page==$pdf_object->ezPageCount) {
			$pdf_object->transaction('commit');
			} else {
			$pdf_object->transaction('abort');
			$pdf_object->ezNewPage();
			$pdf_object=pdf_page_title("$task_info->equip_type\n$task_info->maint_type Inspection & Preventative Maintenance Service cont..",$pdf_object);
			$pdf_object->ezText("\n$num.  $task_row->task_text",12);
			}
		}
	return ($pdf_object);
	}

function pdf_page_title ($title_text,$pdf_object,$textsize='16',$alignment='left',$linesize="5") {
	$pdf_object->ezText("<b>$title_text</b>",$textsize,array('justification'=>"$alignment"));
	$downdist=round($textsize/2);
	$pdf_object->ezSetDy("-$downdist");
	$pdf_object->setLineStyle($linesize,'round');
	$pdf_object->line($pdf_object->ez['leftMargin'],$pdf_object->y,$pdf_object->ez['pageWidth']-$pdf_object->ez['rightMargin'],$pdf_object->y);
	return ($pdf_object);
	}

function task_number_move_up($task_list_id) {
	$tl_info=getone("select * from svc_task_lists where task_list_id = '$task_list_id'");
	if ($tl_info->task_list_number==1) return $tl_info->task_list_number;
	
	$newnum=$tl_info->task_list_number - 1;
	$oldnum=$tl_info->task_list_number;
	$tempnum='999999';
	
	@mysql_query("update svc_task_lists set 
	task_list_number = '$tempnum' where 
	task_id = '$tl_info->task_id' and task_list_number = '$oldnum'");
	
	@mysql_query("update svc_task_lists set 
	task_list_number = '$oldnum' where 
	task_id = '$tl_info->task_id' and task_list_number = '$newnum'");
	
	@mysql_query("update svc_task_lists set 
	task_list_number = '$newnum' where 
	task_id = '$tl_info->task_id' and task_list_number = '$tempnum'");
	return ($newnum);
	}


function get_next_tasknum($task_id) {
	$query="select task_list_number from svc_task_lists where task_id = '$task_id' order by task_list_number desc limit 1";
	$res=@mysql_query($query);
	$rowcount=@mysql_num_rows($res);
	if ($rowcount < 1) return (1);
	else {
		$row=@mysql_fetch_object($res);
		$number=$row->task_list_number;
		$number++;
		return ($number);
		}
	return (-10); // This should *NEVER* happen!!!!!!!
	}

function load_task_info($task_id) {
	$task_info=getone("select * from svc_task_items where task_id = '$task_id'");
	$equip_info=getone("select * from svc_equip_types where equip_type_id = '$task_info->equip_type_id'");
	$maint_info=getoneb("select * from svc_maint_types where maint_type_id = '$task_info->maint_type_id'");
	$task_info->maint_type=$maint_info->maint_type;
	$task_info->equip_type=$equip_info->type;
	$lt_res=@mysql_query("select * from svc_labor_types");
	while ($lt_row=@mysql_fetch_object($lt_res)) {
		$hrs_info=getoneb("select sum(hrs) as total from svc_task_hrs where task_id = '$task_id' and labor_type_id = '$lt_row->labor_type_id'");
		if (($hrs_info->total=="")||($hrs_info->total<=0)) {
			eval ("\$task_info->labor_type_$lt_row->labor_type_id=0;");
			} else {
			eval ("\$task_info->labor_type_$lt_row->labor_type_id=$hrs_info->total;");
			}
		}
	return ($task_info);
	}

function load_equip_info($el_id,$check_age=0) {
	$query="select * from svc_agreement_equip_list where el_id = '$el_id'";
	$equip_info=getoneb($query);
	if (!($equip_info)) return ($equip_info);
	$mfg_info=getoneb("select * from contacts where contacts_id = '$equip_info->mfg'");
	$equip_info->mfg_name=$mfg_info->display_name;
	if ($equip_info->serial_num=="") $equip_info->serial_num="<i>&lt;missing&gt;</i>";
	$category_info=getoneb("select * from svc_equip_categories where category_id = '$equip_info->category_id'");
	$equip_info->category=$category_info->category;
	$type_info=getoneb("select * from svc_equip_types where equip_type_id = '$equip_info->equip_type_id'");
	$equip_info->predict_oil_cost=$type_info->predict_oil_cost;
	$equip_info->type=$type_info->type;
	$equip_info->description="$equip_info->mfg_name : $equip_info->category : $equip_info->type";
	$equip_info->class="$equip_info->category : $equip_info->type";
	if ($check_age!=0) {
		$age_res=getoneb("select min(approval_required_age) as age from svc_equip_sched_index left join svc_task_items using (task_id) where el_id = '$el_id'");
		if ($age_res->age<=$equip_info->age) {
				$equip_info->age_safe=FALSE;
				$equip_info->age_text="$equip_info->age/$age_res->age";
			} else {
				$equip_info->age_safe=TRUE;
				$equip_info->age_text=$equip_info->age;
			}
		} else {
		$equip_info->age_safe=TRUE;
		$equip_info->age_text=$equip_info->age;
		}
	
	return ($equip_info);
	}

function el_create_tasks ($el_id) {
	$el_info=getone("select * from svc_agreement_equip_list where el_id = '$el_id'");
	$agreement_info=load_agreement_info($el_info->agreement_id);
	$find_query="select * from svc_task_items where equip_type_id = '$el_info->equip_type_id' and agreement_type_id = '$agreement_info->agreement_type_id'";
	$res=@mysql_query($find_query);
	while($row=@mysql_fetch_object($res)) {
		schedule_equip_index_set($el_info->el_id,$row->task_id);
		//echo "schedule_equip_index_set for $row->task_id done<hr>";flush();
		}
	}

function schedule_equip_index_set($el_id,$task_id,$frequency="",$start_date="",$special_note="") {
	$error=0;
	$el_info=getone("select * from svc_agreement_equip_list where el_id = '$el_id'");
	$task_info=getone("select * from svc_task_items where task_id = '$task_id'");

	$tmpres=mysql_query("select sched_index_id from svc_equip_sched_index where el_id = '$el_info->el_id' and task_id = '$task_info->task_id' group by sched_index_id");
	
	while($tmp=@mysql_fetch_object($tmpres)) {
		@mysql_query("delete from svc_equip_sched_items where sched_index_id = '$tmp->sched_index_id'");
		$old_sched_index_id=$tmp->sched_index_id;
		}
	mysql_query("delete from svc_equip_sched_index where el_id = '$el_info->el_id' and task_id = '$task_info->task_id'");
	
	$agreement_info=load_agreement_info($el_info->agreement_id);
	
	if ($frequency=="") $frequency=$task_info->recommended_times_a_year;

	if (($frequency < $task_info->required_times_a_year)&&(!(is_admin()))) {
		$error=1;
		$frequency=$task_info->required_times_a_year;
		} 
	if ($frequency == 0.00) return 2;

	$start_date=vali_date($start_date);
	
	$query="insert into svc_equip_sched_index set 
	el_id = '$el_info->el_id',
	task_id = '$task_info->task_id',
	start_date = '$start_date',
	times_a_year = '$frequency',
	special_notes = '$special_notes'";
	
	$res=@mysql_query($query);
	$sched_id=@mysql_insert_id();
	mysql_query("update svc_agreement_materials set sched_index_id = '$sched_id' where sched_index_id = '$old_sched_index_id'");
	schedule_equip_rebuild($sched_id);
	return ($error);
	}

function schedule_equip_rebuild($sched_index_id) {
	$sched_info=getone("select * from svc_equip_sched_index where sched_index_id = '$sched_index_id'");
	$equip_info=getone("select * from svc_agreement_equip_list where el_id = '$sched_info->el_id'");
	$agreement_info=load_agreement_info($equip_info->agreement_id);
	
	$agreement_start_date=$agreement_info->expected_start_date;
	$agreement_start_date=vali_date($agreement_start_date);

	$agreement_end_date_ts=strtotime("$agreement_start_date +$agreement_info->years years");
	$agreement_end_date_ts2=strtotime("-1 days",$agreement_end_date_ts);
	$agreement_end_date=date('Y-m-d',$agreement_end_date_ts2);
	
	$days_between=round(365 / $sched_info->times_a_year);
	$half_period=round($days_between/2);
	if ($sched_info->times_a_year >1) {
		$rebuilt_year_length=round($sched_info->times_a_year * $days_between);
		if ($rebuilt_year_length>365) {
			$diff=$rebuilt_year_length - 365;
			$yearly_correction_text="-$diff days";
			}
		if ($rebuilt_year_length<365) {
			$diff=365 - $rebuilt_year_length;
			$yearly_correction_text="+$diff days";
			}
		if ($rebuilt_year_length==365) {
			$yearly_correction_text="";
			}
		} else {
		$yearly_correction_text="";
		}
	if ($sched_info->start_date=="0000-00-00") {
		$start_date=date('Y-m-d',strtotime("$agreement_start_date +$half_period days"));
		} else {
		$start_date=$sched_info->start_date;
		}
	$next_date=$start_date;
	if ($agreement_end_date < $agreement_start_date) return 1;
	while ($next_date < $agreement_start_date) $next_date=date('Y-m-d',(strtotime("$next_date +$days_between days")));
	while ($next_date < $agreement_end_date) {
		$query="insert into svc_equip_sched_items set sched_index_id = '$sched_info->sched_index_id', maint_date = '$next_date'";
		@mysql_query($query);
		$next_date=date('Y-m-d',(strtotime("$next_date +$days_between days")));
		
		if ($next_date_year=="") $last_date_year=date('Y',(strtotime("$next_date +$days_between days")));
		else $last_date_year=$next_date_year;
		$next_date_year=date('Y',(strtotime("$next_date +$days_between days")));
		if ($last_date_year!=$next_date_year) {
			$next_date=date('Y-m-d',(strtotime("$next_date $yearly_correction_text")));
			}
		}
	flush();
	}

function load_agreement_info($agreement_id,$calculate_cost=0) {
	$agreement_id=addslashes($agreement_id);
	$info=getoneb("select * from svc_agreement_index where agreement_id = '$agreement_id'");
	if (!($info)) return ($info);
	$info->expected_start_date_mysql=$info->expected_start_date;
	$info->expected_start_date=invali_date($info->expected_start_date);
	$info->expected_start_date_override_mysql=$info->expected_start_date_override;
	$info->expected_start_date_override=invali_date($info->expected_start_date_override);
	
	$creator=getone("select * from contacts where contacts_id = '$info->creator'");
	
	$contact_info=getoneb("select * from contacts where contacts_id = '$info->customer_contact_id'");
	$company_info=getoneb("select * from contacts where contacts_id = '$info->company_id'");
	$info->contact_name=$contact_info->display_name;
	$info->contact_first_name=$contact_info->first_name;
	$info->contact_last_name=$contact_info->last_name;
	
	$info->company_name=$company_info->display_name;
	$info->company_address=ereg_replace("\n","",$company_info->address_1);
	$info->company_city=$company_info->address_city;
	$info->company_state=$company_info->address_state;
	$info->company_zip=$company_info->address_zip;
	
	if ($info->site_id <1) $site_id=$info->company_id;
	else $site_id=$info->site_id;
	
	$site_info=getone("select * from contacts where contacts_id = '$site_id'");
	$info->site_name=$site_info->display_name;
	$info->site_name_short=$site_info->company;
	$info->site_address=ereg_replace("\n","",$site_info->address_1);
	$info->site_state=$site_info->address_state;
	$info->site_city=$site_info->address_city;
	$info->site_zip=$site_info->address_zip;
	$info->site_phone=$site_info->phone;
	
	$info->creator_name=$creator->display_name;
	$info->creator_first_name=$creator->first_name;
	$info->creator_last_name=$creator->last_name;
	
	$sum=getoneb("select count(1) as total from svc_agreement_equip_list where agreement_id = '$info->agreement_id'");
	
	$info->equipment_count=$sum->total;

	$agt_info=getone("select * from svc_agreement_types where agreement_type_id = '$info->agreement_type_id'");
	$info->agreement_type=$agt_info->agreement_type;
	
	if ($calculate_cost!=0) {
		$total_cost=agreement_cost($agreement_info->agreement_id);
		}
	if ($info->status=="Won") $info->probability=100;
	if ($info->status=="Lost") $info->probablity=0;
	if ($info->status=="Dead") $info->probability=0;
	if ($info->probability==0) $info->probability="";

	if ($info->agreement_name=="") $info->agreement_name="$info->company_name&nbsp;-&nbsp;$info->contact_name";

	return($info);
	}

function print_agreement_line($agreement_id) {
	$agreement_info=load_agreement_info($agreement_id);
	if (!($agreement_info)) {
		echo "<tr><td><b>ERROR: No agreement_id: \"$agreement_id\"</b></td></tr>";
		return ($agreement_info);
		}
	$agreement_notes=ereg_replace("[\r\n]",'<br>',$agreement_info->agreement_notes);
	if ($agreement_notes=="") $agreement_notes="&nbsp;&nbsp;-&nbsp;&nbsp;";
	echo "<tr valign=top><td>
			$agreement_info->status
		</td><td>
			<a href=$pagename?mode=svc_agreement_edit&agreement_id=$agreement_id><font color=blue>
			$agreement_info->agreement_name
			</font></a>
		</td><td>
			$agreement_info->agreement_type
		</td><td>
			$agreement_info->expected_start_date
		</td><td id=\"agreement_notes_$agreement_info->agreement_id\" style=\"cursor: hand; cursor: pointer;\" onclick=\"load_note_editor($agreement_id)\">
			$agreement_notes
		</td></tr>
		";	
	}
?>
