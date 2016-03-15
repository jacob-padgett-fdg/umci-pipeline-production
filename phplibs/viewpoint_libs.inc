<?php
////////////////////////////////////////////////////////////////////////////////////////
function ms_getone ($query,$link="")
{
	global $conn;
	global $use_odbc;
	
	if ($use_odbc)
	{
		if ($link=="")
			$result=(@odbc_exec($conn,$query));
		else
			$result=(@odbc_exec($conn,$query));
			
		if ((odbc_num_rows($result)) != 1)
		{
			echo "Error: Non unique or missing record in function \"getone()\"<p>";
			exit;
		}
		return(odbc_fetch_object($result));
	}
	else
	{
		if ($link=="")
			$result=(@mssql_query($query));
		else
			$result=(@mssql_query($query,$link));
			
		if ((mssql_num_rows($result)) != 1)
		{
			echo "Error: Non unique or missing record in function \"getone()\"<p>";
			exit;
		}
		return(mssql_fetch_object($result));
	}
}
	

	
////////////////////////////////////////////////////////////////////////////////////////
function is_valid_viewpoint_job($job_num) {   
	$job_num=strtoupper(escapeshellcmd($job_num));
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
	return $job_res;
}

////////////////////////////////////////////////////////////////////////////////////////
function is_viewpoint_job_valid_and_open($job_num) {   
	$job_num=strtoupper(escapeshellcmd($job_num));
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

	$job_query1="select * from JCJM with (NOLOCK) where JCCo = 1 and JobStatus = 1 and Job = '$job_str_query1'"; 
	$job_res=ms_getoneb($job_query1);
    
	if ($job_res) {
		$job_res->job=$job_str_query1;
		$job_res->job_query=$job_query1;
		$job_res->Description=escapeshellcmd($job_res->Description);
	} else {
		if ($job_str_query2!="") {
			$job_query2="select * from JCJM with (NOLOCK) where JCCo = 1 and JobStatus = 1 and Job = '$job_str_query2'"; 
			$job_res=ms_getoneb($job_query2);
			if ($job_res) {
				$job_res->job=$job_str_query2;
				$job_res->job_query=$job_query2;
				$job_res->Description=escapeshellcmd($job_res->Description);
			}
		}
	}
	return $job_res;
}
	
/////////////////////////////////////////////////////////////////////////////////////////
function ms_getoneb($query,$link="")
{
	global $use_odbc;
	
	if ($use_odbc)
	{
		$user = 'reddwarf';
		$pass = 'cl01st3r';
		$server = 'hilly\viewpoint';
		$database = 'Viewpoint';
		
		// No changes needed from now on
		$connection_string = "DRIVER={SQL Server};SERVER=$server;DATABASE=$database"; 
		$conn = odbc_connect($connection_string,$user,$pass);
		
		
		$result=odbc_exec($conn, $query);
		
		if (odbc_num_rows($result) != 1)
			return FALSE;
		
		return odbc_fetch_object($result);
	}
	else
	{
		if ($link=="")
			$result=(@mssql_query($query));
		else
			$result=(@mssql_query($query,$link));
		
		return(mssql_fetch_object($result));
	}
}


function ms_csvdump($query,$header="1"){
$linefeedchar=chr(10);
$returnchar=chr(13);
$results=mssql_query($query);
if(!($results)) die ("Query Error: Could not run query:<br>$query");
	


header("Content-type: text/comma-seperated-file");
$width=mssql_num_fields($results);
while($row=(@mssql_fetch_row($results))) {
	$column=1;
	echo '"' . ereg_replace('"','""',ereg_replace($returnchar,"",ereg_replace($linefeedchar,"",$row[0]))) . '"';
	while ( $column < $width ){
		echo "," . '"' . ereg_replace('"','""',ereg_replace($returnchar,"",ereg_replace($linefeedchar,"",$row[$column]))) . '"';
		$column++;
		}
	echo "";
	}
}

//$job_num_p1=str_pad(ereg_replace("-.*","",$job_num),5,"0",STR_PAD_LEFT);
?>
