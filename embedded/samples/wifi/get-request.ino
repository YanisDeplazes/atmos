#include <ESP8266WiFi.h>

#define WIFI_SSID      "The name of your Wi-fi network"
#define WIFI_PASSWORD  "the password of your Wi-fi network"

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // Make sure the built-in LED is off on start-up.
  digitalWrite(LED_BUILTIN, HIGH);

  // Your WeMos tries to connect to your Wi-fi network
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Keep the while-statement alive as long as we are NOT connected
  // to the Wi-fi network.
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  // If we get out of the while-loop it means that we have a connection!
  // Now move on to the loop-function to blink the LED.
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW); // LED on
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH); // LED off
  delay(500);
}