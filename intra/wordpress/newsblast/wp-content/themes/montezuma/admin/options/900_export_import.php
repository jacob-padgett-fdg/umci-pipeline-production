<?php 

return array(

'title'			=> 'Export / Import Settings',
'description' 	=> 'Export the Montezuma settings to import them into another WP/Montezuma installation, 
or import the settings that you exported from another Montezuma installation',

array(
	'id'	=> 'export_montezuma',
	'type' 	=> 'info',
	'title'	=> 'Export Settings',
	'std' 	=> '',
	'before' => '
	<p>These are the current Montezuma settings. You can save them  to import them into another WordPress / Montezuma installation.
	</p>
	<p><strong>1.</strong> Click into the textarea below, highlight everything (Ctrl+a on Win, Cmd+a on Mac) and 
	copy everything (Ctrl+c on Win, Cmd+c on Mac)</p>
	<p><strong>2.</strong> Open a new file in a text editor <strong style="font-size:20px">*</strong>, click into the blank file 
	and paste the contents of your clipboard into the file (Ctrl+v on Win, Cmd+v on Mac)</p>
	<p><strong>3.</strong> Save the file as a text file, e.g. as <code>montezuma-mydomain.txt</code>.</p>
	<strong>Current Montezuma Settings:</strong>
	<textarea spellcheck="false" style="height:400px">' . json_encode( get_option('montezuma') ) . '</textarea>
	
	',
	'after' => '<strong style="font-size:20px">*</strong> This must be text editor such as as "Notepad" on Windows or "TextEdit" on Mac. <strong>Do not use</strong> a word processor such as 
	MS Word, OpenOffice Writer etc...',

),

array(
	'id'	=> 'import_montezuma',
	'type' 	=> 'info',
	'title'	=> 'Import Settings',
	'std' 	=> '',
	'before' => 'Here you can import Montezuma settings that you exported from another (or this) Montezuma installation. 
	You should have saved those settings in a text file. The content of the file should look similar to (but be much more than) this example excerpt:<br>
	<code>{"favicon_url":"","meta_xua_compatible":"&lt;meta http-equiv=\"X-UA-Compatible\" ...</code>
	<textarea id="import_montezuma_textarea" spellcheck="false" style="height:400px"></textarea>
	<br>
	After you pasted the contents of your Montezuma settings file into the textarea above, click this button:<br>
	<button id="import_montezuma_button"><i></i>Import Settings</button>
	
	',
),


	
);