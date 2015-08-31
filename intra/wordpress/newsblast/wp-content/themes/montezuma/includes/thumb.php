<?php 
/*
 * Resize images dynamically using wp built in functions
 * Victor Teixeira
 *
 * php 5.2+
 *
 * Exemplo de uso:
 * 
 * <?php 
 * $thumb = get_post_thumbnail_id(); 
 * $image = vt_resize( $thumb, '', 140, 110, true );
 * ?>
 * <img src="<?php echo $image[url]; ?>" width="<?php echo $image[width]; ?>" height="<?php echo $image[height]; ?>" />
 *
 * @param int $attach_id
 * @param string $img_url
 * @param int $width
 * @param int $height
 * @param bool $crop
 * @return array
 */

function bfa_delete_thumb_transient( $post_id ) {
	delete_site_transient( 'bfa_thumb_' . $post_id );
}
add_action( 'edit_post', 'bfa_delete_thumb_transient' );

 /* TODO: Add parameter $link = 'fullsize' to link to full size image */
function bfa_thumb( $width, $height, $crop = false, $before = '', $after = '', $link = 'permalink' ) {

	global $post, $upload_dir;
	
	if( ! is_writable( $upload_dir['basedir'] ) ) 
		return;
	
	$id = $post->ID;
	/* if the other fails : */
	/*
	global $wp_query;
	$id = $wp_query->post->ID;
	*/
	
	#$bfa_thumb = get_site_transient( 'bfa_thumb_' . $id );
	
	#if ( false === $bfa_thumb ) {
		// It wasn't there, so regenerate the data and save the transient

		$bfa_thumb = '';
	
		$hasthumb = FALSE;
		$hassrc = FALSE;
		$has_thumbnail = FALSE;
		
		if( has_post_thumbnail() ) {
			$thumb = get_post_thumbnail_id(); 
			$hasthumb = TRUE;
		} elseif ( bfa_has_attachment() ) {
			$thumb = bfa_get_first_attachment_id(); 
			$hasthumb = TRUE;
		} elseif( bfa_has_img_src() ) {
			$thumb = bfa_get_first_img_src();
			$hassrc = TRUE;
		
		}
		
		if( $hasthumb == TRUE ) {
			$thumbimage = bfa_vt_resize( $thumb,'' , $width, $height, $crop );
			$has_thumbnail = TRUE;
		} elseif( $hassrc == TRUE ) {
			$thumbimage = bfa_vt_resize( '', $thumb , $width, $height, $crop );
			$has_thumbnail = TRUE;
		}	
		
		if( $has_thumbnail == TRUE ) { 
			$bfa_thumb .= $before;
			if( $link == 'permalink' ) 
				$bfa_thumb .= '<a href="' . get_permalink( $id ) . '">';
		
			$bfa_thumb .= '<img src="' . $thumbimage['url'] . '" width="' . $thumbimage['width'] . '" height="' . $thumbimage['height'] . '" />
			<span></span>';
		
			if( $link == 'permalink' ) 
				$bfa_thumb .= '</a>';
			$bfa_thumb .= $after;
		} 
	
		#set_site_transient( 'bfa_thumb_' . $id, $bfa_thumb, 60*60*1 );
	#} 
	
	echo $bfa_thumb;
}	

	
function bfa_has_attachment() {

	global $post;	
		
	$args = array( 'post_type' => 'attachment', 'numberposts' => -1, 'post_status' => null, 'post_parent' => $post->ID ); 
	$attachments = get_posts($args);
	if ($attachments) 
		return TRUE;
		
	return FALSE;
}

function bfa_get_first_attachment_id() {

	global $post;	
		
	$args = array( 'post_type' => 'attachment', 'numberposts' => -1, 'post_status' => null, 'post_parent' => $post->ID ); 
	$attachments = get_posts($args);
	return $attachments[0]->ID;
}


