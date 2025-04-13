<?php

/**
 * View Class
 *
 * Handles retrieval of data from database views and provides
 * structured data responses.
 *
 * @author Yanis Deplazes
 * @date 10.03.2025
 */

namespace Api\Views;

use Api\Models\Database;

/**
 * Class View
 * Handles retrieval of data from database views.
 */
class View
{
    /** @var \mysqli Database connection instance. */
    private \mysqli $connection;

    /**
     * Initializes database connection.
     */
    public function __construct()
    {
        $this->connection = Database::getInstance()->getConnection();
    }

    /**
     * Fetch all records from a given database view.
     *
     * @param string $viewName The name of the view.
     * @return array The result set as an associative array.
     */
    public function getAllFromView(string $viewName): array
    {
        return $this->fetchResults($viewName);
    }

    /**
     * Fetch records from a view with an optional device ID filter.
     *
     * @param string $viewName The name of the view.
     * @param int|null $deviceId Optional device ID for filtering.
     * @return array The filtered result set.
     */
    public function getFilteredFromView(string $viewName, ?int $deviceId): array
    {
        return $this->fetchResults($viewName, $deviceId);
    }

    /**
     * Fetch data from a database view with optional filtering.
     *
     * @param string $viewName The view name.
     * @param int|null $deviceId Optional filter by device ID.
     * @return array The query result.
     */
    private function fetchResults(string $viewName, ?int $deviceId = null): array
    {
        $safeViewName = $this->sanitizeViewName($viewName);
        $query = "SELECT * FROM `" . $safeViewName . "`";

        if ($deviceId !== null) {
            $query .= " WHERE device_id = ?";
            $stmt = $this->connection->prepare($query);
            $stmt->bind_param("i", $deviceId);
            $stmt->execute();
            $result = $stmt->get_result();
        } else {
            $result = $this->connection->query($query);
        }

        return $result ? $result->fetch_all(MYSQLI_ASSOC) :
        ["error" => "Query failed or view '$safeViewName' does not exist."];
    }

    /**
     * Sanitize view names to prevent SQL injection.
     *
     * @param string $viewName The raw view name.
     * @return string The sanitized view name.
     */
    private function sanitizeViewName(string $viewName): string
    {
        $allowedViews = ["LatestDeviceReadings"];
        return in_array($viewName, $allowedViews) ? $viewName : "Invalid_View_Name";
    }
}
