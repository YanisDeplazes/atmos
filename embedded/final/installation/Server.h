// ============================================================================
// File: Server.h
// Author: Yanis Deplazes
// License: MIT License
// Copyright (c) 2025 Yanis Deplazes
// Description: Declares the HTTPS server setup with TLS and a basic /index
//              route for secure status verification.
// ============================================================================

#pragma once
#include "esp_http_server.h"

// ----------------------------------------------------------------------------
// Constants & Macros (shared with server.cpp to avoid magic numbers)
// ----------------------------------------------------------------------------
#define TAG_HTTPS_SERVER "HTTPS_SERVER"
#define STATUS_ROUTE "/index"
#define MAX_RESPONSE_LEN 32

// ----------------------------------------------------------------------------
// HTTPS Server Function Declarations
// ----------------------------------------------------------------------------

/**
 * Starts the ESP32 HTTPS server using httpd_ssl with a self-signed certificate.
 */
void start_https_server();

// ----------------------------------------------------------------------------
// Shared State
// ----------------------------------------------------------------------------

extern int currentIndex;
