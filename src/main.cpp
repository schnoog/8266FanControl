#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h> // Include EEPROM library

#include <wificredentials.h>

ESP8266WebServer server(80);

int fanSpeed = 170; // Initial fan speed
int eeAddress = 0;

void handleRoot() {
  String html = "<html><head><title>Fan Control</title></head><body>";
  html += "<h1>Fan Speed Control</h1>";
  html += "<input type='range' min='0' max='255' value='" + String(fanSpeed) + "' id='fanSpeedSlider' onchange='updateFanSpeed(this.value)' />";
  html += "<p>Current Speed: <span id='currentSpeed'>" + String(fanSpeed) + "</span></p>";
  html += "<script>function updateFanSpeed(speed) {document.getElementById('currentSpeed').innerHTML = speed;var xhr = new XMLHttpRequest();xhr.open('GET', '/setspeed?speed=' + speed, true);xhr.send();}</script>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleNotFound() {
  server.send(404, "text/plain", "Not found");
}

void setFanSpeed(int speed) {
  Serial.print("Set speed to: ");
  Serial.println(speed);
  fanSpeed = speed;
  analogWrite(D5, fanSpeed); // Set the PWM output to control fan speed
  EEPROM.write(eeAddress, fanSpeed); // Save fan speed to EEPROM
  EEPROM.commit(); // Commit changes to EEPROM  
}

void handleSetSpeed() {
  Serial.println("Handle speed");
  if (server.args() > 0) {
    for (uint8_t i = 0; i < server.args(); i++) {
      if (server.argName(i) == "speed") {
        int speed = server.arg(i).toInt();
        setFanSpeed(speed);
        break;
      }
    }
  }
  server.send(200, "text/plain", "OK");
}











void setup() {
  pinMode(D5, OUTPUT); // D5 is connected to the PWM pin of the fan
  EEPROM.begin(512); 
  fanSpeed = EEPROM.read(eeAddress);
  analogWriteFreq(25000);
  analogWrite(D5,fanSpeed);
  Serial.begin(9600);
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Print the ESP8266 IP address
  Serial.println(WiFi.localIP());

  // Setup web server
  server.on("/", handleRoot);
  server.on("/setspeed", HTTP_GET, handleSetSpeed);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {
  server.handleClient();
  // You can add additional code here if needed
}

