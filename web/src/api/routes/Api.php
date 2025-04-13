<?php

/**
 * API Router Class
 *
 * Handles HTTP requests, routes them to the appropriate controller,
 * and manages responses.
 *
 * @author Yanis Deplazes
 * @date 10.03.2025
 */

namespace Api\Routes;

use Api\Controllers\Controller;
use Api\Views\View;

/**
 * API Router Class
 * Handles routing for both standard database tables and views.
 */
class Api
{
    /** @var string The requested HTTP method. */
    private string $requestMethod;

    /** @var string|null The requested resource (table or view name). */
    private ?string $resource;

    /** @var string|null The optional record ID for requests. */
    private ?string $id;

    /** @var array The request path as an array. */
    private array $pathParts;

    /**
     * Initializes API routing, sets headers, and extracts request parameters.
     */
    public function __construct()
    {
        $this->setHeaders();
        $this->requestMethod = $_SERVER["REQUEST_METHOD"];
        $this->handleCors();
        $this->parseRequestUri();
    }

    /**
     * Sets the response headers.
     */
    private function setHeaders(): void
    {
        header("Content-Type: application/json");
    }

    /**
     * Handles CORS headers for cross-origin requests.
     */
    private function handleCors(): void
    {
        if ($_SERVER["HTTP_HOST"] === "localhost") {
            header("Access-Control-Allow-Origin: http://localhost:5173");
            header("Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS");
            header("Access-Control-Allow-Headers: Content-Type, Authorization");
        }

        if ($this->requestMethod === "OPTIONS") {
            http_response_code(200);
            exit;
        }
    }

    /**
     * Parses the request URI to determine resource and ID.
     */
    private function parseRequestUri(): void
    {
        $path = trim(strtok($_SERVER["REQUEST_URI"], '?'), '/');
        $this->pathParts = explode('/', $path);
        $this->resource = $this->pathParts[1] ?? null;
        $this->id = $this->pathParts[2] ?? null;
    }

    /**
     * Runs the API and routes the request to the appropriate handler.
     */
    public function run(): void
    {
        if (!$this->resource) {
            $this->sendResponse(["error" => "No resource specified"], 400);
            return;
        }

        if (str_starts_with($this->resource, "views")) {
            $this->handleViewRequest();
            return;
        }

        if ($this->id !== null && !ctype_digit($this->id)) {
            $this->sendResponse(["error" => "Invalid ID format"], 400);
            return;
        }

        if ($this->resource === "reading-with-sensordata" && $this->requestMethod === "POST") {
            $this->handleReadingWithSensorData();
            return;
        }

        $this->handleTableRequest();
    }

    /**
     * Handles API requests for database views.
     */
    private function handleViewRequest(): void
    {
        if (!isset($this->pathParts[2]) || empty($this->pathParts[2])) {
            $this->sendResponse(["error" => "View name is required"], 400);
            return;
        }

        $viewName = $this->pathParts[2];
        $deviceId = $_GET["device_id"] ?? null;
        $viewModel = new View();

        $result = ($deviceId !== null && ctype_digit($deviceId))
            ? $viewModel->getFilteredFromView($viewName, (int) $deviceId)
            : $viewModel->getAllFromView($viewName);

        $this->sendResponse($result);
    }

    /**
     * Handles API requests for database tables.
     */
    private function handleTableRequest(): void
    {
        $controller = new Controller($this->resource);

        switch ($this->requestMethod) {
            case "GET":
                $this->id ? $controller->getById((int) $this->id) : $controller->getAll();
                break;
            case "POST":
                $controller->create();
                break;
            case "PUT":
                $this->validateIdBeforeModification();
                $controller->update((int) $this->id);
                break;
            case "DELETE":
                $this->validateIdBeforeModification();
                $controller->delete((int) $this->id);
                break;
            default:
                $this->sendResponse(["error" => "Method not allowed"], 405);
        }
    }

    /**
     * Validates if ID is present before update or delete operations.
     */
    private function validateIdBeforeModification(): void
    {
        if (!$this->id) {
            $this->sendResponse(["error" => "Missing ID for operation"], 400);
            exit;
        }
    }



    /**
     * Sends a JSON response with an HTTP status code.
     *
     * @param array $data Response data.
     * @param int $status HTTP status code.
     */
    private function sendResponse(array $data, int $status = 200): void
    {
        http_response_code($status);
        echo json_encode($data);
        exit;
    }

    /**
     * Handles a combined insert of reading and sensor data in one POST request.
     * Expects JSON body with: { device_id: int, sensor_data: [ {sensor_id, value}, ... ] }
     */
    private function handleReadingWithSensorData(): void
    {
        $payload = json_decode(file_get_contents("php://input"), true);

        if (
            !isset($payload["device_id"]) ||
            !is_int($payload["device_id"]) ||
            !isset($payload["sensor_data"]) ||
            !is_array($payload["sensor_data"])
        ) {
            $this->sendResponse(["error" => "Invalid or missing payload fields"], 400);
            return;
        }

        $controller = new \Api\Controllers\ReadingWithSensorDataController();
        $result = $controller->createWithSensorData($payload);

        $this->sendResponse($result, $result["status"] ?? 200);
    }
}
