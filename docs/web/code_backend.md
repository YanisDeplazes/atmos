# Code: Backend – PHP API

## Disclaimer

In this part, ChatGPT was used to generate code comments such as class, method and function headers, which helped to maintain a standard and write correct code comments. It is a reflection of the code comments and structure of the code.

## Api Class (`Api/Routes/Api.php`)

**Main API Router**  
Handles all incoming HTTP requests, manages CORS, parses URIs, and dispatches to controller or view handlers.

### Properties

| Name            | Type       | Visibility | Description                                     |
| --------------- | ---------- | ---------- | ----------------------------------------------- |
| `requestMethod` | `string`   | private    | The HTTP method used in the current request.    |
| `resource`      | `?string`  | private    | The resource name (table or view) from the URI. |
| `id`            | `?string`  | private    | The optional ID parameter from the URI.         |
| `pathParts`     | `string[]` | private    | Array of URI path segments.                     |

### Methods

#### `__construct()`

Initializes API routing, sets headers, and extracts request parameters.

- **Visibility:** Public

#### `setHeaders()`

Sets the response headers.

- **Visibility:** Private

#### `handleCors()`

Handles CORS headers for cross-origin requests.

- **Visibility:** Private

#### `parseRequestUri()`

Parses the request URI to determine resource and ID.

- **Visibility:** Private

#### `run()`

Runs the API and routes the request to the appropriate handler.

- **Visibility:** Public

#### `handleViewRequest()`

Handles API requests for database views.

- **Visibility:** Private

#### `handleTableRequest()`

Handles API requests for database tables.

- **Visibility:** Private

#### `validateIdBeforeModification()`

Validates if ID is present before update or delete operations.

- **Visibility:** Private

#### `sendResponse(data, status = 200)`

Sends a JSON response with an HTTP status code.

- **Visibility:** Private
- **Parameter:** `data`: `array` Response data.
- **Parameter:** `status`: `int` HTTP status code.

#### `handleReadingWithSensorData()`

Handles a combined insert of reading and sensor data in one POST request.
Expects JSON body with: { device_id: int, sensor_data: [ {sensor_id, value}, ... ] }

- **Visibility:** Private

## Controller Class (`Api/Controllers/Controller.php`)

**Generic Table Controller**  
Handles CRUD operations for any table dynamically.

### Properties

| Name    | Type    | Visibility | Description                      |
| ------- | ------- | ---------- | -------------------------------- |
| `model` | `Model` | protected  | Model instance bound to a table. |

### Methods

#### `__construct(table)`

Initializes the controller with a specific model.

- **Visibility:** Public
- **Parameter:** `table`: `string` The name of the database table.

#### `getAll()`

Returns all records as JSON.

- **Visibility:** Public

#### `getById(id)`

Returns a single record by ID as JSON.

- **Visibility:** Public
- **Parameter:** `id`: `int` — Record ID

#### `create()`

Creates a new record (supports single and bulk inserts).

- **Visibility:** Public

#### `update(id)`

Updates an existing record.

- **Visibility:** Public
- **Parameter:** `id`: `int` The ID of the record.

#### `delete(id)`

Deletes a record.

- **Visibility:** Public
- **Parameter:** `id`: `int` The ID of the record.

#### `sendJsonResponse(data, status = 200)`

Sends a JSON response with the specified status code.

- **Visibility:** Private
- **Parameter:** `data`: `array` Response data.
- **Parameter:** `status`: `int` HTTP status code.

#### `getJsonRequestBody()`

Retrieves the JSON request body as an associative array.

- **Visibility:** Private
- **Return Type:** `array` The decoded JSON body.

## ReadingWithSensorDataController Class (`Api/Controllers/ReadingWithSensorDataController.php`)

**Custom Controller**  
Handles atomic insert of a reading and associated sensor data in one transaction.

### Properties

| Name              | Type     | Visibility | Description                       |
| ----------------- | -------- | ---------- | --------------------------------- |
| `db`              | `mysqli` | private    | Database connection instance.     |
| `readingModel`    | `Model`  | private    | Model for the `reading` table.    |
| `sensorDataModel` | `Model`  | private    | Model for the `sensordata` table. |

### Methods

#### `__construct()`

Initializes the controller with a specific database models.

- **Visibility:** Public

#### `createWithSensorData(payload)`

Creates a reading and inserts all associated sensor data in one transaction.

