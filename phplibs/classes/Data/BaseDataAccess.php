<?php
/**
 * Created by PhpStorm.
 * User: greg
 * Date: 12/9/14
 * Time: 11:56 AM
 */

namespace classes\Data;
set_include_path(get_include_path() . ':' . $_SERVER['DOCUMENT_ROOT']);

require_once('classes/Config.php');

use classes\Config;
use Exception;
use PDO;
use PDOException;
use ReflectionClass;
use ReflectionProperty;

class BaseDataAccess
{
    public $table;
    public $primaryKey;

    /**
     * Get database handle
     * @return PDO
     */
    protected static function getConnection()
    {
        $config = Config::getInstance();
        try {
            //$dbh = new PDO('mysql:host=' . $config['db_host'] . ';dbname=' . $config['db_name'], $config['db_user'], $config['db_pass']);
            $dbh = new PDO('mysql:dbname=' . $config['db_name'], $config['db_user'], $config['db_pass']);
            return $dbh;
        } catch (PDOException $e) {
            echo $e->getMessage();
        }
    }

    /**
     * Find all records in table.  Careful with this!
     * @param null $orderBy
     * @param string $order
     * @return mixed
     */
    public static function find($orderBy = null, $order = 'ASC') {
        $obj = new static;

        /** @noinspection PhpUndefinedFieldInspection */
        $sql = 'SELECT ' . static::getColumnsString() . ' FROM ' . $obj->table;

        if($orderBy)
        {
            $sql .= ' ORDER BY ' . $orderBy . ' ' . $order;
        }

        /** @noinspection PhpUndefinedMethodInspection */
        return $obj->fetchArray($sql, array());
    }

    /**
     * @param $id
     * @return null|BaseDataAccess
     */
    public static function findById($id) {
        $obj = new static;

        /** @noinspection PhpUndefinedFieldInspection */
        $sql = 'SELECT ' . static::getColumnsString() . ' FROM ' . $obj->table . ' WHERE ' . $obj->primaryKey . ' = ?';

        /** @noinspection PhpUndefinedMethodInspection */
        return $obj->fetchSingle($sql, array($id));
    }

    /**
     * @param array $columnsValues The columns and values to search on
     * @param null $orderBy
     * @param string $order
     * @param null $distinct
     * @throws \Exception
     * @return array An array of the current class
     */
    public static function findByColumnsValues(array $columnsValues, $orderBy = null, $order = 'ASC', $distinct = null) {
        if (count($columnsValues) == 0) {
            throw new Exception("No columns or values provided");
        }

        $obj = new static;

        /** @noinspection PhpUndefinedFieldInspection */
        $sql = 'SELECT ' . ($distinct ? 'DISTINCT ' : '') . static::getColumnsString() . ' FROM ' . $obj->table . ' WHERE ';

        $searches = array();
        $values = array();

        foreach ($columnsValues as $key => $value) {
            $searches[] = '`'. $key . '` = ?';
            $values[] = $value;
        }

        $sql .= join(' AND ', $searches);

        if ($orderBy) {
            $sql .= ' ORDER BY `' . $orderBy . '` ' . $order;
        }

        /** @noinspection PhpUndefinedMethodInspection */
        return $obj->fetchArray($sql, $values);
    }

    /**
     * @param array $columnsValues The columns and values to search on
     * @param $groupBy
     * @param null $orderBy
     * @param string $order
     * @throws \Exception
     * @return array An array of the current class
     */
    public static function findByColumnsValuesGroup(array $columnsValues, $groupBy, $orderBy = null, $order = 'ASC') {
        if (count($columnsValues) == 0) {
            throw new Exception("No columns or values provided");
        }

        $obj = new static;

        /** @noinspection PhpUndefinedFieldInspection */
        $sql = 'SELECT ' . static::getColumnsString() . ' FROM ' . $obj->table . ' WHERE ';

        $searches = array();
        $values = array();

        foreach ($columnsValues as $key => $value) {
            $searches[] = '`'. $key . '` = ?';
            $values[] = $value;
        }

        $sql .= join(' AND ', $searches);

        if ($orderBy) {
            $sql .= ' ORDER BY `' . $orderBy . '` ' . $order;
        }

        if ($groupBy) {
            $sql .= ' GROUP BY `' . $groupBy . '`';
        }

        /** @noinspection PhpUndefinedMethodInspection */
        return $obj->fetchArray($sql, $values);
    }

