
#include <WiFiManager.h>          // WiFiManager library for WiFi configuration via captive portal
#include <WiFiClientSecure.h>     // For secure HTTPS communication
#include <HTTPS_Server_Generic.h> // HTTPS server library for ESP32 HTTPS support
#include "cert.h"                 // Contains PEM-encoded SSL certificate (as const char[])
#include "key.h"                  // Contains PEM-encoded private key (as const char[])

using namespace httpsserver;

// ---------- MACRO DEFINITIONS (CONFIGURABLE CONSTANTS) ----------
#define WIFI_AP_NAME         "yanisdeplazes_IOT_Portal"  // SSID of fallback access point
#define WIFI_AP_PASSWORD     "superSecret123"            // WPA2 password for fallback access point
#define PORTAL_TIMEOUT_SEC   180                         // Max duration (in seconds) to keep config portal open
#define RESTART_DELAY_MS     3000                        // Delay before restart on WiFi failure
#define HTTPS_PORT           443                         // Default HTTPS port

// ---------- GLOBAL OBJECTS ----------
WiFiManager wifiManager;           // WiFiManager instance for handling WiFi setup
WiFiClientSecure secureClient;    // Secure client for HTTPS connections
SSLCert cert = SSLCert((unsigned char*)server_cert, 1472,
                       (unsigned char*)server_key, 1704); // SSL certificate object
HTTPSServer secureServer(&cert);   // HTTPS server bound to the certificate


/**
 * @brief Connects the ESP device to a WiFi network using WiFiManager.
 *        If no known WiFi is available, starts a configuration portal as fallback.
 *
 * @details
 * - Tries auto-connect to last known network
 * - Launches AP+Captive Portal if no credentials are stored or connection fails
 * - Reboots the device if portal times out without user input
 *
 * @return void
 */
void connectToWiFi() {
  // Set how long the configuration portal remains active (in seconds)
  wifiManager.setConfigPortalTimeout(PORTAL_TIMEOUT_SEC);

  // Attempt to auto-connect to previously saved credentials.
  // If not available, start an AP with specified SSID and password.
  if (!wifiManager.autoConnect(WIFI_AP_NAME, WIFI_AP_PASSWORD)) {
    // If portal times out or user exits without connecting, restart the device
    Serial.println("⚠️ Failed to connect or portal timed out. Restarting...");
    delay(RESTART_DELAY_MS);
    ESP.restart();
  }

  // WiFi connection successful
  Serial.println("✅ WiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

/**
 * @brief Handler for the root path ("/").
 * 
 * @param req Pointer to the HTTP request object
 * @param res Pointer to the HTTP response object
 * @return void
 */
void handleRoot(HTTPRequest *req, HTTPResponse *res) {
  res->setHeader("Content-Type", "text/plain");
  res->println("✅ Device is securely running over HTTPS");
}

/**
 * @brief Handler for POST requests to "/status".
 *
 * @details
 * - Reads raw body content from the request.
 * - Logs it to the serial monitor.
 * - Responds with a basic 200 OK message.
 *
 * @param req Pointer to the HTTP request object
 * @param res Pointer to the HTTP response object
 * @return void
 */
void handleStatus(HTTPRequest *req, HTTPResponse *res) {
  size_t len = req->getContentLength();
  String body;

  if (len > 0) {
    char buffer[len + 1]; // +1 for null-terminator
    size_t read = req->readChars(buffer, len);
    buffer[read] = '\0'; // Null-terminate
    body = String(buffer);
  }

  Serial.println("[HTTPS] Status received:");
  Serial.println(body);

  res->setHeader("Content-Type", "text/plain");
  res->println("Status OK");
}

/**
 * @brief Registers HTTPS endpoints and starts the server.
 *
 * @return void
 */
void setupHttpsServer() {
  secureServer.registerNode(new ResourceNode("/", "GET", &handleRoot));
  secureServer.registerNode(new ResourceNode("/status", "POST", &handleStatus));
  secureServer.start();
  Serial.println("✅ HTTPS server started on port 443");
}

/**
 * @brief Arduino setup function. Initializes serial, WiFi, and HTTPS server.
 *
 * @return void
 */
void setup() {
  Serial.begin(115200);
  connectToWiFi();
  setupHttpsServer();
}

/**
 * @brief Main loop. Continuously handles HTTPS server requests.
 *
 * @return void
 */
void loop() {
  secureServer.loop(); 
}
