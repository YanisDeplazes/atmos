<?php

/**
 * Database Connection Class
 *
 * Implements a singleton pattern for database connection management.
 *
 * @author Yanis Deplazes
 * @date 10.03.2025
 */

namespace Api\Models;

use mysqli;
use mysqli_sql_exception;

/**
 * Singleton Database Class
 * Handles database connections using a singleton pattern.
 */
class Database
{
    /** @var Database|null Holds the single instance of the Database class. */
    private static ?Database $instance = null;

    /** @var mysqli The MySQLi database connection instance. */
    private mysqli $connection;

    /**
     * Private constructor to prevent direct instantiation.
     */
    private function __construct()
    {
        $this->initializeConnection();
    }

    /**
     * Retrieves the singleton instance of the Database class.
     *
     * @return Database The singleton instance.
     */
    public static function getInstance(): Database
    {
        if (self::$instance === null) {
            self::$instance = new self();
        }
        return self::$instance;
    }

    /**
     * Gets the MySQLi connection instance.
     *
     * @return mysqli The active database connection.
     */
    public function getConnection(): mysqli
    {
        return $this->connection;
    }

    /**
     * Initializes the database connection.
     */
    private function initializeConnection(): void
    {
        $host = getenv("DB_HOST") ?: "mariadb";
        $username = getenv("DB_USER") ?: "root";
        $password = getenv("DB_PASS") ?: "";
        $dbname = getenv("DB_NAME") ?: "atmos";

        mysqli_report(MYSQLI_REPORT_ERROR | MYSQLI_REPORT_STRICT);

        try {
            $this->connection = new mysqli($host, $username, $password, $dbname);
            $this->connection->set_charset("utf8mb4");
        } catch (mysqli_sql_exception $e) {
            $this->sendJsonErrorResponse($e->getMessage());
        }
    }

    /**
     * Sends a JSON error response and terminates execution.
     *
     * @param string $errorMessage The error message.
     */
    private function sendJsonErrorResponse(string $errorMessage): void
    {
        header("Content-Type: application/json");
        echo json_encode(["success" => false, "error" => $errorMessage]);
        exit;
    }
}