    /**
     * @param array $columnsValues The columns and values to search on
     * @param $lessThanColumnsValues
     * @param $greaterThanEqualColumnsValues
     * @param $mandatoryColumnsValues
     * @param $groupBy
     * @param null $orderBy
     * @param string $order
     * @throws \Exception
     * @return array An array of the current class
     */
    public static function findComprehensive(array $columnsValues, $lessThanColumnsValues, $greaterThanEqualColumnsValues, $mandatoryColumnsValues, $groupBy, $orderBy = null, $order = 'ASC') {
        if (count($columnsValues) == 0) {
            throw new Exception("No columns or values provided");
        }

        $obj = new static;

        /** @noinspection PhpUndefinedFieldInspection */
        $sql = 'SELECT ' . static::getColumnsString() . ' FROM ' . $obj->table . ' WHERE ';

        $searches = array();
        $values = array();

        foreach ($columnsValues as $key => $value) {
            $searches[] = '`'. $key . '` = ?';
            $values[] = $value;
        }

        foreach ($lessThanColumnsValues as $key => $value) {
            $less[] = '`'. $key . '` < ?';
            $values[] = $value;
        }

        foreach ($greaterThanEqualColumnsValues as $key => $value) {
            $greater[] = '`'. $key . '` >= ?';
            $values[] = $value;
        }

        foreach ($mandatoryColumnsValues as $key => $value) {
            $mandatory[] = '`'. $value . '` !=  ' . "''" . ' AND `' . $value . '` IS NOT NULL ';
        }

        $sql .= join(' AND ', $searches);
        if (!empty($less)) {
            $sql .= ' AND ' . join(' AND ', $less);
        }

        if (!empty($greater)) {
            $sql .= ' AND ' . join(' AND ', $greater);
        }

        if (!empty($mandatory)) {
            $sql .= ' AND ' . join(' AND ', $mandatory);
        }

        if ($groupBy) {
            $sql .= ' GROUP BY `' . $groupBy . '`';
        }

        if (!empty($orderBy)) {
            if( is_array($orderBy)) {
                $sql .= ' ORDER BY ';
                $obCount = 0;
                foreach ($orderBy as $v) {
                    if ($obCount) {
                        $sql .= ',';
                    }
                    $sql .= ' `' . $v . '` ';
                    $obCount++;
                }
                $sql .= $order;
            } else {
                $sql .= ' ORDER BY `' . $orderBy . '` ' . $order;
            }
        }

        /** @noinspection PhpUndefinedMethodInspection */
        return $obj->fetchArray($sql, $values);
    }

    /**
     * @param $prefix
     * @param $column
     * @param int $subStrSize
     * @return array of single column values
     */
    public static function findBucketsByPrefix($prefix, $column, $subStrSize = 4)
    {
        $obj = new static;

        /** @noinspection PhpUndefinedFieldInspection */
        $sql = 'SELECT DISTINCT SUBSTR(`' . $column . "`, 1, {$subStrSize}) " .
            'FROM ' . $obj->table . ' ' .
            'WHERE UPPER(`' . $column . "`) LIKE '{$prefix}%'";

        $results = array();

        $dbh = self::getConnection();
        $sth = $dbh->prepare($sql);
        $sth->execute(array());

        while ($row = $sth->fetch(PDO::FETCH_NUM)) {
            $results[] = $row[0];
        }

        return $results;
    }

    /**
     * @param $prefix
     * @param string $column
     * @return mixed
     */
    public static function findByPrefix($prefix, $column = 'name')
    {
        $obj = new static;
        $prefix = addslashes(strtoupper($prefix));

        /** @noinspection PhpUndefinedFieldInspection */
        $sql = 'SELECT ' . static::getColumnsString() . ' ' .
            "FROM {$obj->table} " .
            "WHERE UPPER(`{$column}`) LIKE '{$prefix}%'";

        /** @noinspection PhpUndefinedMethodInspection */
        return $obj->fetchArray($sql, array('prefix' => $prefix.'%'));
    }

    /**
     * Insert current object
     */
    public function insert()
    {
        $properties = static::getProperties();

        $primary = $this->primaryKey;

        $sql = 'INSERT INTO ' . $this->table . ' (__columns__) values(__values__)';

        $sets = array();

        foreach ($properties as $prop) {
            $name = $prop->name;
            if ($name != 'table' && $name != 'primaryKey' && $name != $this->primaryKey) {
                $sets[] = '?';
            }
        }

        $sql = str_replace('__columns__', static::getColumnsString(false), $sql);
        $sql = str_replace('__values__', join(',', $sets), $sql);

        $dbh = $this->getConnection();
        $sth = null;

        try {
            $sth = $dbh->prepare($sql);
            $sth->execute($this->getValuesArray());
        } catch (PDOException $e) {
            error_log("SQL ERROR: {$e->getMessage()}");
            error_log("SQL: $sql");
            error_log("VALUES: {$this->getValuesArray()}");
        }

        $this->$primary = $dbh->lastInsertId();

        $error = $sth->errorInfo();
        return ($error[0] === "00000");
    }

