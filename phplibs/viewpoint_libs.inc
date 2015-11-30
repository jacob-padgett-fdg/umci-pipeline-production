<?php
////////////////////////////////////////////////////////////////////////////////////////
function ms_getone ($query,$link="")
{
	global $conn;
	//echo "<hr>Z<hr>";
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
	
	
/////////////////////////////////////////////////////////////////////////////////////////
function ms_getoneb($query,$link="")
{
	$user = 'reddwarf';
	$pass = 'cl01st3r';
	$server = 'hilly\viewpoint';
	$database = 'Viewpoint';
	
	// No changes needed from now on
	$connection_string = "DRIVER={SQL Server};SERVER=$server;DATABASE=$database"; 
	$conn = odbc_connect($connection_string,$user,$pass);
	
	
	//$serverName = "hilly\viewpoint"; //serverName\instanceName
	//$connectionInfo = array( "Database"=>"Viewpoint", "UID"=>"reddwarf", "PWD"=>"cl01st3r");
	//$conn = sqlsrv_connect( $serverName, $connectionInfo);
	
	//$result=sqlsrv_query($conn, $query);
	$result=odbc_exec($conn, $query);
	
	if (odbc_num_rows($result) != 1)
		return FALSE;
	
	return odbc_fetch_object($result);
	
	//if ($link=="")
		//$result=@sqlsrv_query($query);
//else $result=(@sqlsrv_query($query,$link));

	//if ((@sqlsrv_num_rows($result)) != 1)
	//{   
		//return FALSE;
	//}
	//return(sqlsrv_fetch_object($result));
	/*
	if ($link=="")
		$result=odbc_query($query);
	else $result=(odbc_query($query,$link));

	if ((odbc_num_rows($result)) != 1)
	{   
		return FALSE;
	}
	return(odbc_fetch_object($result));
	*/
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