function bfa_has_img_src( $args = array() ) {

	global $post;

	$site_url = site_url();
	preg_match_all( '|<img.*?src=[\'"](.*?)[\'"].*?>|i', $post->post_content, $matches );

	foreach( $matches[1] as $match ) {
		if ( isset( $match ) && strpos( $match, $site_url ) !== FALSE )
		return TRUE;
	}
	return FALSE;
}

function bfa_get_first_img_src( $args = array() ) {

	global $post;
	$site_url = site_url();
	preg_match_all( '|<img.*?src=[\'"](.*?)[\'"].*?>|i', $post->post_content, $matches );

	foreach( $matches[1] as $match ) {
		if ( isset( $match ) && strpos( $match, $site_url ) !== FALSE )
			return $match;
	}
	
	return false;
		
}


function bfa_vt_resize( $attach_id = null, $img_url = null, $width, $height, $crop = false ) {

	// this is an attachment, so we have the ID
	if ( $attach_id ) {
	
		$image_src = wp_get_attachment_image_src( $attach_id, 'full' );
		$file_path = get_attached_file( $attach_id );
	
	// this is not an attachment, let's use the image url
	} else if ( $img_url ) {
		
		$file_path = parse_url( $img_url );
		// $file_path = $_SERVER['DOCUMENT_ROOT'] . $file_path['path'];
		$file_path = str_replace( '//', '/', $_SERVER['DOCUMENT_ROOT'] . $file_path['path']);
		
		//$file_path = ltrim( $file_path['path'], '/' );
		//$file_path = rtrim( ABSPATH, '/' ).$file_path['path'];
		
		$orig_size = getimagesize( $file_path );
		
		$image_src[0] = $img_url;
		$image_src[1] = $orig_size[0];
		$image_src[2] = $orig_size[1];
	}
	
	$file_info = pathinfo( $file_path );
	$extension = '.'. $file_info['extension'];

	// the image path without the extension
	$no_ext_path = $file_info['dirname'].'/'.$file_info['filename'];

	$cropped_img_path = $no_ext_path.'-'.$width.'x'.$height.$extension;

	// checking if the file size is larger than the target size
	// if it is smaller or the same size, stop right here and return
	if ( $image_src[1] > $width || $image_src[2] > $height ) {

		// the file is larger, check if the resized version already exists (for $crop = true but will also work for $crop = false if the sizes match)
		if ( file_exists( $cropped_img_path ) ) {

			$cropped_img_url = str_replace( basename( $image_src[0] ), basename( $cropped_img_path ), $image_src[0] );
			
			$vt_image = array (
				'url' => $cropped_img_url,
				'width' => $width,
				'height' => $height
			);
			
			return $vt_image;
		}

		// $crop = false
		if ( $crop == false ) {
		
			// calculate the size proportionaly
			$proportional_size = wp_constrain_dimensions( $image_src[1], $image_src[2], $width, $height );
			$resized_img_path = $no_ext_path.'-'.$proportional_size[0].'x'.$proportional_size[1].$extension;			

			// checking if the file already exists
			if ( file_exists( $resized_img_path ) ) {
			
				$resized_img_url = str_replace( basename( $image_src[0] ), basename( $resized_img_path ), $image_src[0] );

				$vt_image = array (
					'url' => $resized_img_url,
					'width' => $proportional_size[0],
					'height' => $proportional_size[1]
				);
				
				return $vt_image;
			}
		}

		// no cache files - let's finally resize it
		$new_img_path = image_resize( $file_path, $width, $height, $crop );
		$new_img_size = getimagesize( $new_img_path );
		$new_img = str_replace( basename( $image_src[0] ), basename( $new_img_path ), $image_src[0] );

		// resized output
		$vt_image = array (
			'url' => $new_img,
			'width' => $new_img_size[0],
			'height' => $new_img_size[1]
		);
		
		return $vt_image;
	}

	// default output - without resizing
	$vt_image = array (
		'url' => $image_src[0],
		'width' => $image_src[1],
		'height' => $image_src[2]
	);
	
	return $vt_image;
}