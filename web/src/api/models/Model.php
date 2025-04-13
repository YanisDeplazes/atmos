<?php

/**
 * Generic Model Class
 *
 * Implements database interactions using prepared statements
 * and provides a structured CRUD API.
 *
 * @author Yanis Deplazes
 * @date 10.03.2025
 */

namespace Api\Models;

use Api\Models\Database;
use mysqli;

/**
 * Generic Model Class
 * Provides CRUD operations for any table dynamically.
 */
class Model
{
    /** @var mysqli Database connection instance. */
    protected mysqli $connection;

    /** @var string Database table name. */
    protected string $table;

    /**
     * Initializes the model with a specific database table.
     *
     * @param string $table The name of the database table.
     */
    public function __construct(string $table)
    {
        $this->connection = Database::getInstance()->getConnection();
        $this->table = $table;
    }

    /**
     * Retrieves all records from the table.
     *
     * @return array Returns an associative array of all records.
     */
    public function getAll(): array
    {
        return $this->executeQuery("SELECT * FROM `{$this->table}`");
    }

    /**
     * Retrieves a single record by its ID.
     *
     * @param int $id The ID of the record.
     * @return array|null Returns the record as an associative array, or null if not found.
     */
    public function getById(int $id): ?array
    {
        return $this->fetchSingle("SELECT * FROM `{$this->table}` WHERE id = ?", "i", [$id]);
    }

    /**
     * Inserts a new record into the table.
     *
     * @param array $data An associative array representing the record.
     * @return bool Returns true if the insert operation was successful.
     */
    public function create(array $data): ?int
    {
        if (empty($data)) {
            return null;
        }

        $columns = implode(", ", array_map(fn($col) => "`$col`", array_keys($data)));
        $placeholders = implode(", ", array_fill(0, count($data), "?"));
        $types = str_repeat("s", count($data));
        $values = array_values($data);

        $query = "INSERT INTO `{$this->table}` ($columns) VALUES ($placeholders)";

        try {
            $stmt = $this->connection->prepare($query);
            $stmt->bind_param($types, ...$values);
            $stmt->execute();

            return $stmt->insert_id ?: null;
        } catch (\mysqli_sql_exception $e) {
            error_log("SQL Error: " . $e->getMessage());
            return null;
        }
    }

    /**
     * Inserts multiple records into the table efficiently.
     *
     * @param array $dataSet Array of records to insert.
     * @return bool Returns true if the operation was successful.
     */
    public function createBulk(array $dataSet): bool
    {
        if (empty($dataSet)) {
            return false;
        }

        // Get column names from the first entry
        $columns = array_keys($dataSet[0]);
        $columnsList = implode(", ", array_map(fn($col) => "`$col`", $columns));

        // Generate placeholders for prepared statement (e.g., (?, ?, ?), (?, ?, ?), ...)
        $rows = count($dataSet);
        $cols = count($columns);
        $singleRow = "(" . implode(", ", array_fill(0, $cols, "?")) . ")";
        $placeholders = implode(", ", array_fill(0, $rows, $singleRow));

        // Flatten the array for binding
        $values = [];
        foreach ($dataSet as $data) {
            foreach ($columns as $col) {
                $values[] = $data[$col];
            }
        }

        // Determine types dynamically (assuming all are strings for simplicity)
        $types = str_repeat("s", count($values));

        $query = "INSERT INTO `{$this->table}` ($columnsList) VALUES $placeholders";

        return $this->execute($query, $types, $values);
    }



    /**
     * Updates an existing record in the database.
     *
     * @param int $id The ID of the record to update.
     * @param array $data An associative array of column-value pairs to update.
     * @return bool Returns true if the update was successful.
     */
    public function update(int $id, array $data): bool
    {
        return $this->executeInsertOrUpdate("UPDATE `{$this->table}` SET", $data, " WHERE id = $id");
    }

    /**
     * Deletes a record from the database.
     *
     * @param int $id The ID of the record to delete.
     * @return bool Returns true if the deletion was successful.
     */
    public function delete(int $id): bool
    {
        return $this->execute("DELETE FROM `{$this->table}` WHERE id = ?", "i", [$id]);
    }

    /**
     * Executes a query and returns the result set as an array.
     *
     * @param string $query The SQL query.
     * @return array The query result as an associative array.
     */
    private function executeQuery(string $query): array
    {
        try {
            $result = $this->connection->query($query);
            return $result ? $result->fetch_all(MYSQLI_ASSOC) : [];
        } catch (\Exception $e) {
            error_log("SQL Error: " . $e->getMessage());
            return ["error" => $e->getMessage()];
        }
    }

    /**
     * Executes a prepared query and returns true on success.
     *
     * @param string $query The SQL query with placeholders.
     * @param string $types The types of parameters.
     * @param array $params The parameters to bind.
     * @return bool Returns true if successful, false otherwise.
     */
    private function execute(string $query, string $types, array $params): bool
    {
        try {
            $stmt = $this->connection->prepare($query);
            $stmt->bind_param($types, ...$params);
            return $stmt->execute();
        } catch (\mysqli_sql_exception $e) {
            error_log("SQL Error: " . $e->getMessage());
            return false;
        }
    }

    /**
     * Executes a prepared query and fetches a single record.
     *
     * @param string $query The SQL query with placeholders.
     * @param string $types The types of parameters.
     * @param array $params The parameters to bind.
     * @return array|null Returns the fetched record as an associative array or null.
     */
    private function fetchSingle(string $query, string $types, array $params): ?array
    {
        try {
            $stmt = $this->connection->prepare($query);
            $stmt->bind_param($types, ...$params);
            $stmt->execute();
            $result = $stmt->get_result();
            return $result ? $result->fetch_assoc() : null;
        } catch (\mysqli_sql_exception $e) {
            error_log("SQL Error: " . $e->getMessage());
            return null;
        }
    }

    /**
     * Executes an insert or update query dynamically.
     *
     * @param string $queryPrefix The query prefix (INSERT INTO or UPDATE ... SET).
     * @param array $data The data to insert or update.
     * @param string $querySuffix Optional suffix (used for WHERE conditions in updates).
     * @return bool Returns true if the operation was successful.
     */
    private function executeInsertOrUpdate(string $queryPrefix, array $data, string $querySuffix = ""): bool
    {
        if (empty($data)) {
            return false;
        }

        $columns = implode(", ", array_map(fn($col) => "`$col`", array_keys($data)));
        $placeholders = implode(", ", array_fill(0, count($data), "?"));
        $types = str_repeat("s", count($data));
        $values = array_values($data);

        if (str_starts_with($queryPrefix, "UPDATE")) {
            $setClause = implode(", ", array_map(fn($col) => "`$col` = ?", array_keys($data)));
            $query = "$queryPrefix $setClause $querySuffix";
        } else {
            $query = "$queryPrefix ($columns) VALUES ($placeholders) $querySuffix";
        }

        return $this->execute($query, $types, $values);
    }
}
