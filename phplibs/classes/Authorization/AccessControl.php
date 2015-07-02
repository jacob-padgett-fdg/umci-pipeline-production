<?php
/**
 * Created by PhpStorm.
 * User: greg
 * Date: 12/9/14
 * Time: 11:51 AM
 */

namespace classes\Authorization;

require_once('classes/Data/Role.php');
require_once('classes/Data/ContactsRole.php');

use classes\Data\Role;
use classes\Data\ContactsRole;

class AccessControl
{
    /**
     * isAdmin
     * @param $userId
     * @return bool
     */
    public static function isAdmin($userId = 0)
    {
        if (!$userId) {
            $userId = $_SESSION['login_id'];
        }
        $roleId = Role::findByType('admin');
        $contactsRole = ContactsRole::findByUserIdRoleId($userId, $roleId[0]->id);
        return (empty($contactsRole) ? false : ($contactsRole[0]->user_id == $userId));
    }


    /**
     * isProjectManager
     * @param $userId
     * @return bool
     */
    public static function isProjectManager($userId = 0)
    {
        if (!$userId) {
            $userId = $_SESSION['login_id'];
        }
        $roleId = Role::findByType('project manager');
        $contactsRole = ContactsRole::findByUserIdRoleId($userId, $roleId[0]->id);
        return (empty($contactsRole) ? false : ($contactsRole[0]->user_id == $userId));
    }


    /**
     * getRoleCount
     * Returns # of roles associated with user
     * @param int $userId
     * @return int
     */
    public static function getRoleCount($userId = 0)
    {
        if (!$userId) {
            $userId = $_SESSION['login_id'];
        }

        $contactsRole = ContactsRole::findByColumnsValues(array('user_id' => $userId));
        return count($contactsRole);
    }
}
