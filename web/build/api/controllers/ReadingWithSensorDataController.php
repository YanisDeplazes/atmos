<?php

/**
 * ReadingWithSensorDataController Class
 *
 * Handles combined creation of a reading and its associated sensor data
 * in a single atomic operation.
 *
 * @author Yanis Deplazes
 * @date 10.03.2025
 */

namespace Api\Controllers;

use Api\Models\Model;
use Api\Models\Database; // ⬅️ import the DB singleton
use mysqli;
use mysqli_sql_exception;

/**
 * Reading With Sensor Data Controller Class
 * Handles combined creation of a reading and its associated sensor data
 * in a single atomic operation.
 */
class ReadingWithSensorDataController
{
    /** @var mysqli MySQLi database connection */
    private mysqli $db;

    /** @var Model */
    private Model $readingModel;

    /** @var Model */
    private Model $sensorDataModel;

    /**
     * Initializes the controller with a specific database models.
     */
    public function __construct()
    {
        $this->db = Database::getInstance()->getConnection();
        $this->readingModel = new Model("reading");
        $this->sensorDataModel = new Model("sensordata");
    }

    /**
     * Creates a reading and inserts all associated sensor data in one transaction.
     *
     * @param array $payload Input data from the client.
     * @return array API response.
     */
    public function createWithSensorData(array $payload): array
    {
        try {
            $this->db->begin_transaction();

            // Step 1: Insert into reading
            $readingInsert = ["device_id" => $payload["device_id"]];
            $readingId = $this->readingModel->create($readingInsert);

            if (!$readingId) {
                $this->db->rollback();
                return ["error" => "Failed to create reading", "status" => 500];
            }

            // Step 2: Prepare sensor data with reading_id
            $sensorData = array_map(function ($entry) use ($readingId) {
                return [
                    "reading_id" => $readingId,
                    "sensor_id" => $entry["sensor_id"],
                    "value"      => $entry["value"]
                ];
            }, $payload["sensor_data"]);

            // Step 3: Bulk insert
            $success = $this->sensorDataModel->createBulk($sensorData);

            if (!$success) {
                $this->db->rollBack();
                return ["error" => "Failed to insert sensor data", "status" => 500];
            }

            $this->db->commit();

            return [
                "message"     => "Created successfully",
            ];
        } catch (mysqli_sql_exception $e) {
            $this->db->rollBack();
            return ["error" => "Database error: " . $e->getMessage(), "status" => 500];
        }
    }
}
