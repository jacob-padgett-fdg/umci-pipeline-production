<?php
/**
 * The base configurations of the WordPress.
 *
 * This file has the following configurations: MySQL settings, Table Prefix,
 * Secret Keys, WordPress Language, and ABSPATH. You can find more information
 * by visiting {@link http://codex.wordpress.org/Editing_wp-config.php Editing
 * wp-config.php} Codex page. You can get the MySQL settings from your web host.
 *
 * This file is used by the wp-config.php creation script during the
 * installation. You don't have to use the web site, you can just copy this file
 * to "wp-config.php" and fill in the values.
 *
 * @package WordPress
 */

// ** MySQL settings - You can get this info from your web host ** //
/** The name of the database for WordPress */
define('DB_NAME', 'wordpress');

/** MySQL database username */
define('DB_USER', 'wordpress');

/** MySQL database password */
define('DB_PASSWORD', 'r00tb33r');

/** MySQL hostname */
define('DB_HOST', 'localhost');

/** Database Charset to use in creating database tables. */
define('DB_CHARSET', 'utf8');

/** The Database Collate type. Don't change this if in doubt. */
define('DB_COLLATE', '');

/**#@+
 * Authentication Unique Keys and Salts.
 *
 * Change these to different unique phrases!
 * You can generate these using the {@link https://api.wordpress.org/secret-key/1.1/salt/ WordPress.org secret-key service}
 * You can change these at any point in time to invalidate all existing cookies. This will force all users to have to log in again.
 *
 * @since 2.6.0
 */
define('AUTH_KEY',         ')i=+(9I8n`qPHDzmuT];a|>aKqj8Z7VP+W]CNqN)<S-M!u!*snv|<)8;1`5wggPj');
define('SECURE_AUTH_KEY',  'W^15nl7!n5d?ND(RRZ4F!$l+ rV,Aal]S:]/0idCG@nlc/}7j8_u/3PA^FjaqRdD');
define('LOGGED_IN_KEY',    'A|bxdS1.Y-R+9a}MF86|EbK^&P@_wpB7SZ~)x}59sVuYt+ jRN$%q[{,3_+>L&`n');
define('NONCE_KEY',        '+c@s|X0={:DE[d&4t<O4p7bqm2{WCh8F#03jp_`_D}p-#(jX3L&xD.YA.h9Wx6.4');
define('AUTH_SALT',        'D))w~K!!/hWH<9{iGOx_4< ]WF4Ln>1T`;B1EJ3qRZSY.;A&Gx^0s@mp,s24*`jD');
define('SECURE_AUTH_SALT', 'SW+@PYRhI-#B{hWaCK) 8}q2D#_,<~,_~P|a#-#/7?SD&b!p4V|J(+p2+hz8+-<(');
define('LOGGED_IN_SALT',   'J=|zkhuvc)I q+D<C@nCwEH2DKUG|.VJ7Ls|F69S^m{lW|D k>nOoa`XfW&|u/T-');
define('NONCE_SALT',       'P{Vv~&4rZMJ`:X|vK.Dl+.KT`?|f~Yom+7+9b a)+OC:2-bX2Jc>k<+GqF(8OG_d');

/**#@-*/

/**
 * WordPress Database Table prefix.
 *
 * You can have multiple installations in one database if you give each a unique
 * prefix. Only numbers, letters, and underscores please!
 */
$table_prefix  = 'wp_';

/**
 * WordPress Localized Language, defaults to English.
 *
 * Change this to localize WordPress. A corresponding MO file for the chosen
 * language must be installed to wp-content/languages. For example, install
 * de_DE.mo to wp-content/languages and set WPLANG to 'de_DE' to enable German
 * language support.
 */
define('WPLANG', '');

/**
 * For developers: WordPress debugging mode.
 *
 * Change this to true to enable the display of notices during development.
 * It is strongly recommended that plugin and theme developers use WP_DEBUG
 * in their development environments.
 */
define('WP_DEBUG', false);

/* That's all, stop editing! Happy blogging. */

/** Absolute path to the WordPress directory. */
if ( !defined('ABSPATH') )
	define('ABSPATH', dirname(__FILE__) . '/');

/** Sets up WordPress vars and included files. */
require_once(ABSPATH . 'wp-settings.php');
