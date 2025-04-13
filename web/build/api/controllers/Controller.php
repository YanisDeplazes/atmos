<?php

/**
 * Controller Class
 *
 * Provides CRUD operations for API endpoints by interacting with
 * database models.
 *
 * @author Yanis Deplazes
 * @date 10.03.2025
 */

namespace Api\Controllers;

use Api\Models\Model;

/**
 * Generic Controller Class
 * Handles CRUD operations for any table dynamically.
 */
class Controller
{
    /** @var Model Model instance. */
    protected Model $model;

    /**
     * Initializes the controller with a specific model.
     *
     * @param string $table The name of the database table.
     */
    public function __construct(string $table)
    {
        $this->model = new Model($table);
    }

    /**
     * Returns all records as JSON.
     */
    public function getAll(): void
    {
        $this->sendJsonResponse($this->model->getAll());
    }

    /**
     * Returns a single record by ID as JSON.
     *
     * @param int $id The ID of the record.
     */
    public function getById(int $id): void
    {
        $data = $this->model->getById($id);
        $data ? $this->sendJsonResponse($data) : $this->sendJsonResponse(["error" => "Not found"], 404);
    }


    /**
     * Creates a new record (supports single and bulk inserts).
     */
    public function create(): void
    {
        $data = $this->getJsonRequestBody();

        // Check if we received an array of multiple records
        $result = is_array($data) && isset($data[0]) && is_array($data[0])
            ? $this->model->createBulk($data)  // Bulk insert
            : $this->model->create($data);     // Single insert

        if ($result) {
            $this->sendJsonResponse(["message" => "Created successfully"], 201);
        } else {
            $this->sendJsonResponse(["error" => "Creation failed"], 500);
        }
    }


    /**
     * Updates an existing record.
     *
     * @param int $id The ID of the record.
     */
    public function update(int $id): void
    {
        $data = $this->getJsonRequestBody();
        $this->model->update($id, $data)
            ? $this->sendJsonResponse(["message" => "Updated successfully"])
            : $this->sendJsonResponse(["error" => "Update failed"], 500);
    }

    /**
     * Deletes a record.
     *
     * @param int $id The ID of the record.
     */
    public function delete(int $id): void
    {
        $this->model->delete($id)
            ? $this->sendJsonResponse(["message" => "Deleted successfully"])
            : $this->sendJsonResponse(["error" => "Deletion failed"], 500);
    }

    /**
     * Sends a JSON response with the specified status code.
     *
     * @param array $data Response data.
     * @param int $status HTTP status code.
     */
    private function sendJsonResponse(array $data, int $status = 200): void
    {
        http_response_code($status);
        echo json_encode($data);
        exit;
    }

    /**
     * Retrieves the JSON request body as an associative array.
     *
     * @return array The decoded JSON body.
     */
    private function getJsonRequestBody(): array
    {
        $data = json_decode(file_get_contents("php://input"), true);
        return $data ?: $this->sendJsonResponse(["error" => "Invalid JSON"], 400);
    }
}
