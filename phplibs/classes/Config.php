<?php
/**
 * Created by PhpStorm.
 * User: greg
 * Date: 12/9/14
 * Time: 11:57 AM
 */
namespace classes;

define('DIR_BASE',      dirname( dirname( __FILE__ ) ) . "/");
define('DIR_VIEWS',     DIR_BASE . 'views/');

/**
 * Class Config - load site ini with local overrides
 */

class Config {

    const BASE_DIR = "/../config/";
    const SITE_INI = "site.ini";
    const LOCAL_INI = "local.ini";

    public static function getInstance()
    {
        // set path relative to this class
        $path = dirname(__FILE__) . self::BASE_DIR;
        // error_log("Loading config from path=" . $path);

        // load site config file
        if(file_exists($path . self::SITE_INI))
        {
            $config = parse_ini_file($path . self::SITE_INI);
        }
        // if local config exists, use overrides.
        // used for development
        // add LOCAL_INI to gitignore to deploy to production
        if (file_exists($path . self::LOCAL_INI))
        {
            $localConfig = parse_ini_file($path . self::LOCAL_INI);
            foreach ($localConfig as $key => $value) {
                $config[$key] = $value;
            }
        }
        // Set error reporting from GET flag //
        if (isset($_REQUEST['debug'])) {
            if ("true" === $_REQUEST['debug']) {
                $_SESSION['debug'] = true;
            }else if ("false" === $_REQUEST['debug']) {
                $_SESSION['debug'] = false;
            }
        }

        if (isset($_REQUEST['debug']) && true == $_SESSION['debug']) {
            /*ini_set('display_errors', 1);
            ini_set('display_startup_errors', 1);*/
            error_reporting(E_ALL);
        }else{
            ini_set('display_errors', 0);
            ini_set('display_startup_errors', 0);
            error_reporting(0);
        }

        return $config;
    }

}