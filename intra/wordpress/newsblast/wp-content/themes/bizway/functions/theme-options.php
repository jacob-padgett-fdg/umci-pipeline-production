<?php

add_action('init', 'bizway_options');
if (!function_exists('bizway_options')) {

    function bizway_options() {
        // VARIABLES
        $themename = get_theme_data(get_template_directory() . '/style.css');
        $themename = $themename['Name'];
        $shortname = "of";
        // Populate OptionsFramework option in array for use in theme
        global $of_options;
        $of_options = bizway_get_option('of_options');
        // Background Defaults
        $background_defaults = array('color' => '', 'image' => '', 'repeat' => 'repeat', 'position' => 'top center', 'attachment' => 'scroll');
        //Front page on/off
        $file_rename = array("on" => "On", "off" => "Off");
        //Stylesheet Reader
        $alt_stylesheets = array("default" => "default", "blue" => "blue", "coffee" => "coffee", "green" => "green", "brown" => "brown", "pink" => "pink", "orange" => "orange", "purple" => "purple", "red" => "red", "forrest-green" => "forrest-green", "yellow" => "yellow");
        // Pull all the categories into an array
        $options_categories = array();
        $options_categories_obj = get_categories();
        foreach ($options_categories_obj as $category) {
            $options_categories[$category->cat_ID] = $category->cat_name;
        }

        // Pull all the pages into an array
        $options_pages = array();
        $options_pages_obj = get_pages('sort_column=post_parent,menu_order');
        $options_pages[''] = 'Select a page:';
        foreach ($options_pages_obj as $page) {
            $options_pages[$page->ID] = $page->post_title;
        }

        // If using image radio buttons, define a directory path
        $imagepath = get_stylesheet_directory_uri() . '/images/';

        $options = array(
            array("name" => "General Settings",
                "type" => "heading"),
            array("name" => "Custom Logo",
                "desc" => "Choose your own logo. Optimal Size: 300px Wide by 90px Height.",
                "id" => "bizway_logo",
                "type" => "upload"),
            array("name" => "Custom Favicon",
                "desc" => "Specify a 16px x 16px image that will represent your website's favicon.",
                "id" => "bizway_favicon",
                "type" => "upload"),
            array("name" => "Tracking Code",
                "desc" => "Paste your Google Analytics (or other) tracking code here.",
                "id" => "bizway_analytics",
                "std" => "",
                "type" => "textarea"),
            //Home page heading
            array("name" => "Homepage Settings",
                "type" => "heading"),
            //First Heading
            array("name" => "First Heading",
                "desc" => "Enter your text for first heading.",
                "id" => "bizway_first_head",
                "std" => "",
                "type" => "textarea"),
            //Second Heading
            array("name" => "Second Heading",
                "desc" => "Enter your text for second heading.",
                "id" => "bizway_second_head",
                "std" => "",
                "type" => "textarea"),
            //Home Page Slider Setting
            array("name" => "Slider Settings",
                "type" => "heading"),
            //First Slider
            array("name" => "Slider Image1",
                "desc" => "Choose your image for first slider. Optimal size is 950px wide and 350px height.",
                "id" => "bizway_slideimage1",
                "std" => "",
                "type" => "upload"),
            array("name" => "Slide 1 Link",
                "desc" => "Enter your link url for slide1",
                "id" => "bizway_slidelink1",
                "std" => "",
                "type" => "text"),
            //Second Slider
            array("name" => "Slider Image2",
                "desc" => "Choose your image for second slider. Optimal size is 950px wide and 350px height.",
                "id" => "bizway_slideimage2",
                "std" => "",
                "type" => "upload"),
            array("name" => "Slide 2 Link",
                "desc" => "Enter your link url for slide2",
                "id" => "bizway_slidelink2",
                "std" => "",
                "type" => "text"),
//Homepage Feature Area
            array("name" => "Homepage Feature Area",
                "type" => "heading"),
            //Right Feature Area
            array("name" => "Homepage Feature Area First Image",
                "desc" => "Choose your image for homepage feature area first image.",
                "id" => "bizway_featureimg1",
                "std" => "",
                "type" => "upload"),
            array("name" => "First Feature Heading",
                "desc" => "Enter your text for first col heading.",
                "id" => "bizway_firsthead",
                "std" => "",
                "type" => "textarea"),
            array("name" => "First Feature Description",
                "desc" => "Enter your text for first col description.",
                "id" => "bizway_firstdesc",
                "std" => "",
                "type" => "textarea"),
            array("name" => "First Feature Link URL",
                "desc" => "Enter your link url for fourth feature section.",
                "id" => "bizway_link1",
                "std" => "",
                "type" => "text"),
            //Second Feature Separetor
            array("name" => "Second Feature Starts From Here.",
                "type" => "saperate",
                "class" => "saperator"),
            array("name" => "Homepage Feature Area Second Image",
                "desc" => "Choose your image for homepage Feature area second image.",
                "id" => "bizway_featureimg2",
                "std" => "",
                "type" => "upload"),
            array("name" => "Second Feature Heading",
                "desc" => "Enter your text for second col heading.",
                "id" => "bizway_secondhead",
                "std" => "",
                "type" => "textarea"),
            array("name" => "Second Col Description",
                "desc" => "Enter your text for second col description.",
                "id" => "bizway_seconddesc",
                "std" => "",
                "type" => "textarea"),
            array("name" => "Second Feature Link URL",
                "desc" => "Enter your link url for fourth feature section.",
                "id" => "bizway_link2",
                "std" => "",
                "type" => "text"),
            //Third Feature Separetor
            array("name" => "Third Feature Starts From Here.",
                "type" => "saperate",
                "class" => "saperator"),
            array("name" => "Homepage Third Feature  Image",
                "desc" => "Choose your image for homepage Feature area third image.",
                "id" => "bizway_featureimg3",
                "std" => "",
                "type" => "upload"),
            array("name" => "Third Feature Heading",
                "desc" => "Enter your text for second col heading.",
                "id" => "bizway_thirdhead",
                "std" => "",
                "type" => "textarea"),
            array("name" => "Third Feature Description",
                "desc" => "Enter your text for Third Feature description.",
                "id" => "bizway_thirddesc",
                "std" => "",
                "type" => "textarea"),
            array("name" => "Third Feature Link URL",
                "desc" => "Enter your link url for fourth feature section.",
                "id" => "bizway_link3",
                "std" => "",
                "type" => "text"),
//****=============================================================================****//
//****-----------This code is used for creating color styleshteet options----------****//							
//****=============================================================================****//				
            array("name" => "Styling Options",
                "type" => "heading"),
            array("name" => "Custom CSS",
                "desc" => "Quickly add some CSS to your theme by adding it to this block.",
                "id" => "bizway_customcss",
                "std" => "",
                "type" => "textarea"));

        bizway_update_option('of_template', $options);
        bizway_update_option('of_themename', $themename);
        bizway_update_option('of_shortname', $shortname);
    }

}