    /**
     * Update all fields for current object
     */
    public function update()
    {
        $properties = static::getProperties();

        $sql = 'UPDATE ' . $this->table . ' SET ';

        $sets = array();

        foreach ($properties as $prop) {
            $name = $prop->name;
            if ($name != 'table' && $name != 'primaryKey' && $name != $this->primaryKey) {
                $sets[] = $prop->name . ' = ?';
            }
        }

        $sql .= join(',', $sets);
        $sql .= ' WHERE ' . $this->primaryKey . ' = ?';

        $dbh = $this->getConnection();
        $sth = null;
        try {
            $sth = $dbh->prepare($sql);
            $sth->execute($this->getValuesArray(true));
        } catch (PDOException $e) {
            error_log("SQL ERROR: {$e->getMessage()}");
            error_log("SQL: $sql");
            error_log("VALUES: {$this->getValuesArray()}");
        }

        $error = $sth->errorInfo();
        return ($error[0] === "00000");
    }

    /**
     * Delete current record
     */
    public function delete()
    {
        $sql = 'DELETE FROM ' . $this->table . ' WHERE ' . $this->primaryKey . ' = ?';
        $dbh = $this->getConnection();
        $sth = $dbh->prepare($sql);
        $sth->execute(array($this->getPrimaryKeyValue()));

        $error = $sth->errorInfo();
        return ($error[0] === "00000");
    }

    /**
     * Return just the value of the primary key
     * @return mixed
     */
    public function getPrimaryKeyValue() {
        return array_pop($this->getValuesArray(true));
    }

    public function getArray() {
        $array = (array) $this;
        unset($array['table']);
        unset($array['primaryKey']);

        return $array;
    }

    /**
     * Return comma-delimited columns
     * @param bool $withPrimaryKey
     * @return string
     */
    protected static function getColumnsString($withPrimaryKey = true) {
        $properties = static::getProperties();
        $columns = array();

        foreach ($properties as $prop) {
            $name = $prop->name;
            if ($name != 'table' && $name != 'primaryKey') {
                $columns[] = '`' . $prop->name . '`';
            }
        }
        if (!$withPrimaryKey) {
            array_shift($columns);
        }
        return join(',', $columns);
    }

    /**
     * Get array representation of this object's properties
     * @param bool $withId
     * @return array
     */
    protected function getValuesArray($withId = false)
    {
        $values = array();

        $properties = static::getProperties();

        $primary = null;

        /** @var $prop ReflectionProperty */
        foreach ($properties as $prop) {
            $name = $prop->name;
            if ($name != 'table' && $name != 'primaryKey') {
                if ($name == $this->primaryKey) {
                    $primary = $prop->getValue($this);
                } else {
                    $values[] = $prop->getValue($this);
                }
            }
        }

        if ($withId) {
            $values[] = $primary;
        }

        return $values;
    }

    /**
     * Map returned rows to instance of self
     * @param $row
     * @return BaseDataAccess
     */
    protected static function mapProperties($row)
    {
        $obj = new static;

        foreach ($row as $key => $value) {
            $obj->$key = $value;
        }

        return $obj;
    }

    /**
     * Return list of ReflectionProperties
     * @return \ReflectionProperty[]
     */
    public static function getProperties() {
        $obj = new static;
        $reflector = new ReflectionClass($obj);
        return $reflector->getProperties();
    }

    /**
     * Return single result
     * @param string $sql The parameterized query to execute
     * @param array $values The query parameters
     * @return BaseDataAccess|null
     */
    protected static function fetchSingle($sql, array $values) {
        $result = null;

        $dbh = self::getConnection();
        $sth = $dbh->prepare($sql);
        $sth->execute($values);

        while ($row = $sth->fetch(PDO::FETCH_ASSOC)) {
            $result = self::mapProperties($row);
            break;
        }

        return $result;
    }

    /**
     * Return array of results
     * @param string $sql The parameterized query to execute
     * @param array $values The query parameters
     * @return array An array of the current class
     */
    protected static function fetchArray($sql, array $values) {
        $results = array();

        $dbh = self::getConnection();
        $sth = $dbh->prepare($sql);
        $sth->execute($values);

        while ($row = $sth->fetch(PDO::FETCH_ASSOC)) {
            $results[] = self::mapProperties($row);
        }

        return $results;
    }
}
