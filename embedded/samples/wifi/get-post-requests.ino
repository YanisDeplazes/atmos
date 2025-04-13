#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#define WIFI_SSID      "iotroam"
#define WIFI_PASSWORD  "miRzOuMhm1"

WiFiClient client;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.print("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFailed to connect to WiFi!");
    return;
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Example Usage
  String getResponse = sendGETRequest("http://145.92.189.63/api/measurements");
  Serial.println("GET Response: " + getResponse);

  float temperature = 22.5;  // Replace with actual sensor data
  float humidity = 60.0;

    // Creating JSON payload
  String postData = "{\"temperature\":" + String(temperature, 2) + 
                       ", \"humidity\":" + String(humidity, 2) + "}";

  String postResponse = sendPOSTRequest("http://145.92.189.63/api/measurements", postData);
  Serial.println("POST Response: " + postResponse);
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW); // LED on
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH); // LED off
  delay(500);
}

// Function to send a GET request
String sendGETRequest(String endpoint) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected, skipping GET request.");
    return "";
  }

  HTTPClient http;
  http.begin(client, endpoint);

  int httpCode = http.GET();
  String response = "";
  
  if (httpCode > 0) {
    response = http.getString();
  } else {
    Serial.println("GET Request failed: " + http.errorToString(httpCode));
  }

  http.end();
  return response;
}

// Function to send a POST request with JSON body
String sendPOSTRequest(String endpoint, String jsonPayload) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected, skipping POST request.");
    return "";
  }

  HTTPClient http;
  http.begin(client, endpoint);
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(jsonPayload);
  String response = "";
  
  if (httpCode > 0) {
    response = http.getString();
  } else {
    Serial.println("POST Request failed: " + http.errorToString(httpCode));
  }

  http.end();
  return response;
}
