<?
function listjobs($query,$show_letter_jobs=0,$showtraxx=0) {
	global $fd_color_4,$fd_color_3;
	$res=@mysql_query($query);
	if (!($res)) die("ERROR: Query failed. Failed query:<hr>$query");
	$count=@mysql_num_rows($res);
	if ($count < 1) {
		echo "<b>No Records Found...</b>";
		return 0;
		}
	if ($showtraxx==1) $colcount=9;
		else $colcount=8;
	echo "<table border=1 cellspacing=0 cellpadding=4>
	<tr><td colspan=$colcount bgcolor=$fd_color_4 align=center><h3>Matching Jobs</h3></td></tr><tr><td colspan=$colcount><font size=-3>&nbsp;</font></td></tr>
	<tr bgcolor=$fd_color_3><td>
		Job #
	</td><td>
		Job Name
	</td><td>
		Orig Contract Amt
	</td><td>
		Final Contract Amt
	</td><td>
		Contract Type
	</td><td>
		Dsg/Build
	</td><td>
		Department
	</td><td>
		Complete Date
	</td></tr>
	";
	
	while ($row=@mysql_fetch_object($res)) listjobs_row($row,$show_letter_jobs,$showtraxx);
	
	echo "</table>";
	}

function listjobs_row($object,$show_letter_jobs=0,$showtraxx=0) {
		if ((!($show_letter_jobs)) && (!(ereg("^[0-9]",$object->contract_num)))) return 0;
			
		if ($object->active=='Y') $bgcolor="#ffffff";
		else $bgcolor="#eeeeee";
		
		$object->complete_date=invali_date($object->complete_date);
		$job_type_num=ereg_replace(" .*$","",$object->job_type);
		echo "<tr bgcolor=$bgcolor><td>
				<a href=$pagename?mode=jobinfo_edit&jobinfo_id=$object->jobinfo_id><font color=blue>$object->contract_num</font></a>
			</td><td>
				$object->job_name
			</td><td>
				$object->contract_amount_orig
			</td><td>
				$object->contract_amount_final
			</td><td>
				$object->contract_type&nbsp;
			</td><td>
				$object->design_build
			</td><td>
				$job_type_num
			</td><td>
				$object->complete_date&nbsp;
			";
			
			if ($showtraxx==1) {
				$traxxcount=getoneb("select sum(1) as count from bidlist where jobinfo_id = '$object->jobinfo_id'");
				if ($traxxcount->count=="") $traxxcount->count="&nbsp";
				echo "</td><td>$traxxcount->count";
				}
			
			
			echo "</td></tr>";
	

	}

?>
