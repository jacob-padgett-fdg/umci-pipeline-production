<?
$json = $_POST['data'];
$_SESSION['cnstdwglog_params'] = json_decode($json);
return true;