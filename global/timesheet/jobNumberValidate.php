<?PHP
session_start();

global $use_odbc;
global $global_user;

$use_odbc = 0;

require_once("settings.cfg");
require_once("querylib.inc");
require_once("global-auth.inc");

if ($global_contacts_id='353'||$global_contacts_id=='4517'||$global_contacts_id='2'||$global_contacts_id='1') include("viewpoint_connect_ro_pr.phtml");
else include("viewpoint_connect_ro.phtml");

$jobNumber = $_REQUEST["jobNumber"] . "-";
$resultArray = array();


$sql = "SELECT * FROM JCJM WITH (NOLOCK) WHERE Job = '$jobNumber'";

if ($use_odbc)
{
    $result = odbc_exec($conn,$sql);
    if (odbc_num_rows($result) == 0)
    {
        $resultArray["job"] = false;
        $resultArray["open"] = false;
    }
    else
    {
        while ( $row = odbc_fetch_array($result))
        {
            $resultArray["job"] = true;
            if ($row["JobStatus"] == 1)
                $resultArray["open"] = true;
            else
                $resultArray["open"] = false;
        }
    }
}
else
{
    $result = mssql_query($sql);
    if (mssql_num_rows($result) == 0)
    {
        $resultArray["job"] = false;
        $resultArray["open"] = false;
    }
    else
    {
        while ( $row = mssql_fetch_array($result))
        {
            $resultArray["job"] = true;
            if ($row["JobStatus"] == 1)
                $resultArray["open"] = true;
            else
                $resultArray["open"] = false;
        }
    }
}

echo json_encode( $resultArray );
?>