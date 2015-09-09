<?php


require("cnfgdwglog.cfg");
require("global-auth.inc");

if ($global_jobinfo) {
    $query = "select * from drawinglog where jobinfo_id = '$global_jobinfo->jobinfo_id' and section = '{$_SESSION['current_drawinglog_section']}' $query_search_add order by type,sort_priority,sheet_num";
    $res = @mysql_query($query);

    while($row=@mysql_fetch_object($res)) {
        $temp_doc_id_query="select * from documents where doc_type = 'drawinglog' and section = '$row->section' and app_record_id = '$row->drawing_id'";

        $temp_doc_info=getoneb($temp_doc_id_query);
        $temp_doc_id=$temp_doc_info->doc_id;

        $stbg="#ffffff";
        if ($row->submittal_status=='N/A') {
            $stbg="#dddddd";
        }

        $s_sep="";
        $st_text="";
        if ($row->submitted_to_umc=='Y') { $st_text=$st_text . $s_sep . "umc";$s_sep=","; }
        if ($row->submitted_to_gc=='Y') { $st_text=$st_text . $s_sep . "gc";$s_sep=","; }
        if ($row->submitted_to_owner=='Y') { $st_text=$st_text . $s_sep . "owner";$s_sep=","; }
        if ($row->submitted_to_consultant=='Y') { $st_text=$st_text . $s_sep . "consultant";$s_sep=","; }
        if ($st_text=="") $st_text="&nbsp;";
        if (getoneb("select * from drawinglog_notes where drawing_id = '$row->drawing_id' limit 1")) {
            $notesimage="<img src=/images/note.gif>";
        } else {
            $notesimage="&nbsp;";
        }
        $orig_issue=invali_date($row->orig_issue,'/');
        $current_submittal_date=invali_date($row->current_submittal_date,'/');
        $const_issue=invali_date($row->issued_for_construction_date,'/');
        $current_rev_date=invali_date($row->current_rev_date,'/');

        if ($row->submitted_to_eng!="0000-00-00") $ste="X";
        else $ste="&nbsp;";

        if ($row->returned_from_eng!="0000-00-00") $rfe="X";
        else $rfe="&nbsp;";

        $rr="$row->correction_required";
        if (($rr=="N")||($rr=="")) $rr="N";

        $current_rev=getoneb("select * from drawinglog_revs where drawing_id = '$row->drawing_id' order by revision desc,rev_date desc, dwg_rev_id desc limit 1");
        if ($current_rev->sent_to_field_date!="0000-00-00") $stf="X";
        else $stf="&nbsp;";

        $file_paperclip=webfile_paperclip_download($current_rev->attached_file_group_id);
        $file_batch=batch_this_document($temp_doc_id);

        if ($current_rev_date=="") $current_rev_date="&nbsp;";
        if ($row->sheet_num=="") $row->sheet_num='&lt;?&gt;';
        $bgcolor="#ffffff";
        $note_addendum="";
        if ($row->drawing_is_current=='N') {
            $bgcolor="#ffaaaa";
            $note_addendum="<br>**** Warning, the drawing respresented here is not up to date. Please contact detailing for details ******";
        }
        echo "

		<tr bgcolor=$bgcolor id='$row->drawing_id'><td>
				<table border=0 cellspacing=0 cellpadding=0><tr><td>$file_paperclip&nbsp;</td><td>$file_batch</td></tr></table>
			</td><td id=\"is_current_$row->drawing_id\">
				<a href=\"javascript:toggle_is_current($row->drawing_id,'$row->drawing_is_current')\">$row->drawing_is_current</a>
			</td><td id=\"area_$row->drawing_id\">
				$row->area
			</td><td>";
        if (($write)||(($guest)&&($_SESSION['global_user_id']==$row->creator))) {
            echo "<a href=$pagename?mode=drawinglog_edit&drawing_id=$row->drawing_id>$row->sheet_num</a>";
        } else {
            echo "$row->sheet_num";
        }
        echo "
			</td>
			<td id=\"description_$row->drawing_id\" onclick=quick_form('description',this)>
				$row->description
			</td><td>
				$row->revision
			</td><td>
				$current_rev_date
			</td><td>
				$orig_issue
			</td><td>
				$const_issue
			</td><td align=center bgcolor=\"$stbg\">
				$ste
			</td><td align=center bgcolor=\"$stbg\">
				$rfe
			</td><td align=center bgcolor=\"$stbg\">
				$rr
			</td><td>
				$row->submittal_status
			</td><td>
				$current_submittal_date
			</td><td align=center>
				$stf
			</td><td align=center>
				$st_text
			</td><td>$notesimage$note_addendum
			</td><td>$row->type</td></tr>";
    }
}