<?php 

return array(

'title'			=> 'Comments',
'description' => 'Configure the comment area',


# Comment area title

array(
'id'		=> 'comments_title_single',
'type' 		=> 'text',
'title'		=> 'Comments title',
'before' 	=> 'If there is 1 comment. <code>%2$s</code> = Post Title: <span class="arrow-down">&nbsp;</span><br>',
'std'		=> '<span>One</span> thought on %2$s',
),

array(
'id'		=> 'comments_title_plural',
'type' 		=> 'text',
'title'		=> '',
'before' 	=> 'If there are 2 or more comments. <code>%1$s</code> = comment number, <code>%2$s</code> 
				= Post Title: <span class="arrow-down">&nbsp;</span><br>',
'std'		=> '<span>%1$s</span> thoughts on %2$s',
'group'		=> true
),



# Avatar sizes

array(
'id'		=> 'avatar_size',
'type' 		=> 'text',
'title'		=> 'Avatar image',
'before'	=> 'Put a number between 30 and 80 here. (50 means Avatar is 50x50 pixels):<br>',
'after' 	=> ' &nbsp;<span class="arrow-left">&nbsp;</span> Default Avatar size',
'std'		=> 50,
'style'		=> 'width:30px',
),

array(
'id'		=> 'avatar_size_small',
'type' 		=> 'text',
'title'		=> '',
'after' 	=> ' &nbsp;<span class="arrow-left">&nbsp;</span> Avatar size for 2nd+ level comments',
'std'		=> 35,
'style'		=> 'width:30px',
'group'		=> true
),

array(
'id'		=> 'avatar_url',
'type' 		=> 'upload-image',
'title'		=> '',
'style'		=> 'width:100px;height:100px',
'before'	=> 'Default Avatar image URL: <span class="arrow-down">&nbsp;</span><br>',
'after'		=> 'Leave empty to use the default avatar image as set in WP Admin -> Settings -> Discussion -> Default Avatar<br>',
'group'		=> true
),	



# comment quicktags

array(
'id'		=> 'comment_quicktags',
'type' 		=> 'text',
'before'	=> '

<img style="float:right;margin: 0 0 5px 15px" src="' . get_template_directory_uri() . '/admin/images/quicktagbuttons.png" />

List "quicktag" buttons to display above the comment form. Separate with comma, without spaces: <span class="arrow-down">&nbsp;</span><br>', 
'after'		=> '<br>Leave empty to not display any "quicktag" button. Available buttons: <code>strong</code>, 
				<code>em</code>, <code>link</code>, <code>block</code>, <code>code</code>, <code>close</code>. 
				This does not affect the HTML tags that are actually allowed. For that, see the next option.',
'std'		=> 'strong,em,link,block,code,close',
'title'		=> 'Quicktag buttons',
'style'		=> 'width:400px',
),

# allowed html

array(
	'id'	=> 	'comment_allowed_tags',
	'type' 	=> 	'checkbox-list',
	'values'=> 	array( 
					'a' => '<code>&lt;a href="..." title="..."&gt;</code>',
					'abbr' => '<code>&lt;abbr title="..."&gt;</code>',
					'acronym' => '<code>&lt;acronym title="..."&gt;</code>',
					'b' => '<code>&lt;b&gt;</code>',
					'blockquote' => '<code>&lt;blockquote cite="..."&gt;</code>',
					'br' => '<code>&lt;br&gt;</code>',
					'cite' => '<code>&lt;cite&gt;</code>',
					'code' => '<code>&lt;code&gt;</code>',
					'del' => '<code>&lt;del datetime="..."&gt;</code>',
					'dd' => '<code>&lt;dd&gt;</code>',
					'dl' => '<code>&lt;dl&gt;</code>',
					'dt' => '<code>&lt;dt&gt;</code>',
					'em' => '<code>&lt;em&gt;</code>', 
					'i' => '<code>&lt;i&gt;</code>',
					'ins' => '<code>&lt;ins datetime="..." cite="..."&gt;</code>',
					'li' => '<code>&lt;li&gt;</code>',
					'ol' => '<code>&lt;ol&gt;</code>',
					'p' => '<code>&lt;p&gt;</code>',
					'q' => '<code>&lt;q cite="..."&gt;</code>',
					'strike' => '<code>&lt;strike&gt;</code>',
					'strong' => '<code>&lt;strong&gt;</code>',
					'sub' => '<code>&lt;sub&gt;</code>',
					'sup' => '<code>&lt;sup&gt;</code>',
					'u' => '<code>&lt;u&gt;</code>',
					'ul' => '<code>&lt;ul&gt;</code>',
				),
	'title'	=> 	'Allowed HTML',
	'std'	=> array( 'a', 'abbr', 'acronym', 'b', 'blockquote', 'cite', 'code',
					'del', 'em', 'q', 'strike', 'strong' ), // default allowed tags in wp-includes/kses.php
	'columns' => 3,
	'before'	=> 'Check the HTML tags you want to allow inside comments. Uncheck all to not allow any HTML tags in comments:<br><br>'
),



# form custom code before / after

array(
'id'		=> 'comment_notes_before',
'type' 		=> 'codemirror',
'title'		=> 'Code before/after form',
'before' 	=> 'Custom text or HTML right before the comment form text area. Will only be displayed to users that are not logged in 
(just like the name, email &amp; url input fields): <span class="arrow-down">&nbsp;</span><br>',
'std'		=> '',
),

array(
'id'		=> 'comment_notes_after',
'type' 		=> 'codemirror',
'title'		=> '',
'before' 	=> 'Custom text or HTML right after the comment form text area: Will only be displayed to users that are not logged in 
(just like the name, email &amp; url input fields): <span class="arrow-down">&nbsp;</span><br>',
'std'		=> '',
'group'		=> true
),









	
);
