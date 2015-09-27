<?
$json = $_POST['data'];
$_SESSION['submittallog_params'] = json_decode($json);
return true;
