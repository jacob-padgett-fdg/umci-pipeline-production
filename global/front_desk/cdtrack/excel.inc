<? if ($excellib != 1) {
$excellib=1;
require("excellib.cfg");

#######################################################################
#	Function to dump a table that results from $query 
#	in an HTML format, with all text clickable for editing..
#######################################################################

function exceltabledump($query){

global $excellib_clicktext,$excellib_highlight,$excellib_highlight_bg,$excellib_oddrow,$excellib_evenrow;

$results=mysql_query("$query");

if(!($results)) {
	include("updateerror.phtml");
	exit;
	}

$tablename=eregi_replace("^.* from ","",$query);
$tablename=eregi_replace(" .*$","",$tablename);

$width=mysql_num_fields($results);

echo "<table border=1><tr>";
$column=1;
$IDNAME=mysql_field_name($results,0);

while ($column < $width) {
	$field=mysql_field_name($results,$column);
	echo"<td bgcolor=$excellib_highlight_bg valign=top>";
	addspec($tablename,$field);
	echo"<b><font color=$excellib_highlight>$field</font></b></td>";
	$column++;
	}
echo "</tr>";

$column=1;
$rownum=0;
while($row=(@mysql_fetch_row($results))) {
	$rownum++;
	$column=1;
	$IDVAL=$row[0];
	if ( ($rownum % 2) == 0 ) { $cellbg=$excellib_evenrow; }
					else	  { $cellbg=$excellib_oddrow; }	
	echo "<tr>";
	while ( $column < $width ){
		echo "<td bgcolor=$cellbg><a href=$pagename?mode=${tablename}_edit&$IDNAME=$IDVAL>
			<font color=black>";
		if ($row[$column]=="0000-00-00") $row[$column]="";
		if ($row[$column]=="") $row[$column]="<center>_</center>";
		echo "$row[$column]";
		echo "</a></font></td>";
		$column++;
		}
	echo "</tr>";
	}
echo "</table>";

}


//////////////////////////////////////////////////////////////////////
//
//	New functionality that emulates some excel options (filters)
//
//////////////////////////////////////////////////////////////////////

if ($add2query != "") {
	$tmp=explode(" ",$add2query);
	if ($querypassed != "") {	
		if (eregi(" $tmp[0] $tmp[1] $tmp[2] ",$querypassed)) {
			$querypassed=eregi_replace(" $tmp[0] $tmp[1] $tmp[2] '[^']*'"," ",$querypassed);
			}
		if ($tmp[3]=="''") 
			$query="$querypassed";
		else
			$query="$querypassed $add2query";
		}
	}


function addspec ($spectable,$colname){
global $pagename, $mode, $query;

if (eregi(" as '$colname'[ ,]",$query)) {
	$colname=eregi_replace(" *as *'$colname'.*$","",$query);
	$colname=eregi_replace("^.*[, ]","",$colname);
	}

$queryb="select $colname,$colname from $spectable group by $colname";
if (eregi(" and $colname = ",$query)) {
	$nofront=eregi_replace("^.*$colname = '","",$query);
	$noback=eregi_replace("'.*$","",$nofront);
	}
echo "

<form name=$colname action=$pagename method=post>
<input type=hidden name=querypassed value=\"$query\">
<input type=hidden name=mode value=$mode>
<input type=hidden name=add2query value=\"\">
";dropbox2($queryb,"$noback",1,"${colname}f();",1);echo"
</form>

<script>
function ${colname}f () {
	var temp=document.$colname.$colname.options[document.$colname.$colname.selectedIndex].value;
	temp = 'and $colname = \'' + temp + '\'';
	document.$colname.add2query.value=temp;
	document.$colname.submit();
	}

</script>

";

}


#######################################################################
#	End of functions, put the next one here.
#######################################################################

}?>
