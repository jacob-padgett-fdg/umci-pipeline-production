<?PHP
session_start();

global $use_odbc;
global $global_user;

$use_odbc = 0;

require_once("settings.cfg");
require_once("querylib.inc");
require_once("global-auth.inc");

$jobNumber = $_REQUEST["jobNumber"];
$phase = $_REQUEST["phase"];

if ($jobNumber == "Sick" || $jobNumber == "Vacation" || $jobNumber == "Holiday" || $jobNumber == "Training")
{
    print true;
}
else
{
    //check last character
    $lastCharacter = substr($jobNumber,strlen($jobNumber) - 1);
    
    if ($lastCharacter != "-")
        $jobNumber .= "-";
    
    if ($global_contacts_id='353'||$global_contacts_id=='4517'||$global_contacts_id='2'||$global_contacts_id='1') include("viewpoint_connect_ro_pr.phtml");
    else include("viewpoint_connect_ro.phtml");
    
    $phases_locked_sql = "select * from JCJM with (NOLOCK) where JCCo = 1 and Job = '$jobNumber' and LockPhases = 'Y'";
    
    //$phases_locked=ms_getoneb("select * from JCJM with (NOLOCK) where JCCo = 1 and Job = '$jobNumber' and LockPhases = 'Y'");
    $sql = "";
    
    /*if ($phases_locked)
    {
        $sql="select Phase,Description from JCJP with (NOLOCK) where Job = '$jobNumber' and JCCo = 1 and ActiveYN = 'Y'";
    }
    else
    {
        $sql="select * from JCPC with (NOLOCK),JCPM with (NOLOCK) where JCPC.JCCo = JCPM.JCCo and JCPC.JCCo = 1 and JCPC.Phase = JCPM.Phase and JCPC.CostType = 1 and JCPC.PhaseGroup = 1 and JCPM.PhaseGroup = 1";
    }*/
    
    $resultArray = array();
    
    if ($use_odbc)
    {
        $phases_locked_result = odbc_exec($conn,$phases_locked_sql);
        if (odbc_num_rows($phases_locked_result) > 0)
            $sql="select Phase,Description from JCJP with (NOLOCK) where Job = '$jobNumber' and JCCo = 1 and ActiveYN = 'Y'";
        else
            $sql="select Phase from JCPC with (NOLOCK),JCPM with (NOLOCK) where JCPC.JCCo = JCPM.JCCo and JCPC.JCCo = 1 and JCPC.Phase = JCPM.Phase and JCPC.CostType = 1 and JCPC.PhaseGroup = 1 and JCPM.PhaseGroup = 1";
            
        $result = odbc_exec($conn,$sql);
        while ( $row = odbc_fetch_array($result))
        {
            array_push($resultArray, $row['Phase']);
        }
    }
    else
    {
        $phases_locked_result = mssql_query($phases_locked_sql);
        if (mssql_num_rows($phases_locked_result) > 0)
            $sql="select Phase,Description from JCJP with (NOLOCK) where Job = '$jobNumber' and JCCo = 1 and ActiveYN = 'Y'";
        else
            $sql="select * from JCPC with (NOLOCK),JCPM with (NOLOCK) where JCPC.JCCo = JCPM.JCCo and JCPC.JCCo = 1 and JCPC.Phase = JCPM.Phase and JCPC.CostType = 1 and JCPC.PhaseGroup = 1 and JCPM.PhaseGroup = 1";
    
        $result = mssql_query($sql);
        while ( $row = mssql_fetch_array($result))
        {
            array_push($resultArray, $row['Phase']);
        }
    }
    
    //print_r( $resultArray );
    $phase = $phase . "-   -";
    
    if (in_array($phase,$resultArray))
        print true;
    else
        print false;   
}

?>