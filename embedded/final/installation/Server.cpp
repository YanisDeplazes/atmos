// ============================================================================
// File: Server.cpp
// Author: Yanis Deplazes
// License: MIT License
// Copyright (c) 2025 Yanis Deplazes
// Description: Sets up an HTTPS server with a /index route and WebSocket over
//              TLS support for bi-directional communication.
// ============================================================================

#include "server.h"
#include "esp_https_server.h"
#include "esp_log.h"
#include "esp_tls.h"
#include "cert.h"
#include "key.h"
#include "Client.h"  // To access currentIndex
#include <string.h>
#include <stdlib.h>

// ----------------------------------------------------------------------------
// Constants & Macros
// ----------------------------------------------------------------------------
#define TAG_NAME "HTTPS_SERVER"
#define ROUTE_INDEX_URI "/index"
#define ROUTE_INDEX_JSON_FMT "{\"index\": %d}"
#define JSON_BUFFER_SIZE 32
#define CERT_STRLEN_PADDING 1

// ----------------------------------------------------------------------------
// State
// ----------------------------------------------------------------------------
static const char *TAG = TAG_NAME;
static httpd_handle_t server = NULL;
static int ws_client_fd = -1;  // Track connected WebSocket client

// ----------------------------------------------------------------------------
// Route Handlers
// ----------------------------------------------------------------------------

/**
 * handle_index()
 * --------------
 * Basic GET endpoint for /index to verify server health.
 */
esp_err_t handle_index(httpd_req_t *req) {
  char resp_str[JSON_BUFFER_SIZE];
  snprintf(resp_str, sizeof(resp_str), ROUTE_INDEX_JSON_FMT, currentIndex);

  httpd_resp_set_type(req, "application/json");
  return httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);
}

// ----------------------------------------------------------------------------
// HTTPS Server Setup
// ----------------------------------------------------------------------------

/**
 * start_https_server()
 * --------------------
 * Initializes and starts the HTTPS server with certificate/key from memory.
 * Registers REST and WebSocket endpoints.
 */
void start_https_server() {
  httpd_ssl_config_t conf = HTTPD_SSL_CONFIG_DEFAULT();
  conf.servercert = (const uint8_t *)server_cert;
  conf.servercert_len = strlen(server_cert) + CERT_STRLEN_PADDING;
  conf.prvtkey_pem = (const uint8_t *)server_key;
  conf.prvtkey_len = strlen(server_key) + CERT_STRLEN_PADDING;

  esp_err_t ret = httpd_ssl_start(&server, &conf);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to start HTTPS server: %s", esp_err_to_name(ret));
    return;
  }

  httpd_uri_t index_uri = {
    .uri = ROUTE_INDEX_URI,
    .method = HTTP_GET,
    .handler = handle_index,
    .user_ctx = NULL
  };

  httpd_register_uri_handler(server, &index_uri);
  ESP_LOGI(TAG, "HTTPS server started");
}
