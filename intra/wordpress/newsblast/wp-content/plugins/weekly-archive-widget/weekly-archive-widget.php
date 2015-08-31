<?php
/*
Plugin Name: Weekly Archive Widget
Plugin URI: http://judenware.com/projects/wordpress/weekly-archive-widget/
Version: 1.0
Description: Create a widget that displays the archives by week
Author: ericjuden
Author URI: http://www.judenware.com/
*/

class Weekly_Archive_Widget extends WP_Widget {
    function Weekly_Archive_Widget(){
        $widget_ops = array('classname' => 'weekly-archive-widget', 'description' => _('Create a widget that displays the archives by week'));
        $this->WP_Widget('Weekly_Archive_Widget', _('Weekly Archive'), $widget_ops);
    }
    
    function form($instance){
        $instance = wp_parse_args((array) $instance, array('title' => ''));
        $title = $instance['title'];
?>
		<p><label for="<?php echo $this->get_field_id('title'); ?>">Title: <input class="widefat" id="<?php echo $this->get_field_id('title'); ?>" name="<?php echo $this->get_field_name('title'); ?>" type="text" value="<?php echo esc_attr($title); ?>" /></label></p>
<?php
    }
    
    function update($new_instance, $old_instance){
        $instance = $old_instance;
        $instance['title'] = $new_instance['title'];
        return $instance;
    }
    
    function widget($args, $instance){
        extract($args, EXTR_SKIP);
        
        echo $before_widget;
        $title = empty($instance['title']) ? '' : apply_filters('widget_title', $instance['title']);
        
        if(!empty($title)){
            echo $before_title . $title . $after_title;
        }
?>
        <ul>
			<select name="weekly-archive-dropdown" onChange='document.location.href=this.options[this.selectedIndex].value;'>
		    	<option value=""><?php _e('Select Week'); ?></option>
		    	<?php wp_get_archives('type=weekly&format=option'); ?>
		    </select>
		</ul>
<?php
        echo $after_widget;
    }
}

add_action('widgets_init', create_function('', 'return register_widget("Weekly_Archive_Widget");'));
?>