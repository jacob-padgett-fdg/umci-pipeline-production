<?php
/**
 * Created by PhpStorm.
 * User: greg
 * Date: 12/9/14
 * Time: 3:17 PM
 */

namespace classes\Data;
require_once('BaseDataAccess.php');

class Contact extends BaseDataAccess
{
    public $table = 'contacts';
    public $primaryKey = 'contacts_id';

	public $contacts_id;
	public $contact_type;
	public $cam_id;
	public $point_of_contact_id;
	public $first_name;
	public $first_name_real;
	public $last_name;
	public $login_name;
	public $initials;
	public $employee_num;
	public $supervisor_employee_num;
	public $employee_group;
	public $employee_trade;
	public $employee_class;
	public $employee_trade_description;
	public $employee_trade_warning;
	public $employee_union_local;
	public $employee_dept;
	public $overtime_exempt;
	public $web_password;
	public $company;
	public $company_id;
	public $alphasort;
	public $display_name;
	public $email;
	public $title;
	public $address_1;
	public $address_city;
	public $address_state;
	public $address_zip;
	public $phone;
	public $phone_ext;
	public $phone_fax;
	public $phone_cell;
	public $phone_page;
	public $phone_home;
	public $icq_number;
	public $auto_notify_method;
	public $umc_emp;
	public $is_company;
	public $current;
	public $proj_manager;
	public $proj_sponsor;
	public $gc_proj_manager;
	public $owner;
	public $is_general;
	public $mechanical_contractor;
	public $manufacturer;
	public $consultant;
	public $is_reprographics_firm;
	public $architect;
	public $foreman;
	public $superintendent;
	public $mech_engineer;
	public $proj_engineer;
	public $bidlist_takeoff_lists;
	public $estimator;
	public $head_cheese;
	public $detailer;
	public $umc_site_1;
	public $umc_site_2;
	public $umc_site_3;
	public $contract_admin;
	public $notes;
	public $record_creator;
	public $timesheet_payroll_sequence_num;
	public $timesheet_allow_ap_entry;
	public $timesheet_group_one_foreman;
	public $timesheet_power_user;
	public $pref_paper_size;
	public $pref_timesheet_hrs_threshold;
	public $pref_timesheet_reminder_hour;
	public $pref_timesheet_svl;
	public $pref_timesheet_svemail;
	public $pref_timesheet_always_new_toggle;
	public $pref_uslist_exclude;
	public $pref_timesheet_fab_phases_only;
	public $pref_timesheet_fab_phases_warn;
	public $last_touched;
	public $pref_proposals_show_advanced_ownerbox;
	public $photo_file_group_id;
	public $vp_cust_num;
	public $pref_pw_log;
	public $last_pass;
	public $intranet_cookie_id;
	public $lam_notes;
	public $fd2_pass_salt;
	public $fd2_pass_hash;
	public $fd2_pass_hash_change_available;
    public $fd2_pass_hash_change_expires;
}