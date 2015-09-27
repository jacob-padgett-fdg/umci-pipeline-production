<?
$json = $_POST['data'];
$_SESSION['drawinglog_params'] = json_decode($json);
return true;
