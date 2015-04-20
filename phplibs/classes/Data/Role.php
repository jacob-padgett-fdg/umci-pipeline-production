<?php
/**
 * Created by PhpStorm.
 * User: greg
 * Date: 12/9/14
 * Time: 11:55 AM
 */

namespace classes\Data;

require_once('BaseDataAccess.php');

class Role extends BaseDataAccess
{
    public $table = 'role';
    public $primaryKey = 'id';

    public $id;
    public $type;

    public static function findByType($type)
    {
        $sql = <<<SQL
SELECT
  id,
  type
FROM role
WHERE type = ?
SQL;
        return self::fetchArray($sql, array($type));
    }
}
