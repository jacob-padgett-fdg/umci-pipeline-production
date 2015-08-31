<?php return array(

'title'			=> 'Admin Settings',
'description' 	=> 'Configure the admin area',

array(
	'id'	=> 'code_font_family',
	'type' 	=> 'text',
	'title'	=> 'Font used for code in text areas',
	'std' 	=> 'consolas, monaco, monospace',
	'after' => ' &nbsp;<span class="arrow-left">&nbsp;</span> <strong>Font Family</strong> of code in text areas. 
	Default: <code>consolas, monaco, monospace</code><br>Separate with comma, and put font names with spaces inside quotes: <code>arial, "times new roman", verdana</code> ',
	'style'		=> 'width:250px',
),

array(
	'id'	=> 'code_font_size',
	'type' 	=> 'text',
	'title'	=> '',
	'std' 	=> 13,
	'after' => ' px &nbsp;<span class="arrow-left">&nbsp;</span> <strong>Font Size</strong> of code in text areas. Default: <code>13</code>',
	'style'		=> 'width:30px',
	'group'		=> true
),

array(
	'id'	=> 'code_line_height',
	'type' 	=> 'text',
	'title'	=> '',
	'std' 	=> 18,
	'after' => ' px &nbsp;<span class="arrow-left">&nbsp;</span> <strong>Line Height</strong> of code in text areas. Default: <code>18</code>',
	'style'		=> 'width:30px',
	'group'		=> true
),

	
);