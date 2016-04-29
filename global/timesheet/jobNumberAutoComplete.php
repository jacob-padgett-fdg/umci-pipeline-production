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

// get what user typed in autocomplete input
$term = trim($_GET['term']);
 
$a_json = array();
$a_json_row = array();
 
$a_json_invalid = array(array("id" => "#", "value" => $term, "label" => "Only letters and digits are permitted..."));
$json_invalid = json_encode($a_json_invalid);
 
// replace multiple spaces with one
$term = preg_replace('/\s+/', ' ', $term);
 
// SECURITY HOLE ***************************************************************
// allow space, any unicode letter and digit, underscore and dash
if(preg_match("/[^\040\pL\pN_-]/u", $term)) {
  print $json_invalid;
  exit;
}
// *****************************************************************************
 
$parts = explode(' ', $term);
$p = count($parts);
 
/**
 * Create SQL
 */

$sql = "select top 100 * from JCJM with (NOLOCK) where JobStatus = 1 and JCCo = 1 ";

if ($p == 0)
{
    $sql .= " and Job not like '00000%' and Job not like 'M%' and Job not like 'E%' and Job not like '1%' and Job not like '2%' and Job not like '3%' and Job not like '4%' and Job not like '5%' and Job not like '6%' and Job not like '7%' and Job not like '8%' and Job not like '9%' ";
}
else
{
    for($i = 0; $i < $p; $i++)
    {
        $sql .= " and ( LOWER(Description) LIKE LOWER('%".addslashes($parts[$i])."%') OR Job LIKE '".addslashes($parts[$i])."%' ) ";
    }   
}

if ($use_odbc)
{
    $result = odbc_exec($conn,$sql);
    while ( $row = odbc_fetch_array($result))
    {
        $a_json_row["id"] = $row['Job'];
        $a_json_row["value"] = $row['Job'];
        $a_json_row["label"] = $row['Job']  . $row['Description'];
        array_push($a_json, $a_json_row);
        //array_push($a_json, $row);
    }
}
else
{
    $result = mssql_query($sql);
    while ( $row = mssql_fetch_array($result))
    {
        $a_json_row["id"] = $row['Job'];
        $a_json_row["value"] = $row['Job'];
        $a_json_row["label"] = $row['Job']  . $row['Description'];
        array_push($a_json, $a_json_row);
        //array_push($a_json, $row);
    }
}
 
$json = json_encode($a_json);
print $json;
?>