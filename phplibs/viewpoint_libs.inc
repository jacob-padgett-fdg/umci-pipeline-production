<?
/////////////////////////////////////////////////////////////////////////////////////////
function ms_getone ($query,$link="") {
//echo "<hr>Z<hr>";	
if ($link=="") $result=(@mssql_query($query));
	else $result=(@mssql_query($query,$link));
if ((mssql_num_rows($result)) != 1) {   
	echo "Error: Non unique or missing record in function \"getone()\"<p>";
	exit;
	}
return(mssql_fetch_object($result));
}
                         
/////////////////////////////////////////////////////////////////////////////////////////
function ms_getoneb($query,$link="") {
if ($link=="") $result=@mssql_query($query);
else $result=(@mssql_query($query,$link));
if ((@mssql_num_rows($result)) != 1) {   
	return FALSE;
	}
return(mssql_fetch_object($result));
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
