
#include <WiFiManager.h>       // WiFiManager library for WiFi configuration via captive portal
#include <WiFiClientSecure.h>  // For secure HTTPS communication

// ---------- MACRO DEFINITIONS (CONFIGURABLE CONSTANTS) ----------

#define WIFI_AP_NAME         "yanisdeplazes_IOT_Portal"  // SSID of fallback access point
#define WIFI_AP_PASSWORD     "superSecret123"            // WPA2 password for fallback access point
#define PORTAL_TIMEOUT_SEC   180                         // Max duration (in seconds) to keep config portal open
#define RESTART_DELAY_MS     3000                        // Delay before restart on WiFi failure

// ---------- GLOBAL OBJECTS ----------

WiFiManager wifiManager;           // WiFiManager instance for handling WiFi setup
WiFiClientSecure secureClient;    // Secure client for HTTPS connections

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