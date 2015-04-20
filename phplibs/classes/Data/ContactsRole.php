<?php
/**
 * Created by PhpStorm.
 * User: greg
 * Date: 12/9/14
 * Time: 12:39 PM
 */

namespace classes\Data;

require_once('BaseDataAccess.php');
require_once('Role.php');

class ContactRole extends BaseDataAccess
{
    public $table = 'user_role';
    public $primaryKey = 'id';

    public $id;
    public $user_id;
    public $role_id;

    /**
     * @param $userId
     * @param $roleId
     * @return array
     */
    public static function findByUserIdRoleId($userId, $roleId)
    {
        $sql = <<<SQL
SELECT
  id,
  user_id,
  role_id
FROM user_role
WHERE user_id = ?
AND role_id = ?
SQL;
        return self::fetchArray($sql, array($userId, $roleId));
    }

    /**
     * assignRoleByUserId
     * @param $userId
     * @param $role
     * @return int
     */
    public static function assignRoleByUserId($userId, $role)
    {
        $ret = 1;   // assume failure
        $rArr = Role::findByType($role);
        if (!empty($rArr)) {
            $urArr = ContactRole::findByUserIdRoleId($userId, $rArr[0]->id);
            if(empty($urArr)) {
                $ur = new ContactRole();
                $ur->role_id = $rArr[0]->id;
                $ur->user_id = $userId;
                $ur->insert();
                $ret = 0;
            }
        }
        return $ret;
    }

    /**
     * deleteRoleByUserId
     * @param $userId
     * @param $role
     * @return int
     */
    public static function deleteRoleByUserId($userId, $role)
    {
        $sql = <<<SQL
DELETE FROM user_role
WHERE user_id = ?
AND role_id = ?
SQL;
        $ret = 1;   // assume failure
        $rArr = Role::findByType($role);
        if (!empty($rArr)) {
            $dbh = self::getConnection();
            $sth = $dbh->prepare($sql);
            $sth->execute(array($userId, $rArr[0]));
            $ret = 0;
        }
        return $ret;
    }

    /**
     * @param $userId
     * @return array
     */
    public static function findRoles($userId)
    {
        $sql = <<<SQL
SELECT
  ur.id,
  ur.user_id,
  ur.role_id,
  r.type
FROM user_role ur
INNER JOIN role r
ON r.id = ur.role_id
WHERE user_id = ?
SQL;
        return self::fetchArray($sql, array($userId));
    }
}