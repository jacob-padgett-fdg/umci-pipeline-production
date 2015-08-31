<?php 

$montezuma = get_option( 'Montezuma' );

$pagetemplatefiles = array(
	'title'			=> 'Edit Main Templates',
	'description' 	=> 'Edit the default WordPress page templates. '
);


/*<h3>WordPress Terminology</h3>

<p class="colcount3">	
Understanding WordPress, and Montezuma, is mostly a matter of understanding words like 
<strong>Page</strong>, <strong>Static Page</strong> and <strong>Post</strong>. Many of these 
words aren\'t technical in nature - you just need to know their meaning.
Sometimes the same words will mean different things based on the context in which they are written, and different people 
 use different words for the same thing. For instance, 
how do you call a <strong>page</strong> in WordPress that displays 
more than one post - contrary to one that displays just one post? 
I like to call them <strong>Multi Post Pages</strong> but others call them 
<strong>non-singular pages</strong>, a term I\'ve seen on an official wordpress.org page as well. A quick Google search 
inidcates that <strong>Multi Post Pages</strong> might be the more popular term for this but ironically many of the 
search results appear to be related to Montezuma. A third option could be to call them <strong>index pages</strong> 
because these multi post pages typically display an index of e.g. "all posts in the category X", but <strong>index page</strong> 
would probably be confusing because there is also the template with the file name <strong>index.php</strong>. 
So keep in mind that if there\'s something you don\'t understand it is probably rather a semantic issue than a technical one. 
</p>
*/

// Don't use 'maintemplate-...' as ID for 'info' items, should be reserved for actual templates

