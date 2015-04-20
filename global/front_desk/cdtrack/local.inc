<?

#######################################################################
#	Function to dump a table that results from $query 
#	in an HTML format...
#######################################################################

function tabledump2($query){

$results=mysql_query("$query");
if(!($results)) {
	include("updateerror.phtml");
	exit;
	}
$width=mysql_num_fields($results);
echo "<table border=1><tr><td bgcolor=#444444 color=#ffffff><font color=#ffffff><b>Edit</b></font></td>";
$column=1;
while ($column < $width) {
	$field=mysql_field_name($results,$column);
	echo"<td bgcolor=#444444><b><font color=#ffffff>$field</font></b></td>";
	$column++;
	}
echo "</tr>";

while($row=(@mysql_fetch_row($results))) {
	$column=0;
	$column=1;
	echo "<tr><td><a href=$pagename?mode=jobinfo_edit&cd_jobinfo_id=$row[0]>Edit</a></td>";
	while ( $column < $width ){
		echo "<td>";
		echo "$row[$column]";
		echo "</td>";
		$column++;
		}
	echo "</tr>";
	}
echo "</table>";

}



?>
