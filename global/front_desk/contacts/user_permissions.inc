<?
$authqry="select * from access where 
contacts_id = '$global_user_id' and 
application_name = '$application_name' 
and rightsmode = 'grant'";

$authres=@mysql_query($authqry);
if ((mysql_num_rows($authres))!=1) {
	$authres=0;
	} else {
	$authres=@mysql_fetch_object($authres);
	}
//if (!($authres)) {
//	echo "Error: Invalid authentication information for this database!<hr><p>
//	<a href=$pagename_up><font color=blue>EXIT</font></a><br>
//	";
//	exit;
//	}

$application_user_type=$authres->type;

if ($application_user_type=="admin")
	$adminuser=1;
	else
	$adminuser=0;
?>
