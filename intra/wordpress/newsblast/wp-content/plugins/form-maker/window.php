<?php
$path  = ''; // It should be end with a trailing slash  
if ( !defined('WP_LOAD_PATH') ) {

	/** classic root path if wp-content and plugins is below wp-config.php */
	$classic_root = dirname(dirname(dirname(dirname(__FILE__)))) . '/' ;
	
	if (file_exists( $classic_root . 'wp-load.php') )
		define( 'WP_LOAD_PATH', $classic_root);
	else
		if (file_exists( $path . 'wp-load.php') )
			define( 'WP_LOAD_PATH', $path);
		else
			exit("Could not find wp-load.php");
}

// let's load WordPress
require_once( WP_LOAD_PATH . 'wp-load.php')
?>
<html xmlns="http://www.w3.org/1999/xhtml"><head>
	<title>Form Creator</title>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<script language="javascript" type="text/javascript" src="<?php echo get_option("siteurl"); ?>/wp-includes/js/jquery/jquery.js"></script>
	<script language="javascript" type="text/javascript" src="<?php echo get_option("siteurl"); ?>/wp-includes/js/tinymce/tiny_mce_popup.js"></script><link rel="stylesheet" href="<?php echo get_option("siteurl"); ?>/wp-includes/js/tinymce/themes/advanced/skins/wp_theme/dialog.css?ver=342-20110630100">
	<script language="javascript" type="text/javascript" src="<?php echo get_option("siteurl"); ?>/wp-includes/js/tinymce/utils/mctabs.js"></script>
	<script language="javascript" type="text/javascript" src="<?php echo get_option("siteurl"); ?>/wp-includes/js/tinymce/utils/form_utils.js"></script>
	<base target="_self">
</head>
<body id="link"  style="" dir="ltr" class="forceColors">
<table>
<tr>
<td style="height:100px; vertical-align:top;">
Select a Form 
</td>
<td style="vertical-align:top">
<select name="Form_Makername" id="Form_Makername" style="width:150px;">
<option value="- Select Form -" selected="selected">- Select a Form -</option>
<?php    $ids_Form_Maker=$wpdb->get_results("SELECT * FROM ".$wpdb->prefix."formmaker order by title",0);
	   foreach($ids_Form_Maker as $arr_Form_Maker)
	   {
		   ?>
           <option value="<?php echo $arr_Form_Maker->id; ?>"><?php echo $arr_Form_Maker->title; ?></option>
           <?php }?>
</select>
</td>
</tr>
<tr>
<td>
</td>
<td style="text-align:right">

	<div class="mceActionPanel">
		<div style="float: right">
			<input type="button" id="insert" name="insert" value="<?php _e("Insert", 'flag'); ?>" onClick="insert_Form_Maker()" />
		</div>
	</div>
    </td>
    </tr>
</table>
<script type="text/javascript">
function insert_Form_Maker() {
	if(document.getElementById('Form_Makername').value=='- Select Form -')
	{
		tinyMCEPopup.close();
	}
	else
	{
	   var tagtext;
	   tagtext='[Form id="'+document.getElementById('Form_Makername').value+'"]';
	   window.tinyMCE.execInstanceCommand('content', 'mceInsertContent', false, tagtext);
	   tinyMCEPopup.editor.execCommand('mceRepaint');
	   tinyMCEPopup.close();		
	}
	
}

</script>
</body></html>
<?php
?>