- **Visibility:** Public
- **Parameter:** `payload`: `array` Input data from the client.
- **Return Type:** `array` API response.

## Database Class (`Api/Models/Database.php`)

**Database Singleton**  
Manages a single MySQLi connection instance across the backend.

### Properties

| Name         | Type        | Visibility | Description                      |
| ------------ | ----------- | ---------- | -------------------------------- |
| `instance`   | `?Database` | private    | Singleton instance of the class. |
| `connection` | `mysqli`    | private    | Active MySQLi connection.        |

### Methods

#### `getInstance()`

Retrieves the singleton instance of the Database class.

- **Visibility:** Public
- **Return Type:** `Database` The singleton instance.

#### `getConnection()`

Gets the MySQLi connection instance.

- **Visibility:** Public
- **Return Type:** `mysqli` The active database connection.

#### `initializeConnection()`

Initializes the database connection.

- **Visibility:** Private

#### `sendJsonErrorResponse(errorMessage)`

Sends a JSON error response and terminates execution.

- **Visibility:** Private
- **Parameter:** `errorMessage`: `string` The error message.

## Model Class (`Api/Models/Model.php`)

**Generic Model**  
IProvides CRUD operations for any table dynamically

### Properties

| Name         | Type     | Visibility | Description                  |
| ------------ | -------- | ---------- | ---------------------------- |
| `connection` | `mysqli` | protected  | Database connection instance |
| `table`      | `string` | protected  | Database table name          |

### Methods

#### `__construct(table)`

Initializes the model with a specific database table.

- **Visibility:** Public
- **Parameter:** `table`: `string` The name of the database table.

#### `getAll()`

Retrieves all records from the table.

- **Visibility:** Public
- **Return Type:** `array` Returns an associative array of all records.

#### `getById(id)`

Retrieves a single record by its ID.

- **Visibility:** Public
- **Parameter:** `id`: `int` The ID of the record.
- **Return Type:** `?array` Returns the record as an associative array, or null if not found.

#### `create(data)`

Inserts a new record into the table.

- **Visibility:** Public
- **Parameter:** `data`: `array` An associative array representing the record.
- **Return Type:** `?int` Returns true if the insert operation was successful.

#### `createBulk(dataSet)`

Inserts multiple records into the table efficiently.

- **Visibility:** Public
- **Parameter:** `dataSet`: `array[]` Array of records to insert.
- **Return Type:** `bool` Returns true if the operation was successful.

#### `update(id, data)`

Updates an existing record in the database.

- **Visibility:** Public
- **Parameter:** `id`: `int` The ID of the record to update.
- **Parameter:** `data`: `array` An associative array of column-value pairs to update.
- **Return Type:** `bool`Returns true if the update was successful.

#### `delete(id)`

Deletes a record from the database.

- **Visibility:** Public
- **Parameter:** `id`: `int` The ID of the record to delete.
- **Return Type:** `bool` Returns true if the deletion was successful.

#### `executeQuery(query)`

Executes a prepared query and returns true on success.

- **Visibility:** Private
- **Parameter:** `query`: `string` The SQL query.
- **Return Type:** `array` The query result as an associative array.

#### `execute(query, types, params)`

Executes a prepared query and returns true on success.

- **Visibility:** Private
- **Parameter:** `query`: `string` The SQL query with placeholders.
- **Parameter:** `types`: `string` The types of parameters.
- **Parameter:** `params`: `array`The parameters to bind.
- **Return Type:** `bool` Returns true if successful, false otherwise.

#### `fetchSingle(query, types, params)`

Executes a prepared query and fetches a single record.

- **Visibility:** Private
- **Parameter:** `query`: `string` The SQL query with placeholders.
- **Parameter:** `types`: `string` The types of parameters.
- **Parameter:** `params`: `array`The parameters to bind.
- **Return Type:** `?array` Returns the fetched record as an associative array or null.

#### `executeInsertOrUpdate(queryPrefix, data, querySuffix = "")`

Executes an insert or update query dynamically.

- **Visibility:** Private
- **Parameter:** `queryPrefix`: `string` The query prefix (INSERT INTO or UPDATE ... SET).
- **Parameter:** `data`: `array` The data to insert or update.
- **Parameter:** `querySuffix`: `string` Optional suffix (used for WHERE conditions in updates).
- **Return Type:** `bool` Returns true if the operation was successful.
