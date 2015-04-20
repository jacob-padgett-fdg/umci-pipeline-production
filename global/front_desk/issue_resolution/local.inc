<?

function testrights($rights) {
	global $issue_res_read_only,$issue_res_read_write,$issue_res_full,$issue_res_admin,$issue_res_application_id;

	if ($rights=="read_only")
		if (!($issue_res_read_only))
			die("Error: Missing required permissions ('$rights'). Please contact your site administrator if you have any questions.<hr><a href=$pagename?mode=main&issue_res_application_id_set=$issue_res_application_id><font color=blue>Cancel</font></a>");
	
	if ($rights=="read_write")
		if (!($issue_res_read_write))
			die("Error: Missing required permissions ('$rights'). Please contact your site administrator if you have any questions.<hr><a href=$pagename?mode=main&issue_res_application_id_set=$issue_res_application_id><font color=blue>Cancel</font></a>");

	if ($rights=="full")
		if (!($issue_res_full))
			die("Error: Missing required permissions ('$rights'). Please contact your site administrator if you have any questions.<hr><a href=$pagename?mode=main&issue_res_application_id_set=$issue_res_application_id><font color=blue>Cancel</font></a>");
	}

function problem_information($problem_id) {
	global $pagename;
	$return_object=new bsclass;
	$problem=getoneb("select * from issue_res_issues where problem_id = '$problem_id'");
	$category=getoneb("select * from issue_res_category where issue_res_category_id = '$problem->issue_res_category_id'");
	$application=getoneb("select * from issue_res_application where issue_res_application_id = '$category->issue_res_application_id'");
	$job=getoneb("select * from jobinfo where jobinfo_id = '$application->jobinfo_id'");

	$return_object->problem_id=escapeshellcmd($problem_id);
	$return_object->jobinfo_id=$job->jobinfo_id;
	$return_object->job_name=$job->job_name;
	$return_object->jobinfo_job_name=$job->job_name;
	$return_object->contract_num=$job->contract_num;
	$return_object->jobinfo_contract_num=$job->contract_num;
	
	$return_object->application_name=$application->issue_res_application_name;
	$return_object->application_id=$application->issue_res_application_id;
	
	$return_object->category_name=$category->category_name;
	$return_object->category_id=$category->issue_res_category_id;
	
	$return_object->textpath="$return_object->job_name ($return_object->contract_num):$return_object->application_name:$return_object->category_name";
	$return_object->problem_url="$pagename?glad1=$return_object->category_id&glad2=$return_object->problem_id";
	return $return_object;
	}




?>
