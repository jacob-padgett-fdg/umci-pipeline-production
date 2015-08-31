<?php

include_once get_template_directory() . '/functions/bizway-functions.php';
$functions_path = get_template_directory() . '/functions/';
/* These files build out the options interface.  Likely won't need to edit these. */
require_once ($functions_path . 'admin-functions.php');  // Custom functions and plugins
require_once ($functions_path . 'admin-interface.php');  // Admin Interfaces (options,framework, seo)
/* These files build out the theme specific options and associated functions. */
require_once ($functions_path . 'theme-options.php');   // Options panel settings and custom settings
require_once ($functions_path . 'dynamic-image.php');
require_once ($functions_path . 'themes-page.php');

/* ----------------------------------------------------------------------------------- */
/* jQuery Enqueue */
/* ----------------------------------------------------------------------------------- */

function bizway_wp_enqueue_scripts() {
    if (!is_admin()) {
        wp_enqueue_script('jquery');
        wp_enqueue_script('bizway-ddsmoothmenu', get_template_directory_uri() . '/js/ddsmoothmenu.js', array('jquery'));
        wp_enqueue_script('bizway-slider', get_template_directory_uri() . '/js/jquery.flexslider-min.js', array('jquery'));      
        wp_enqueue_script('bizway-mobilemenu', get_template_directory_uri() . '/js/mobilemenu.js', array('jquery'));
        wp_enqueue_script('bizway-custom', get_template_directory_uri() . '/js/custom.js', array('jquery'));       
    } elseif (is_admin()) {
        
    }
}

add_action('wp_enqueue_scripts', 'bizway_wp_enqueue_scripts');
//Front Page Rename
$get_status = bizway_get_option('re_nm');
$get_file_ac = TEMPLATEPATH . '/front-page.php';
$get_file_dl = TEMPLATEPATH . '/front-page-hold.php';
//True Part
if ($get_status === 'off' && file_exists($get_file_ac)) {
    rename("$get_file_ac", "$get_file_dl");
}
//False Part
if ($get_status === 'on' && file_exists($get_file_dl)) {
    rename("$get_file_dl", "$get_file_ac");
}

//
function bizway_get_option($name) {
    $options = get_option('bizway_options');
    if (isset($options[$name]))
        return $options[$name];
}

//
function bizway_update_option($name, $value) {
    $options = get_option( 'bizway_options' );
    $options[$name] = $value;
    return update_option( 'bizway_options', $options );
}

//
function bizway_delete_option($name) {
    $options = get_option('bizway_options');
    unset( $options[$name] );
    return update_option( 'bizway_options', $options );
}

//Enqueue comment thread js
function bizway_enqueue_scripts() {
    if (is_singular() and get_site_option('thread_comments')) {
        wp_print_scripts('comment-reply');
    }
}

add_action('wp_enqueue_scripts', 'bizway_enqueue_scripts');