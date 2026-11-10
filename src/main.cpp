
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// Access Point credentials
const char* ssid = "ESP32-Control";
const char* password = "12345678";

// Onboard LED
#define LED_PIN 2

// Create web server on port 80
WebServer server(80);

// Web page
void handleRoot() {
  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <title>ESP32 Control Panel</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body {
        font-family: Arial;
        text-align: center;
        background: #eeeeee;
        padding: 30px;
      }
      .container {
        background: white;
        padding: 25px;
        border-radius: 15px;
        max-width: 400px;
        margin: auto;
      }
      button {
        padding: 15px 35px;
        font-size: 20px;
        margin: 10px;
        border: none;
        border-radius: 8px;
        color: white;
        cursor: pointer;
      }
      .on { background: green; }
      .off { background: red; }
    </style>
  </head>
  <body>
    <div class="container">
      <h1>ESP32 Control Panel</h1>
      <p>Welcome to my first IoT project!</p>
      <h2>LED Control</h2>
      <a href="/on"><button class="on">LED ON</button></a>
      <a href="/off"><button class="off">LED OFF</button></a>
    </div>
  </body>
  </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

// Turn LED ON
void handleLEDOn() {
  digitalWrite(LED_PIN, HIGH);
  server.sendHeader("Location", "/");
  server.send(303);
}

// Turn LED OFF
void handleLEDOff() {
  digitalWrite(LED_PIN, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println();
  Serial.println("Starting ESP32...");

  // Start Wi-Fi Access Point
  WiFi.softAP(ssid, password);

  Serial.println("Wi-Fi Access Point started!");
  Serial.print("Network: ");
  Serial.println(ssid);

  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Configure web routes
  server.on("/", handleRoot);
  server.on("/on", handleLEDOn);
  server.on("/off", handleLEDOff);

  // Start web server
  server.begin();

  Serial.println("HTTP server started!");
}

void loop() {
  server.handleClient();
}
