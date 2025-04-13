<?php

/**
 * API Entry Point
 *
 * Bootstraps the application, initializes routing, and runs the API.
 *
 * This file loads the necessary dependencies using Composer autoload,
 * initializes the API router, and executes the request handling process.
 *
 * @author Yanis Deplazes
 * @date 10.03.2025
 */

require_once __DIR__ . "/vendor/autoload.php";

use Api\Routes\Api;

$api = new Api();
$api->run();