$add_1 = array(
		'id' => 'info-maintemplates',
		'type' => 'info',
		'title' => 'About main templates',
		'std' => '',
		'before' => '
		
<h3>There are 2 types of main templates: "Default WP templates" and "Custom Page Templates"</h3>

<p class="colcount3">	
<strong>Default WP Templates</strong> and <strong>Custom Page Templates</strong> are  
(named) <strong>Main Templates</strong> (here in the Montezuma Admin), because they have everything to display a complete page. 
Contrary to that <strong>Sub templates</strong> cannot display a whole page on their own, they are just parts of a Main Template. 
</p>

<br>
<img src="' . get_template_directory_uri() . '/admin/images/mainandsubtemplates.png" />


<h3>It\'s the <span style="text-decoration:underline;font-weight:bold">name</span> of a Main Template 
that makes it become either a "Default WP template" OR a "Custom Page Template"</h3>
<p class="colcount3">
Whether a <strong>Main Template</strong> is a <strong>Default WP Template</strong> or a <strong>Custom Page Template</strong> 
depends on the template <span style="text-decoration:underline">name</span>. 
There is no \'setting\' for that. If a template has one of the reserved Default WP Template names, then 
it is a Default WP Template. All other Main Templates are Custom Page Templates. Reserved Default WP Template names are 
<code>index</code>, <code>page</code>, <code>single</code>, <code>category</code>, <code>tag</code>, <code>author</code>, 
<code>search</code> and many more. For a complete list see the image at 
<a target="_blank" href="http://codex.wordpress.org/Template_Hierarchy">WordPress Template Hierarchy</a>.
</p>

<h3>"Default WP templates" and "Custom Page Templates" are treated differently by WordPress</h3>

<h4>&raquo; Default WP Templates are <span style="text-decoration:underline">used automatically by WordPress</span></h4>
<p>A "Default WP template" will be used by WordPress <strong>automatically</strong> (there\'s no \'setting\' for this anywhere) 
based on 2 things: 
</p>
<ol>
<li><p>The "type" of page being requested by the visitor of your site, e.g. whether it is a "list of posts in category X", a "search results for term Y" and so on.</p></li>
<li><p>The physical (in Montezuma also: virtual) presence of the best - or any - <strong>matching</strong> template for the requested page type.</p></li>
</ol>
<p>
Condition 1. and 2. are dealt with by WordPress based on a set of rules also known as the 
<a target="_blank" href="http://codex.wordpress.org/Template_Hierarchy">WordPress Template Hierarchy</a>.
</p>

<h4>&raquo; Custom Page Templates <span style="text-decoration:underline">must be assigned by you</span> to certain pages (in Montezuma you can also assign them to posts!) before they get used</h4>
<p>
Contrary to Default WP Templates, Custom Page Templates are not chosen automatically. If you don\'t assign a Custom 
Page Template to one or more individual posts or pages then that Custom Page Template, e.g. <code>my-custom-template.php</code>,
 won\'t be used, ever.
</p>


<h3>Montezuma adds a "Virtual Template System" on top of WP\'s template system</h3>

<img src="' . get_template_directory_uri() . '/admin/images/templatehierarchy.png" />


<p  class="colcount3">
WordPress chooses the right page template for displaying a page based on the 
<a target="_blank" href="http://codex.wordpress.org/Template_Hierarchy">WordPress Template Hierarchy</a>. 
That WordPress Template Hierarchy is based on <strong>physically existing</strong> files inside the theme\'s 
(e.g. Montezuma\'s) main directory. In addition to this Montezuma adds a virtual file system, with virtual \'files\' 
saved in the database. The virtual file system acts like the WP Template Hierarchy. It also 
looks for the best matching template first (e.g. <code>category-cameras.php</code>) and then for the next best matching 
template (<code>category.php</code>), then the next best (<code>archive.php</code>) until it arrives at the "template of last resort" 
(<code>index.php</code>). The purpose of 
this virtual file system is to avoid writing files into the theme\'s directory, while at the same time 
allowing you to edit and even create templates online, right in the Montezuma backend, with code 
highlighting, bracket matching, auto intendation etc... The Montezuma virtual file system does <strong>not</strong> get in the 
way of the WP Template Hierarchy. Only if the WP Template Hierarchy fails to find a matching, physically existing template 
file in the theme\'s main directory and is about to use the "template of last resort" <code>index.php</code> then the Montezuma 
virtual file system takes over. This allows you to keep working with the traditional approach of editing real files 
on your desktop computer and uploading them to your WordPress installation through FTP. 
</p>





<h3>Convenience or total control: Edit &amp; add virtual files online or - for full PHP access - create & upload physical template files. 
Physical templates take precedence over virtual templates</h3>
<p  class="colcount3">
Montezuma\'s virtual file system offers convenient 
online editing and creation of templates. For full 
access to all PHP functions you can still 
work the traditional way, e.g. with a child theme, real files, and uploading those files with FTP. 
If a template is requested by WP, for instance "my-template.php", the virtual "my-template.php" is only used if no real 
"my-template.php" exists in the main directory of the theme. 
This applies to both main and sub templates.
</p>






<h3>Let WordPress/Montezuma figure out the best matching virtual template, or assign specific virtual templates to specific posts or pages</h3>

<div style="background:#f7f7f7;font-size:13px;float:right;width:300px;padding:5px;border:solid 1px #ccc;margin: 5px 5px 10px 20px">
<img src="' . get_template_directory_uri() . '/admin/images/virtualtemplates.png" />
You will see the dropdown menu "Virtual Template" at the bottom right on Page &amp; Post write panels in the 
WP admin. You can apply any of the virtual main templates to any post or page, or choose "Best match based on WP Hierarchy".</div>

<p>
As mentioned, Montezuma\'s virtual file system works like the "WP Template Hierarchy" in that 
it chooses the best matching template based on the type of page being requested. The match is 
figured out based on the template\'s name. 
</p>
<p>
On top of that you can also assign a main template to individual posts or pages.  
All existing virtual main templates will be listed in a select dropdown menu 
named <strong>Virtual Template</strong>, located on the right hand side of the WP post and page write panels. 
</p>

<h4>2 differences between the WordPress "Template" setting and 
Montezuma\'s "Virtual Template" setting:</h4>
<ul>
<li>1. All (virtual) main templates will be available, not just the (virtual) "Custom Page Templates" but also the 
(virtual) "Default WP Templates", 
just in case you want to assign one of the Default Templates to an indivdual post or page.</li>
<li>2. Unlike pyhsical templates &amp; the WP "Template" setting, Virtual Templates can be assigned to posts as well, not just to pages. </li>
</ul>



	'
);

$add_2 = array(
		'id' => 'add-maintemplate',
		'type' => 'info',
		'title' => '+ Add main template',
		'std' => '',
		'before' => '
<div class="templatenameinfo">
<h3>Template names must be unique</h3>
<p>You cannot have 
a main template <code>whatever</code> and a sub template <code>whatever</code> at the same time. </p>
<h3>Template names already in use</h3>
<ul class="red">
<li>comments</li>
' . bfa_get_used_templates( 'all_templates' ) . '
</ul>
</div>


Template name, without ".php". No spaces. Only letters, numbers, <strong>-</strong> and <strong>_</strong>:<br>
<input class="item_name" type="text" style="color:blue;width:350px;text-align:right;font-size:20px;" value="" />
<code>.php</code> <span style="font-size:30px;color:red">*</span>



<p>
Make new template a copy of: 
<select id="make_copy_of">
<option value="startblank">none (empty template)</option>
' . bfa_get_used_templates( 'used_templates_dropdown' ) . '
</select>
</p>
<br>
<button class="ata-add-item" type="button" rel="maintemplate"><i></i>ADD Main Template</button>

<h3><span style="font-size:30px;color:red">*</span> 
The <span style="color:red;font-weight:bold">name</span> of a main template constitutes whether it is a 
<span style="text-decoration:underline">Default</span> WP template or a <span style="text-decoration:underline">Custom</span> page template:</h3> 

<div style="padding:10px;border:solid 1px #ccc;background:#f7f7f7;margin: 0 180px 10px 0;">
<h4 style="margin-top:0">&rarr; <span style="text-decoration:underline">Default</span> WP template</h4> 
<p class="colcount2">
To create a Default WP template use one of the still unused Default WP template names. 
Templates with a predefined Default name such as "search", "category" or "tag" will be automatically selected by WP 
to display the matching page type (a search page, a category page, a tag page etc.) according to the 
<a target="_blank" href="http://codex.wordpress.org/File:Template_Hierarchy.png" title="External link, opens in new tab">
WP Template Hierarchy</a>: 
</p>

<strong>Currently unused default WP template names:</strong>
<ul class="tpllist cf">
' . bfa_get_used_templates( 'unused_default_templates' ) . '
</ul>

</div>

<div style="padding:10px;border:solid 1px #ccc;background:#f7f7f7;margin: 0 180px 10px 0;">
<h4 style="margin-top:0">&rarr; <span style="text-decoration:underline">Default</span> WP template with "parameter"</h4> 
<p>To match a more specific pageview (e.g. category page for "Cameras") 
use a Default WP template name with a "parameter":
</p>

<strong>Always available default WP template names with parameter</strong> (as long as the -XXX part is different):
<ul class="tpllist cf">
' . bfa_get_used_templates( 'unused_default_templates_parameter' ) . '
</ul>

<p class="colcount2">
Replace <code>XXX</code> with the slug or ID of the category, tag, author, etc... according to 
the <a target="_blank" href="http://codex.wordpress.org/File:Template_Hierarchy.png" title="External link, opens in new tab">
WP Template Hierarchy</a>. For instance, <code>category-cameras</code> would be used by WordPress to display the category 
for the category "Cameras".  
Assuming here that the "slug" of that category is <code>cameras</code>. The "slug" of an item is displayed 
in many places in the WP admin. The slugs of categories you can see at Posts -> Categories -> See "Slug" column in category list.
</p>
</div>

<div style="padding:10px;border:solid 1px #ccc;background:#f7f7f7;margin: 0 180px 10px 0;">
<h4 style="margin-top:0">&rarr; <span style="text-decoration:underline">Custom</span> template:</h4> 
<p class="colcount2">
A template with a custom name will not be automatically selected by WP for anything. 
The point of these custom templates is that you can assign them to individual posts or pages.   
For posts at WP -> Posts -> Add New / All Posts and for static pages at WP -> Pages -> Add New / All Pages.
</p>

<strong>Examples for custom template names:</strong>
<ul class="tpllist cf"><li>whatever</li>
<li>anything-abc</li>
<li>my-template</li>
<li>blue-landing-page</li>
</ul>

</div>
'
);



$pagetemplatefiles[] = $add_1;
$pagetemplatefiles[] = $add_2;

$existing_ids = array();

// Get default templates = pyhsical files
foreach ( glob( get_template_directory() . "/admin/default-templates/main-templates/*.php") as $filepath) {

	$filename = basename( $filepath ) ;
	
	$file_ID = str_replace( '.php', '', $filename );
	
	$thisfile = array(
		'id' => 'maintemplate-' . $file_ID,
		'type' => 'codemirror',
		'title' => $file_ID .  '<span style="color:#666">.php</span>',
		'std' => implode( "", file( $filepath ) ),
		'codemirrormode' => 'php',
		'before' => 'Edit the <code>' . $file_ID . '<span style="color:#999">.php</span></code> main template. You can use HTML and 
		<a href="#" class="limitedphpcode">this limited set of PHP code</a> to insert dynamic info. 
		<p>Don\'t be afraid if you don\'t 
		"know" PHP. You can just copy &amp; paste the pieces of PHP code (starting with <code>&lt;?php</code> and ending with 
		<code>?&gt;</code>), and then adjust the parts inside the braces - the so-called "parameters" - if there are any, and if 
		you want / need to adjust them. Always stay close to the original PHP code you found in a template. Don\'t introduce 
		line breaks, don\'t add comments, etc...
		</p>',
		'istemplate' => 'maintemplate'
	);
	$pagetemplatefiles[] = $thisfile;
	
	$existing_ids[] = 'maintemplate-' . $file_ID;
}

// Get any additional custom templates = code string in option $montezuma 

if( $montezuma ) {
	foreach( $montezuma as $key => $value ) {

		if( strpos( $key, 'maintemplate-' ) === 0 && ! in_array( $key, $existing_ids ) ) {
		
			$title = str_replace( substr( $key, 0, 13 ), '', $key );
			
			$thisfile = array(
				'id' => $key,
				'type' => 'codemirror',
				'title' => $title .  '<span style="color:#666">.php</span>',
				'codemirrormode' => 'php',
				'before' => '<p><button onclick="return confirmDeleteItem(\'' . $title . '\')" class="ata-delete-item" type="button" id="deleteitem-'. $key .'"><i></i>Delete "'. $title .'"</button></p>',
				'istemplate' => 'maintemplate'
			);
			$pagetemplatefiles[] = $thisfile;	
			
			
			#unset( $montezuma['maintemplate-' . $key] );
			
		}
	}
}


#update_option( 'Montezuma', $montezuma );

return $pagetemplatefiles;