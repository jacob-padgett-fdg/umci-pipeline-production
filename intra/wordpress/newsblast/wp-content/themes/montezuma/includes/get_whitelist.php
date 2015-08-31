<?php 
function bfa_get_whitelist() {

	/* 'function_name' => array( 'type' => 'parameter_type' ),
	   parameter_type = single | array | queryarray | function
	   
	   'single' - 0 or 1 parameter: function_name() - function_name( parameter )
	   'array'  - parameter is array: function_name( array( 'key' => 'value', 'key' => 'value' ) )
	   'queryarray' - parameter is URL-query style: function_name('this=that&this=that&this=that')
	   'function' - parameters are function style: function_name( param, 'param2', param3 );
	*/
	
	$wl_global = array(

		'printf' => array( 
			'type' => 'function',
			'examples' => array(
				"<?php printf( __( 'Published in %1\$s on %2\$s', 'montezuma' ), the_date(), the_category( ' &middot; ' ) ); ?>" 
				=> 'Prints and replaces the %1\$s variables.',
			),
			'info' => '	'
		),
		
		'get_header' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php get_header(); ?>' => 'Includes <code>header.php</code> sub template.',
			),
			'info' => "In case you created a second 'header' sub template in addition to the standard <code>header.php</code>, 
			such as <code>header-2.php</code>, you could include it with <?php get_header( '2' ); ?>. Whatever you put into the 
			brackets will be appended to <code>header</code> with a dash <code>-</code> in between, and that file will be included: 
			E.g. to include <code>header-whatever.php</code> in a main template you would use <code><?php get_header( 'whatever' ); ?></code>"
		),
		
		'get_footer' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php get_footer(); ?>' => 'Includes <code>footer.php</code> sub template.',
			),
			'info' => "Same as <code>get_header</code> but for the 'footer' sub template",
		),

		'get_search_form' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php get_searchform(); ?>' => 'Includes <code>searchform.php</code> sub template.',
			),
			'info' => 'Similar to <code>get_header</code> and <code>get_header</code> but without the option to 
			include alternative sub template versions by providing a parameter inside the brackets. Use as is.',
		),		
		
		
		'__' => array( 
			'type' => 'function',
			'examples' => array(
				"<?php __( 'Some text', 'montezuma' ); ?>" => 'Replaces \'Some text\' or the translated version of it',
				"<?php __( 'Peter\'s <span class=\"peterpage\">Page</span>', 'montezuma' ); ?>" => 
				'Note how that single quote is "escaped" with a backslash, 
				because single quotes are also used to wrap the string in  this example. ',
				"<?php __( \"Peter's <span class=\\\"peterpage\\\">Page</span>\", 'montezuma' ); ?>" => 
				'Note how the double quotes are "escaped" with a backslash, 
				because double quotes are also used to wrap the string in this example. <strong>(Note: This exmaple 
				might not show a success window when copied with icon button but should copy regardless)</strong>',
			),
			'info' => 'These are 2 underscore characters followed by 2 paramaters inside brackets. 
			The first parameter is always the text string, the second is <code>montezuma</code>. If you wrap the string into 
			single quotes then single quotes inside the string need to be escaped with a backslash but double quotes don\'t need to 
			be escaped. And the other way around with double quotes: If you wrap the string with double quotes you escape any double quotes 
			inside the string with a backslash while single quotes inside the string don\'t need to be escaped. Escaping means telling 
			that you want to print this character literally and that it is not supposed to be a string delimiter. 
			This function <code>__(...)</code> only makes sense as a parameter inside another function because it does not print anything 
			on itself. Example: <code><?php edit_post_link( __(\'Edit\', \'montezuma\'), \'<div class="post-edit">\', \'</div>\' ); ?></code>
			
			
			'
		),
		
		'_e' => array( 
			'type' => 'function',
			'examples' => array(
				'<?php _e( "Some text", "montezuma" ); ?>' => 'Displays "Some text" or the translated version of it',
			),
			'info' => 'These are 2 "underscore" characters followed by 2 paramaters inside brackets. 
			The first is the text string, the second is "montezuma".'
		),	
		
		'dynamic_sidebar' => array( 
			'type' => 'single',
			'examples' => array(
				"<?php dynamic_sidebar( 'Widget Area ONE' ); ?>" => 
					'Displays the contents of a widget area named <code>Widget Area ONE</code>', 
				"<?php dynamic_sidebar( 'My other widget area' ); ?>" => 
					'Displays the contents of a widget area named <code>My other widget area</code>', 
				"<?php dynamic_sidebar( 'Footer stuff' ); ?>" => 
					'Displays the contents of a widget area named <code>Widget Area ONE</code>', 				
			),
			'info' => 'Creates and displays widget areas in one go. 
			Simply put this short one-liner into a template to display the 
			contents of that widget area (= the widgets placed therein) at exactly the place in the template where you put the code. 
			This also creates that widget area in the WP backend at WP -> Appearance -> Widgets'
		),
		
		'home_url' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php echo home_url(); ?>' => 'Displays the content of the field labeled "Site Address (URL)" 
					in WP -> General -> Settings, usually without trailing slash. <code>http://www.mydomain.com</code>',
			),
			'info' => '<code>home_url</code> does not print anything on its own, so use it with <code>echo</code> as shown in the example.'
		),
		
		'site_url' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php echo site_url(); ?>' => 'Displays the content of the field labeled "WordPress Address (URL)" in WP -> Settings -> General, usually without trailing slash. This can but doesn\'t have to be the same as <code>site_url</code>. <code>http://www.mydomain.com</code> or <code>http://www.mydomain.com/wordpress</code>',
			),
			'info' => '<code>site_url</code> does not print anything on its own, so use it with <code>echo</code> as shown in the example.'
		),
		
		'bloginfo' => array( 
			'type' => 'single',
			'examples' => array(
				"<?php bloginfo('name'); ?>" => "Testpilot",
				"<?php bloginfo('description'); ?>" => "Just another WordPress blog",
				"<?php bloginfo('admin_email'); ?>" => "admin@example",
				"<?php bloginfo('atom_url'); ?>" => "http://example/home/feed/atom",
				"<?php bloginfo('rss2_url'); ?>" => "http://example/home/feed",
				"<?php bloginfo('rss_url'); ?>" => "http://example/home/feed/rss",
				"<?php bloginfo('pingback_url'); ?>" => "http://example/home/wp/xmlrpc.php",
				"<?php bloginfo('rdf_url'); ?>" => "http://example/home/feed/rdf",
				"<?php bloginfo('comments_atom_url'); ?>" => "http://example/home/comments/feed/atom",
				"<?php bloginfo('comments_rss2_url'); ?>" => "http://example/home/comments/feed",
				"<?php bloginfo('charset'); ?>" => "UTF-8",
				"<?php bloginfo('html_type'); ?>" => "text/html",
				"<?php bloginfo('language'); ?>" => "en-US",
				"<?php bloginfo('version'); ?>" => "3.1",			
			),
			'info' => '<code>bloginfo</code> displays many different bits of global information about the site based on the 
			parameter inside the brackets.',
		),
		
		'wp_nav_menu' => array( 
			'type' => 'array',
			'examples' => array(
				"<?php wp_nav_menu( array( 'theme_location' => 'menu1', 'fallback_cb' => 'bfa_page_menu', 'container' => false ) ); ?>" 
					=> 'Prints menu bar with the links as set in WP -> Appearance -> Menus -> Theme Locations -> menu1. 
					If no menu has been configured for "Theme Location: menu1", the menu "bfa_page_menu" will be used as default menu. 
					Available default menus: <code>bfa_page_menu</code> (Page Menu, uses all static pages that exist 
					in this WP installation), <code>bfa_cat_menu</code> 
					(Category menu, uses all categories that exist in this WP installation)',
				"<?php wp_nav_menu( array( 'theme_location' => 'menu2', 'fallback_cb' => 'bfa_cat_menu', 'container' => false ) ); ?>" => 
					'Like above but with "Theme Location: menu2" and "bfa_cat_menu" as the default fallback if no menu was configured for 
					Theme Location menu2',
			),
			'info' => 'Displays a navigation menu created in WP -> Appearance -> Menus.',
		),		
			

			
		'wp_dropdown_users' => array( 
			'type' => 'array',
			'examples' => array(
				'<?php wp_dropdown_users(); ?>' => 'Displays dropdown HTML content of users',
			),
			'info' => 'Use as is or with one/some of the many parameters available, see WP Docs.',
		),		
			
		'wp_list_authors' => array( 
			'type' => 'array',
			'examples' => array(
				'<?php wp_list_authors(); ?>' => "Displays a list of the sites's authors (users), and if the user has authored any posts, the author name is displayed as a link to their posts. Optionally this tag displays each author's post count and RSS feed link.",
			),
			'info' => 'Use as is or with one/some of the many parameters available, see WP Docs.',
		),		
			
		'wp_list_bookmarks' => array( 
			'type' => 'array',
			'examples' => array(
				'<?php wp_list_bookmarks(); ?> ' => 'Displays bookmarks found in WP -> Links',
			),
			'info' => 'Use as is or with one/some of the many parameters available, see WP Docs.',
		),		
			
		'wp_dropdown_categories' => array( 
			'type' => 'array',
			'examples' => array(
				'<?php wp_dropdown_categories(); ?> ' => 'Displays HTML dropdown list of categories.',
			),
			'info' => 'Use as is or with one/some of the many parameters available, see WP Docs.',
		),		
			
		'wp_list_categories' => array( 
			'type' => 'array',
			'examples' => array(
				'<?php wp_list_categories(); ?>' => 'Displays a list of Categories as links.',
			),
			'info' => 'Use as is or with one/some of the many parameters available, see WP Docs.',
		),		
			
		'wp_tag_cloud' => array( 
			'type' => 'array',
			'examples' => array(
				'<?php wp_tag_cloud(); ?>' => "Displays a list of tags in what is called a 'tag cloud', where the size of each tag is determined by how many times that particular tag has been assigned to posts.",
			),
			'info' => 'In Montezuma the different sizes of tags in the cloud is removed for a stramlined display. CSS classes are available 
			(see Widgets/Tag Cloud in the CSS section) for applying not just different sizes but any kind of style based on popularity of a tag. Use as is or with one/some of the many parameters available, see WP Docs.',
		),		
			
		'single_term_title' => array( 
			'type' => 'function',
			'examples' => array(
				'<?php single_term_title(); ?>' => 'Displays the "term" title for the current page.',
				"<?php single_term_title( 'Currently browsing ' ); ?>" => 'Displays "Currently browsing " 
				followed by the "term" title for the current page.',
			),
			'info' => 'Displays the title for a taxonomy on taxonomy pages. 
			Can be used instead of single_cat_title() and single_tag_title().',
		),		
		
		// Needs post ID. Get post ID inside bfa_parse_php() ?
		/*
		'the_terms' => array( 
			'type' => 'function',
			'examples' => array(
				'' => '',
			),
			'info' => '',
		),		
		*/	
			
		'wp_list_comments' => array( 
			'type' => 'array',
			'examples' => array(
				'<?php wp_list_comments(); ?>' => 'Displays all comments for a post or Page based on a variety 
					of parameters including ones set in the administration area. ',
			),
			'info' => 'Use as is or with one/some of the many parameters available, see WP Docs.',
		),		
		
		/*
		'esc_url' => array( 
			'type' => 'function',
			'examples' => array(
				'' => '',
			),
			'info' => '',
		),		
			
		'esc_attr_e' => array( 
			'type' => 'function',
			'examples' => array(
				'' => '',
			),
			'info' => '',
		),		
		*/
		
		'bfa_loop' => array( 
			'type' => 'single',
			'examples' => array(
				"<?php bfa_loop(); ?>" => 'Displays the list of posts (the WordPress "Loop") on multi post pages. 
					For each post it uses the default post format sub template <code>postformat.php</code> as the base 
					template if no other base post format template is provided inside the brackets.',
				"<?php bfa_loop( 'otherformat' ); ?>" => 'Displays the list of posts (the WordPress "Loop") on multi post pages. 
					For each post it uses the sub template <code>otherformat.php</code>. You would have to create 
					that sub template <code>otherformat.php</code> in Montezuma (See "Add sub template").',
			),
			'info' => "This function will first look for the specific version of a post format template (e.g. 
					<code>postformat-video.php</code> for a post that was set to Format: 'Video' in the WP posts panel) 
					and then, if that does not exist (because you did not create a sub template named <code>postformat-video.php</code> 
					in Montezuma) 
					fall back to using the base version <code>postformat.php</code>. And if you\'re working 
					with a new set of post format templates (which you created as sub templates in Montezuma, e.g 
					<code>otherformat.php</code>, <code>otherformat-video.php</code>, <code>otherformat-link.php</code> ...) 
					and you use <code><?php bfa_loop( 'otherformat' ); ?></code> then it will look for (again, if a post 
					was set to Format: 'Video' for instance) for <code>otherformat-video.php</code>, and if that does not exist, 
					it will fall back to using <code>otherformat.php</code>."

		),		
		
		/*
		'bfa_loop_single' => array( 
			'type' => 'single',
			'examples' => array(
				'' => '',
			),
			'info' => '',
		),		
			
		'bfa_loop_page' => array( 
			'type' => 'single',
			'examples' => array(
				'' => '',
			),
			'info' => '',
		),		
		*/
		
		'bfa_breadcrumbs' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php bfa_breadcrumbs(); ?>' => 'Displays the breadcrumbs navigation.',
				"<?php bfa_breadcrumbs( 'breadcrumbs1' ); ?>" => 'Displays the breadcrumbs navigation and adds the CSS ID <code>breadcrumbs1</code>
					to the container element. Useful if you want to display the breadcrumbs more than once on a page, and style them differently.',
			),
			'info' => '',
		),		

		'bfa_paginate_comments' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php bfa_paginate_comments(); ?>' => 'Displays a numbered comment navigation.',
				"<?php bfa_paginate_comments( 'comment-pagination-1' ); ?>" => 'Displays a numbered comment navigation and 
						adds the CSS ID <code>comment-pagination-1</code> to the container element. 
						Useful if you want to display the comment navigation more than once on a page (for instance: above and below the 
						comment list), and style them differently.',
			),
			'info' => '',
		),		
			
		'bfa_comment_form' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php bfa_comment_form(); ?>' => 'Displays the comment form.',
			),
			'info' => '',
		),		
		
		'comment_class' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php comment_class(); ?>' => 'Displays various CSS classes for each single comment based on position of comment, whether 
					the comment is from the post author etc... These CSS classes can then be used to style single comments.',
			),
			'info' => 'This should be used in <code>comments-comment.php</code> - the sub template for a single comment.',
		),		
			
		'comment_ID' => array( 
			'type' => 'single',	
			'examples' => array(
				'<?php comment_ID(); ?>' => 'Displays the numeric ID of the current comment. ',
			),
			'info' => 'This should be used in <code>comments-comment.php</code> - the sub template for a single comment.',
		),		
			
		'bfa_avatar' => array( 
			'type' => 'single',	
			'examples' => array(
				'<?php bfa_avatar(); ?>' => 'Displays the avatar of a comment author.',
			),
			'info' => 'This should be used in <code>comments-comment.php</code> - the sub template for a single comment.',
		),		
			
		'comment_author_link' => array( 
			'type' => 'single',	
			'examples' => array(
				'<?php comment_author_link(); ?>' => "Displays the comment author's name linked to his/her URL, if one was provided.",
			),
			'info' => 'This should be used in <code>comments-comment.php</code> and <code>comments-pingback.php</code> 
				- the sub templates for a single comment and a single pingback/trackback.',
		),		
		
		'bfa_content_nav' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php bfa_content_nav(); ?>' => 'Displays a numbered navigation on multi post pages.',
			),
			'info' => 'Should be used on multi post pages (index, tag, category, search...) but not on single post pages 
				(single, page, 404, custom templates that are meant to be single post pages...) ',
		),

		'bfa_comments_title' => array( 
			'type' => 'single',
			'examples' => array(
				'' => '',
			),
			'info' => '',
		),
					
		'comment_link' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php comment_link(); ?>' => 'Display "permalink" URL of a single comment. This is the URL that you or someone else can use to 
					link exactly to a specific comment, instead of linking to just the page the comment is displayed on',
			),
			'info' => 'This should be used in <code>comments-comment.php</code> - the sub template for a single comment.
					This is commonly used with the comment date (see <code><?php comment_date(); ?></code>) as the link text. 
					This way the comment date serves a second purpose 
					instead of just displaying the comment date.',
		),
		
		'comment_date' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php comment_date(); ?>' => 'Displays the date a comment was posted, using the default date format set in WordPress.',
				"<?php comment_date('n-j-Y'); ?>" => 'Displays the date a comment was posted, in the format <code>6-30-2014</code>.',
			),
			'info' => 'This should be used in the sub templates <code>comments-comment.php</code> and 
				<code>comments-pingback.php</code>. For more date formatting options see 
				<a target="_blank" href="http://codex.wordpress.org/Formatting_Date_and_Time">WP Date & Time Formats</a>',
		),
		
		'comment_time' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php comment_time(); ?>' => 'Displays the time a comment was posted, using the default date format set in WordPress.',
				"<?php comment_time('H:i:s'); ?>" => 'Displays the time a comment was posted, in the format <code>22:04:11</code>.',
			),
			'info' => 'This should be used in the sub templates <code>comments-comment.php</code> and 
				<code>comments-pingback.php</code>. For more time formatting options see 
				<a target="_blank" href="http://codex.wordpress.org/Formatting_Date_and_Time">WP Date & Time Formats</a>',
		),
		
		'edit_comment_link' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php edit_comment_link(); ?>' => 'Displays a link to edit the current comment, if the user is logged in and allowed to edit the comment.',
				"<?php edit_comment_link( __( 'Edit', 'montezuma' ) ); ?>" => 'Displays a link to edit the current comment, with "Edit" as the text link.',
			),
			'info' => 'This should be used in <code>comments-comment.php</code> - the sub template for a single comment.',
		),
		
		'bfa_comment_delete_link' => array( 
			'type' => 'single',
			'examples' => array(
				"<?php bfa_comment_delete_link( __( 'Delete', 'montezuma' ) ); ?>" => 'Displays a link for deleting a comment.',
			),
			'info' => 'Will only be displayed if current user is logged in and allowed to delete comments. 
				This should be used in <code>comments-comment.php</code> - the sub template for a single comment.',
		),
		
		'bfa_comment_spam_link' => array( 
			'type' => 'single',
			'examples' => array(
				"<?php bfa_comment_spam_link(); ?>" => 'Displays a link for deleting and tagging a comment as spam, without link text. 
					Could be used to style this as a graphical link, with a background image.',
				"<?php bfa_comment_spam_link( __( 'Spam', 'montezuma' ) ); ?>" => 'Displays a link to delete and tag a comment as spam, with "Spam" as the link title.',
			),
			'info' => 'Will only be displayed if current user is logged in and allowed to delete comments. 
				This should be used in <code>comments-comment.php</code> - the sub template for a single comment.',
		),
		
		'comment_text' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php comment_text(); ?>' => 'Displays the text of a comment.',
			),
			'info' => 'This should be used in <code>comments-comment.php</code> - the sub template for a single comment.',
		),
		
		'bfa_comment_awaiting' => array( 
			'type' => 'single',
			'examples' => array(
				"<?php bfa_comment_awaiting( __( 'Your comment is awaiting moderation.', 'montezuma' ) ); ?>" 
					=> 'Displays "Your comment is awaiting moderation." is comments are being "moderated" (= checked first 
					by site owner before being published, instead of being published immediately. According to setting at 
					WP -> Settings -> Discussion -> Before a comment appears.',
			),
			'info' => 'This will only be displayed to the person that submitted a comment, not to everyone.',
		),
	
		'date' => array( 
			'type' => 'single',
			'examples' => array(
				"<?php echo date( 'Y'); ?>" 
					=> 'The PHP date() function. Prints the current date and/or time in the specified format. This example prints the year as 4-digit number e.g. 2014. For 
					possible parameters see <a target="_blank" href="http://www.php.net/manual/en/function.date.php">PHP Date (External)</a>.',
			),
			'info' => 'This functions does not print by itself. Always use it in combination with <echo>echo</code> as shown in the example.',
		),


		'get_num_queries' => array( 
			'type' => 'single',
			'examples' => array(
				"<?php echo get_num_queries(); ?>" 
					=> 'Displays the Database queries consumed to render the page.',
			),
			'info' => 'This functions does not print by itself. Always use it in combination with <echo>echo</code> as shown in the example.',
		),
		'timer_stop' => array( 
			'type' => 'single',
			'examples' => array(
				"<?php timer_stop(1); ?>" 
					=> 'Displays the time needed to render the page.',
			),
			'info' => 'Useful to check the impact of settings or plugins etc...',
		),


		'bfa_if_front_else' => array( 
			'type' => 'function',
			'examples' => array(
				'<?php bfa_if_front_else( "h1", "h3" ); ?>' => "Print 'h1' if current page is the front page, else print 'h3'",
			),
			'info' => 'This could be used to switch the HTML tag used e.g. for the site title from h1 on the front page to h2 or h3 or even a div on all other pages, 
			to put the SEO focus on the title of a post, a page or a category title or whatever the essence of a given page is. You can (and usually will) make the site title 
			look the same, through CSS. It is 
			commony accepted good SEO practise to give the site title an H1 only on the front page and nowhere else. You mileage may vary if your 
			site title is very important and/or full of relevant keywords. You usually use this function twice, once to open a tag, then again to close the tag, like this: 
			<code><<?php bfa_if_front_else( "h1", "h3" ); ?>>Some Title</<?php bfa_if_front_else( "h1", "h3" ); ?>></code>. The first parameter is 
			what will be printed on the front page, the second what will be printed on all other pages.',
		),
		
		
	);
		
	$wl_loop = array(

		'bfa_excerpt' => array( 
			'type' => 'function',
			'examples' => array(
				'<?php bfa_excerpt( num_words, more ); ?>' => "",
				'<?php bfa_excerpt(); ?>' => "Default. Same as <?php bfa_excerpt( 55, ' ...' ); ?>",
				"<?php bfa_excerpt( 40, ' ... read more' ); ?>" => "Print first 40 words, followed by ' ... read more'",
				"<?php bfa_excerpt( 100, ' ... continue reading <a href=\"%url%\">%title%</a>' ); ?>" => "Print first 100 words, followed by by link to full post with post title as link text",
			),
			'info' => 'This should be used in post format templates, e.g. <code>postformat.php</code>. 
			<code>%url%</code> will be replaced with the post permalink URL, 
			<code>%title%</code> with the post title.',
		),
		
		'previous_post_link' => array( 
			'type' => 'function',
			'examples' => array(
				'<?php previous_post_link(); ?>' => "Default. Same as <?php previous_post_link( '&laquo; %link', '%title', FALSE ); ?>",
				"<?php previous_post_link( '&laquo; %link', '%title', TRUE ); ?>" => 'Linked post must be in same category',
				"<?php previous_post_link( '&laquo; %link', '%title', TRUE, '1 and 5 and 15' ); ?>" => 'Exclude categories with the IDs 1, 5 and 15. Must have the word " and " between the cat IDs',
				"<?php previous_post_link( '&laquo; %link', __('Previous Post', 'montezuma') ); ?>" => 'Link text is "Previous Post" instead of actual post title',		
				"<?php previous_post_link( __('Previous post is here: %link <- previous post', 'montezuma'), '%title', TRUE ); ?>" => 'Some text before and after the link',				
			),
			'info' => 'The default has reasonable settings. Simply use <?php previous_post_link(); ?> if you\'re unsure.',
		),
		
		'next_post_link' => array( 
			'type' => 'function',
			'examples' => array(
				'<?php next_post_link(); ?>' => "Default. Same as <?php next_post_link( '&laquo; %link', '%title', FALSE ); ?>",
			),
			'info' => 'For more examples see <code>previous_post_link</code>.',
		),
		
		'post_class' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php post_class(); ?>' => 'Displays various CSS classes related to the current post.',
			),
			'info' => 'Should be used in templates where single posts or static pages are displayed: 
					In post format templates such as <code>postformat.php</code>, <code>postformat-video.php</code>, 
					<code>postformat-link.php</code>, <code>my-other-format.php</code>,  <code>my-other-format-video.php</code> etc... 
					Also in <code>single.php</code> and <code>page.php</code>.',
		),		
			
		'the_ID' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php the_ID(); ?>' => 'Displays the ID of the current post. Commonly used to print a CSS ID of a post, so that 
					each post can be styled individually.',
			),
			'info' => 'Should be used in templates where single posts or static pages are displayed: 
					In post format templates such as <code>postformat.php</code>, <code>postformat-video.php</code>, 
					<code>postformat-link.php</code>, <code>my-other-format.php</code>,  <code>my-other-format-video.php</code> etc... 
					Also in <code>single.php</code> and <code>page.php</code>.',
		),		
			
		'the_title' => array( 
			'type' => 'function',
			'examples' => array(
				'<?php the_title(); ?>' => 'Displays the title of the current post. 
					If the post is protected or private, this will be noted by the words "Protected: " or "Private: " prepended to the title.',
			),
			'info' => 'Should be used in templates where single posts or static pages are displayed: 
					In post format templates such as <code>postformat.php</code>, <code>postformat-video.php</code>, 
					<code>postformat-link.php</code>, <code>my-other-format.php</code>,  <code>my-other-format-video.php</code> etc... 
					Also in <code>single.php</code> and <code>page.php</code>.',
		),		
			
		'the_title_attribute' => array( 
			'type' => 'queryarray',
			'examples' => array(
				'<?php the_title_attribute(); ?>' => "Displays the title of the current post. It somewhat duplicates the 
					functionality of <code>the_title()</code>, but provides a 'clean' version of the title for use in HTML attributes 
					by stripping HTML tags and converting certain characters (including quotes) to their character entity equivalent",
			),
			'info' => 'Should be used in templates where single posts or static pages are displayed: 
					In post format templates such as <code>postformat.php</code>, <code>postformat-video.php</code>, 
					<code>postformat-link.php</code>, <code>my-other-format.php</code>,  <code>my-other-format-video.php</code> etc... 
					Also in <code>single.php</code> and <code>page.php</code>.',
		),		
			
		'the_permalink' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php the_permalink(); ?>' => 'Displays the permalink URL for the current post.',
			),
			'info' => 'Should be used in templates where single posts or static pages are displayed: 
					In post format templates such as <code>postformat.php</code>, <code>postformat-video.php</code>, 
					<code>postformat-link.php</code>, <code>my-other-format.php</code>,  <code>my-other-format-video.php</code> etc... 
					Also in <code>single.php</code> and <code>page.php</code>.',
		),		
			
		'the_time' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php the_time(); ?>' => 'Displays the time and/or date the current post was published at, using the default date/time format set in WordPress.',
				"<?php the_time('g:i a'); ?>" => 'Displays post time as 10:36 pm',
				"<?php the_time('G:i'); ?>" => 'Displays post time as 17:24',				
			),
			'info' => '',
		),		

		'the_date' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php the_date(); ?>' => 'Displays the post date in the default WP format.',		
			),
			'info' => '',
		),		
		
		'the_author' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php the_author(); ?>' => "Displays the value in the post author's 'Display name publicly as' field.",
			),
			'info' => 'Should be used in templates where single posts or static pages are displayed: 
					In post format templates such as <code>postformat.php</code>, <code>postformat-video.php</code>, 
					<code>postformat-link.php</code>, <code>my-other-format.php</code>,  <code>my-other-format-video.php</code> etc... 
					Also in <code>single.php</code> and <code>page.php</code>.',
		),		
			
		'the_author_link' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php the_author_link(); ?>' => "Displays a link to the Website for the author of a post. The Website field is set in the user's profile (WP -> Users -> Your Profile). The text for the link is the author's Profile 'Display name publicly as' field.",
			),
			'info' => 'Should be used in templates where single posts or static pages are displayed: 
					In post format templates such as <code>postformat.php</code>, <code>postformat-video.php</code>, 
					<code>postformat-link.php</code>, <code>my-other-format.php</code>,  <code>my-other-format-video.php</code> etc... 
					Also in <code>single.php</code> and <code>page.php</code>.',
		),		
			
		'the_author_meta' => array( 
			'type' => 'function',
			'examples' => array(
				"<?php the_author_meta( 'user_login' ); ?>" => 'See parameter name inside brackets',
				"<?php the_author_meta( 'user_pass' ); ?>" => 'See parameter name inside brackets',
				"<?php the_author_meta( 'user_nicename' ); ?>" => 'See parameter name inside brackets',
				"<?php the_author_meta( 'user_email' ); ?>" => 'See parameter name inside brackets',
				"<?php the_author_meta( 'user_url' ); ?>" => 'See parameter name inside brackets',
				"<?php the_author_meta( 'user_registered' ); ?>" => 'See parameter name inside brackets',
				"<?php the_author_meta( 'user_activation_key' ); ?>" => 'See parameter name inside brackets',
				"<?php the_author_meta( 'user_status' ); ?>" => 'See parameter name inside brackets',
				"<?php the_author_meta( 'display_name' ); ?>" => 'See parameter name inside brackets',
				"<?php the_author_meta( 'nickname' ); ?>" => 'See parameter name inside brackets',
				"<?php the_author_meta( 'first_name' ); ?>" => 'See parameter name inside brackets',
				"<?php the_author_meta( 'last_name' ); ?>" => 'See parameter name inside brackets',
				"<?php the_author_meta( 'description' ); ?>" => 'See parameter name inside brackets',
				"<?php the_author_meta( 'jabber' ); ?>" => 'See parameter name inside brackets',
				"<?php the_author_meta( 'aim' ); ?>" => 'See parameter name inside brackets',
				"<?php the_author_meta( 'yim' ); ?>" => 'See parameter name inside brackets',
				"<?php the_author_meta( 'user_level' ); ?>" => 'See parameter name inside brackets',
				"<?php the_author_meta( 'user_firstname' ); ?>" => 'See parameter name inside brackets',
				"<?php the_author_meta( 'user_lastname' ); ?>" => 'See parameter name inside brackets',
				"<?php the_author_meta( 'user_description' ); ?>" => 'See parameter name inside brackets',
				"<?php the_author_meta( 'rich_editing' ); ?>" => 'See parameter name inside brackets',
				"<?php the_author_meta( 'comment_shortcuts' ); ?>" => 'See parameter name inside brackets',
				"<?php the_author_meta( 'admin_color' ); ?>" => 'See parameter name inside brackets',
				"<?php the_author_meta( 'plugins_per_page' ); ?>" => 'See parameter name inside brackets',
				"<?php the_author_meta( 'plugins_last_view' ); ?>" => 'See parameter name inside brackets',
				"<?php the_author_meta( 'ID' ); ?>" => 'See parameter name inside brackets',
			),
			'info' => 'Displays meta info about the post author, based on the parameter inside the brackets. 
					Should be used in templates where single posts or static pages are displayed: 
					In post format templates such as <code>postformat.php</code>, <code>postformat-video.php</code>, 
					<code>postformat-link.php</code>, <code>my-other-format.php</code>,  <code>my-other-format-video.php</code> etc... 
					Also in <code>single.php</code> and <code>page.php</code>.',
		),		
			
		'the_author_posts' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php the_author_posts(); ?>' => "Displays the total number of posts an author has published. Drafts and private posts aren't counted.",
			),
			'info' => 'Should be used in templates where single posts or static pages are displayed: 
					In post format templates such as <code>postformat.php</code>, <code>postformat-video.php</code>, 
					<code>postformat-link.php</code>, <code>my-other-format.php</code>,  <code>my-other-format-video.php</code> etc... 
					Also in <code>single.php</code> and <code>page.php</code>.',
		),		
			
		'the_author_posts_link' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php the_author_posts_link(); ?>' => "Displays a link to all posts by an author. The link text is the user's 'Display name publicly as' field.",
			),
			'info' => 'Should be used in templates where single posts or static pages are displayed: 
					In post format templates such as <code>postformat.php</code>, <code>postformat-video.php</code>, 
					<code>postformat-link.php</code>, <code>my-other-format.php</code>,  <code>my-other-format-video.php</code> etc... 
					Also in <code>single.php</code> and <code>page.php</code>.',
		),		
			
			
		'the_excerpt' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php the_excerpt(); ?>' => 'Displays an excerpt of the current post.',
			),
			'info' => 'Use in post format templates such as <code>postformat.php</code>, <code>postformat-video.php</code>, 
					<code>postformat-link.php</code>, <code>my-other-format.php</code>,  <code>my-other-format-video.php</code> etc... ',
		),		
			
		'the_content' => array( 
			'type' => 'function',
			'examples' => array(
				"<?php the_content(); ?>" => 'Displays the contents of the current post. This default version is sufficient on single.php and page.php because there no 
					"Read more" link is displayed anyway.',
				"<?php the_content('Read more...'); ?>" => 'Displays the contents of the current post.',
			),
			'info' => 'Should be used in <code>single.php</code> and <code>page.php</code>. Can also be used 
					instead of <code>the_excerpt()</code> in post format templates: <code>postformat.php</code>, <code>postformat-video.php</code>, 
					<code>postformat-link.php</code>, <code>my-other-format.php</code>,  <code>my-other-format-video.php</code> etc... 
					',
		),		
			
		'wp_link_pages' => array( 
			'type' => 'array',
			//'type' => 'queryarray',
			'examples' => array(
				"<?php wp_link_pages( array( 'before' => __('<p class=\"post-pagination\">Pages:', 'montezuma'), 'after' => '</div>' ) ); ?>" => 'Displays page-links for paginated posts.',
			),
			'info' => 'This will only display something in posts that include the <code>&lt;!--nextpage--&gt;</code> Quicktag one or more times.',
		),		

		'bfa_link_pages' => array( 
			'type' => 'array',
			//'type' => 'queryarray',
			'examples' => array(
				"<?php bfa_link_pages( array( 'before' => __('<p class=\"post-pagination\">Pages:', 'montezuma'), 'after' => '</div>' ) ); ?>" => 'Displays page-links for paginated posts.',
			),
			'info' => 'This will only display something in posts that include the <code>&lt;!--nextpage--&gt;</code> Quicktag one or more times. 
			This is a advanced version of <code>wp_link_pages()</code>. Unlike the WP version, this function wraps the current page number into a span to be able to stylke that, too.',
		),	
		
		'edit_post_link' => array( 
			'type' => 'function',
			'examples' => array(
				"<?php edit_post_link( __('Edit', 'montezuma'), '<div class=\"post-edit\">', '</div>' ); ?>" => 'Displays a link to edit the current post. ',
			),
			'info' => 'This will only display something if a user is logged in and allowed to edit the post.',
		),		
			
		'the_category' => array( 
			'type' => 'function',
			'examples' => array(
				"<?php the_category(' &middot; '); ?>" => 'Displays a link to the category or categories a post belongs to, with " &middot; " as the separator between multiple category names.',
			),
			'info' => 'Could be used in post format templates 
					(<code>postformat.php</code>, <code>postformat-video.php</code>, 
					<code>postformat-link.php</code>, <code>my-other-format.php</code>,  <code>my-other-format-video.php</code> etc... ) 
					and in <code>single.php</code> and <code>page.php</code>.',
		),		
			
		'the_tags' => array( 
			'type' => 'function',
			'examples' => array(
				"<?php the_tags( '<p class=\"post-tags\">', ' &middot; ', '</p>' ); ?>" 
					=> 'This template tag displays a link to the tag or tags a post belongs to, with " &middot; " as the separator between multiple tag names.',
			),
			'info' => 'If no tags are associated with the current entry, nothing is displayed. Could be used in post format templates 
					(<code>postformat.php</code>, <code>postformat-video.php</code>, 
					<code>postformat-link.php</code>, <code>my-other-format.php</code>,  <code>my-other-format-video.php</code> etc... ) 
					and in <code>single.php</code> and <code>page.php</code>.',
		),		
			
		'the_taxonomies' => array( 
			'type' => 'function',
			'examples' => array(
				'<?php the_taxonomies(); ?>' => 'Displays the taxonomies for a post.',
			),
			'info' => '',
		),		
		
		'comments_template' => array( 
			'type' => 'function',
			'examples' => array(
				'<?php comments_template(); ?>' => 'Displays the comment template. ',
			),
			'info' => 'For use in <code>single.php</code>, <code>page.php</code> and other "singular" templates.',
		),		
					
		'bfa_thumb' => array( 
			'type' => 'function',
			'examples' => array(
				'<?php bfa_thumb( $width, $height, $crop = false, $before = \'\', $after = \'\', $link = \'permalink\' ); ?>' => 
				'This shows the available parameters and their default values if any.',
				'<?php bfa_thumb( 620, 180, true); ?>' => 'Displays a post thumbnail with width 620px, height 180px, cropped, nothing before, 
				nothing after, linked to post.',
			),
			'info' => 'Should be used in post format templates (<code>postformat.php</code>, <code>postformat-video.php</code>, 
					<code>postformat-link.php</code>, <code>my-other-format.php</code>,  <code>my-other-format-video.php</code> etc...).
				If a "Featured Image" was set for the post, that one will 
				be used. Else, the first attached/inserted local image in the post will be used. 
				Else the first local image URL will be used. External images will not be used. Will create thumbnail on the fly if it does 
				not exist. You can use different values for width, height and crop (true/false) in each post format template.	
				Possible values for <code>$link</code>: \'permalink\' (links to post) or empty (not linked). TODO: Add \'fullsize\' to link to full size version of image. 				
					
					',
		),		
			
		'bfa_comments_popup_link' => array( 
			'type' => 'function',
			'examples' => array(
				'<?php bfa_comments_popup_link(); ?>' => 'Displays link to comments of a post IF there are comments. Can show the comment number.',
			),
			'info' => 'Can be used in post format templates (<code>postformat.php</code>, <code>postformat-video.php</code>, 
					<code>postformat-link.php</code>, <code>my-other-format.php</code>,  <code>my-other-format-video.php</code> etc... ). 
					Should not be used in singular templates like <code>single.php</code> and <code>page.php</code>.',
		),	
		
		'bfa_comments_number' => array( 
			'type' => 'function',
			'examples' => array(
				'<?php bfa_comments_number(); ?>' => 'Displays number of comments IF there are any.',
			),
			'info' => '',
		),	


		'comment_reply_link' => array( 
			'type' => 'array',
			'examples' => array(
				"<?php comment_reply_link( array( 
			'reply_text' => __( 'Reply', 'montezuma' ), 
			'login_text' => __( 'Log in to Reply', 'montezuma' ),
			'depth' => 1,
			'max_depth' => 3) ); ?>" => 'Displays direct reply link for individual comment.'
			),
			'info' => 'Should be used in sub templates <code>comments-comment.php</code>',
		),
		

		'bfa_attachment_url' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php bfa_attachment_url(); ?>' => 'Prints the URL of an attachment.',
			),
			'info' => 'Use on attachment templates such as image.php',
		),	

		'bfa_attachment_image' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php bfa_attachment_image( $size ); ?>' => 'Displays number of comments IF there are any.',
			),
			'info' => 'Use on attachment templates such as image.php. Replace $size with <code>thumbnail</code>, <code>medium</code>, <code>large</code> or <code>full</code>.',
		),	

		'bfa_parent_permalink' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php bfa_parent_permalink(); ?>' => 'Prints the permalink of the post\'s PARENT.',
			),
			'info' => 'Use on attachment templates such as image.php',
		),	

		'bfa_parent_title' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php bfa_parent_title(); ?>' => 'Prints the title of the post\'s PARENT.',
			),
			'info' => 'Use on attachment templates such as image.php',
		),	
		
		'bfa_attachment_caption' => array( 
			'type' => 'single',
			'examples' => array(
				'<?php bfa_attachment_caption( $before, $after ); ?>' => 'Prints caption of an attachment, if it exists, with some HTML before and after.',
			),
			'info' => 'Use on attachment templates such as image.php',
		),	
		
		'bfa_image_meta' => array( 
			'type' => 'array',
			'examples' => array(
				"<?php bfa_image_meta( array( 
					'keys' => '',
					'before' => '<ul>', 
					'after' => '</ul>',
					'item_before' => '<li>', 
					'item_after' => '</li>',
					'item_sep' => '',
					'key_before' => '',
					'key_after' => ': ',
					'value_before' => '',
					'value_after' => '',
					'display_empty' => false
				) ); ?>"
					=> 'Displays all image meta data, alphabetically sorted, with the specified HTML tags.',
				"<?php bfa_image_meta(); ?>" 
					=> 'Same as above. Uses the default settings.',
				"<?php bfa_image_meta( array( 
					'keys' => 'camera, aperture, focal_length, shutter_speed',
					'before' => '<p class=\"my-image-meta\">', 
					'after' => '</p>',
					'item_before' => '', 
					'item_after' => '',
					'item_sep' => ', ',
					'key_after' => '= ',
				) ); ?>"
					=> 'Displays only the image meta data specified in parameter "keys" (and in that order), wrapped in a paragraph tag with class 
					"my-image-meta", with a comma between the data items, and a "= " after each key.',
			),
			'info' => 'Useful on image.php template for displaying details of the given image. The default full set of "keys" is: 
			width, height, aperture, credit, camera, caption, created_timestamp, copyright, focal_length, iso, shutter_speed, title.',
		),	
		
		
	);
		


	$whitelist = array_merge( $wl_global, $wl_loop );
	
	ksort( $whitelist );
	
	return $whitelist;
}

