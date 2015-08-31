<?php 

$montezuma = get_option( 'Montezuma' );
/**********************************************

// actual CSS files should have type codemirror.
// Do not add type "codemirror" to non files 
in 400_css_files.php
See admin.php
***********************************************/

$cssfiles = array(
	'title'			=> 'CSS',
	'description' 	=> 'For referencing background or other images use the following placeholders:
	<ul>
	<li><code>%tpldir%</code> = Template Directory = http://www.yourdomain.com/wp-content/themes/Montezuma</li>
	<li><code>%tplupldir%</code> = Template\'s own folder inside WP Uploads = http://www.yourdomain.com/wp-content/uploads/Montezuma</li>
	<li><code>%upldir%</code> = Default WordPress Uploads directory = http://www.yourdomain.com/wp-content/uploads</li>
	<li><span class="closemirror">Close Mirror</span></li>
	'
);


foreach ( glob( get_template_directory() . "/admin/default-templates/css/*.css") as $filepath) {

	$filename = basename( $filepath );
	$filename = substr( $filename, strpos( $filename, '-') + 1 );
	
	$file_ID = str_replace(
		array( '.css', '-' ),
		array( '', '_' ),
		$filename );
	

	$thisfile = array(
		'id' => 'css_' . $file_ID,
		'type' => 'codemirror',
		'title' => $file_ID .  '<span style="color:#666">.css</span>',
		// 'std' => file_get_contents( $filepath )
		// Alternative to file_get_contents:
		'std' => implode( "", file( $filepath ) ),
		'before' => '
For referencing background or other images use the following placeholders:

	<ul>
	<li><code id="testclip">%tpldir%</code>  = Template Directory = http://www.yourdomain.com/wp-content/themes/Montezuma</li>
	<li><code>%tplupldir%</code> = Template\'s own folder inside WP Uploads = http://www.yourdomain.com/wp-content/uploads/Montezuma</li>
	<li><code>%upldir%</code> = Default WordPress Uploads directory = http://www.yourdomain.com/wp-content/uploads</li>
	</ul>

',
		'codemirrormode' => 'css'
	);
	$cssfiles[] = $thisfile;
}


return $cssfiles;