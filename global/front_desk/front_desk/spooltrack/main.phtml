<?
require("header.phtml");
fd_navs_header();
//require("tableprint.phtml");

//echo "<div style=\"float: left; border: 2px solid black; padding: 20px;\">";menu();echo "</div>";
echo "<table border=2 cellspacing=0 cellpadding=20><tr valign=top><td>";
require("system_chooser.phtml");echo "</td><td>";
menu();echo "</td></tr><table>";

//require("system_chooser.phtml");

session_register('current_system_id');
session_register('orderbyfield');
session_register('spooltrackviewall');

if ($spooltrackviewall=="") $spooltrackviewall="no";
if ($orderbyfield_set!="") {
	$orderbyfield_set=escapeshellcmd($orderbyfield_set);
	if ($orderbyfield_set!=$orderbyfield)
		$orderbyfield=$orderbyfield_set;
		else
		$orderbyfield="$orderbyfield_set desc";
	}
if ($orderbyfield=="") $orderbyfield="spool_fab_tracking_id";




if ($spooltrackviewall == "no") {
	$query="select * from spool_fab_tracking where jobinfo_id = '$global_jobinfo->jobinfo_id' and system_id = '$current_system_id' order by $orderbyfield";
	//menu();
} else {
	$query="select * from spool_fab_tracking where jobinfo_id = '$global_jobinfo->jobinfo_id' order by $orderbyfield";
	echo "<a href=$pagename?mode=toggleall><font color=blue>Return to Normal View</font></a>";
}




if (!($system_information))
	echo "<p><b><i>Please select a system for this job</i></b> (upper right hand corner)<br>";
	else
	if ($batch_mode!="Y")
		spooltableprint($query,$dynamic_field_info);
	else
		spooltableprint_batch($query,$dynamic_field_info);
	


//echo "hi";
fd_navs_footer();

//require('footer.phtml');
?>
