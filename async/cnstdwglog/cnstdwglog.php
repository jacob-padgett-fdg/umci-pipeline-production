<?php


require("cnfgdwglog.cfg");
require("global-auth.inc");

$jobinfo_id = $_GET["jobinfo_id"];

$current_cnstdwglog_section = $_GET["section"];
//die($current_cnstdwglog_section);
$last_issuance_id = $_GET["issuance_id"];
$query = "select * from cnstdwglog right join documents on ( doc_type = 'cnstdwglog' and documents.app_record_id = cnstdwglog.cnstdwglog_id ) where ( cnstdwglog.jobinfo_id = '$jobinfo_id' and cnstdwglog.section = '$current_cnstdwglog_section') order by cnstdwglog.drawing_type, documents.sort_rank, documents.doc_id, cnstdwglog.drawing_num";
//die($query);
//tabledump ("explain " . $query);die;
$res = @mysql_query($query);
$last_drawing_type = '';

$payload =  " { \"data\": [ ";
$first_time = true;
while ($dwg = @mysql_fetch_object($res)) {
    if (!$first_time) {
        $payload .= ",";
    } else {
        $first_time = false;
    }
    $payload .= " [ ";

    $cookie_link = batch_this_document($dwg->doc_id);
    //////////////////////////////
    // look at this next line and
    // include jobinfo_id to pre-
    // vent errors in display
    //////////////////////////////
    $cnstdwglog_info = getoneb("select * from cnstdwglog_files right join cnstdwglog_issuances on (cnstdwglog_issuances.issuance_id = cnstdwglog_files.issuance_id) right join webfile_groups on ( cnstdwglog_files.file_group_id = webfile_groups.file_group_id ) right join webfile_files on ( webfile_groups.file_group_id = webfile_files.file_group_id ) where cnstdwglog_id = '$dwg->cnstdwglog_id' and cnstdwglog_issuances.issuance_id = cnstdwglog_files.issuance_id order by cnstdwglog_issuances.issuance_type, cnstdwglog_issuances.issuance_date desc, cnstdwglog_issuances.sort_priority desc, cnstdwglog_issuances.name desc limit 1");
    $paperclip_link = webfile_paperclip_view($cnstdwglog_info->file_group_id, "&nbsp;", false);
   if (!empty($last_issuance_id)) {
        $last_file_info = getoneb("select * from cnstdwglog_files where cnstdwglog_id = '$dwg->cnstdwglog_id' and issuance_id = '$last_issuance_id'");
    }
    if (!empty($last_file_info)) {
        $addenda_files = getoneb("select sum(1) as count from cnstdwglog_addenda where files_id = '$last_file_info->files_id' and required_in_this_issuance = 'Y' and posted = 'N'");
    }

    $drawing_num_text = $dwg->drawing_num;
    $drawing_num_text = $drawing_num_text;
    if ($drawing_num_text == "") $drawing_num_text = "??????";
    if ($write) {
        $edit_record_link = "<a href=$pagename?mode=cnstdwglog_edit&cnstdwglog_id=$dwg->cnstdwglog_id>$drawing_num_text</a>";
        if (!isset($addenda_files)) {
            $addenda_files = (object)array();
            $addenda_files->count = 0;
        }
        $addenda_count = "<a href='$pagename?mode=cnstdwglog_edit&cnstdwglog_id=$dwg->cnstdwglog_id&showmode=current_only'>$addenda_files->count</a>";
    } else {
        $edit_record_link = "$drawing_num_text";
        $addenda_count = "$addenda_files->count";
    }

    $bgcolortext = "";
    if ($cnstdwglog_info->issuance_type == "Design") $bgcolortext = " style='width:100%;background-color:$fd_color_6''";
    if ($cnstdwglog_info->issuance_type == "Construction Orig Issue") $bgcolortext = " style='width:100%;background-color:$fd_color_5''";
    if ($cnstdwglog_info->issuance_type == "") $bgcolortext = " style='width:100%;background-color:$fd_color_2'";

    $payload .= "\"<table border=0 cellspacing=0 cellpadding=1><tr><td width=10px>$paperclip_link</td><td>$cookie_link</td></tr></table>\", ";
    $payload .= "\"$addenda_count&nbsp;\", ";
    $payload .= "\"$edit_record_link\", ";
    $payload .= "\"<font size=-1>$dwg->description</font>\", ";
    $payload .= "\"$cnstdwglog_info->revision\", ";
    $payload .= "\"<div $bgcolortext>$cnstdwglog_info->name</div>\", ";
    $payload .= "\"$dwg->drawing_type\"";

    if ($_SESSION['cnstdwglog_view'] == "expanded") {
        // Just revised to hide errant extra data.. There are extra cnstdwglog_files items from a different job showing up.. they have
        // the cnstdwglog_id from a record in job a, but have a issuance_id from a issuance in job b - must look into.
        $issuances_res = @mysql_query("select * from cnstdwglog_files right join cnstdwglog_issuances on (cnstdwglog_issuances.issuance_id = cnstdwglog_files.issuance_id) left join webfile_groups on ( cnstdwglog_files.file_group_id = webfile_groups.file_group_id ) left join webfile_files on ( webfile_groups.file_group_id = webfile_files.file_group_id ) where cnstdwglog_files.cnstdwglog_id = '$dwg->cnstdwglog_id' and cnstdwglog_issuances.issuance_id = cnstdwglog_files.issuance_id and cnstdwglog_issuances.jobinfo_id = '{$_SESSION['global_jobinfo_id']}' group by cnstdwglog_issuances.issuance_id order by cnstdwglog_issuances.issuance_type desc, cnstdwglog_issuances.issuance_date, cnstdwglog_issuances.sort_priority, cnstdwglog_issuances.name");
        $rowcount = @mysql_num_rows($issuances_res);
        while ($issuance_row = @mysql_fetch_object($issuances_res)) {
            $paperclip_link = webfile_paperclip_view($issuance_row->file_group_id, "&nbsp;", true);
            $bgcolortext = "";
            if ($paperclip_link != "&nbsp;") {
                if ($cnstdwglog_info->issuance_type == "Design") $bgcolortext = " style='width:100%;background-color:" . $fd_color_6 . "'";
                if ($cnstdwglog_info->issuance_type == "Construction Orig Issue") $bgcolortext = " style='width:100%;background-color:" . $fd_color_5 . "'";
            }
            $payload .=  ", \"<div $bgcolortext>$paperclip_link</div>\"";
        }
    }
    $payload .= " ]";
}
$payload .= " ] }";

echo $payload;
