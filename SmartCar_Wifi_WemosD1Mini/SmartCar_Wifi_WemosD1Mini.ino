#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Motor Pin Definitions
#define leftm1     D5   // inB1 (Left Motor)
#define leftm2     D6   // inB2 (Left Motor)
#define rightm1    D7   // inA1 (Right Motor)
#define rightm2    D8   // inA2 (Right Motor)
#define left_EN    D3   // enB (Left Motor Enable)
#define right_EN   D4   // enA (Right Motor Enable)

// Wifi Setup
const char* ssid = "ESP-Car";
const char* password = "12345678";

ESP8266WebServer server(80);

void setup() {
    Serial.begin(115200);

    pinMode(leftm1, OUTPUT);
    pinMode(leftm2, OUTPUT);
    pinMode(rightm1, OUTPUT);
    pinMode(rightm2, OUTPUT);
    pinMode(left_EN, OUTPUT);
    pinMode(right_EN, OUTPUT);

    stop();

    WiFi.softAP(ssid, password);
    Serial.println("WiFi AP Started");


    server.on("/forward", forward);
    server.on("/backward", backward);
    server.on("/left", left);
    server.on("/right", right);
    server.on("/stop", stop);
    server.on("/", handleRoot);
    
 
    server.begin();
    Serial.println("Web server started");
}

void loop() {
    server.handleClient();
}

void handleRoot() {
    server.send(200, "text/html", R"rawliteral(
      <!DOCTYPE html>
      <html lang="en">
      <head>
          <meta charset="UTF-8">
          <meta name="viewport" content="width=device-width, initial-scale=1.0">
          <title>Robot Control</title>
          <style>
              body {
                  font-family: Arial, sans-serif;
                  text-align: center;
                  background-color: #f4f4f4;
                  margin: 0;
                  padding: 0;
                  display: flex;
                  justify-content: center;
                  align-items: center;
                  height: 100vh;
              }
              .container {
                  display: grid;
                  grid-template-columns: repeat(3, 80px);
                  grid-template-rows: repeat(3, 80px);
                  gap: 10px;
                  justify-content: center;
                  align-items: center;
              }
              button {
                  width: 80px;
                  height: 80px;
                  font-size: 20px;
                  border: none;
                  border-radius: 10px;
                  background-color: #007bff;
                  color: white;
                  box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.2);
              }
              button:active {
                  background-color: #0056b3;
              }
              .up { grid-column: 2; grid-row: 1; }
              .left { grid-column: 1; grid-row: 2; }
              .stop { grid-column: 2; grid-row: 2; background-color: red; }
              .right { grid-column: 3; grid-row: 2; }
              .down { grid-column: 2; grid-row: 3; }
          </style>
      </head>
      <body>
          <div class="container">
              <button class="up" onclick="sendCommand('forward')">&#9650;</button>
              <button class="left" onclick="sendCommand('left')">&#9664;</button>
              <button class="stop" onclick="sendCommand('stop')">&#9632;</button>
              <button class="right" onclick="sendCommand('right')">&#9654;</button>
              <button class="down" onclick="sendCommand('backward')">&#9660;</button>
          </div>
          <script>
              function sendCommand(command) {
                  fetch('/' + command).catch(error => console.log('Error:', error));
              }
          </script>
      </body>
      </html>
    )rawliteral");
}

void forward() {
    Serial.println("Moving forward");
    digitalWrite(left_EN, HIGH);
    digitalWrite(right_EN, HIGH);
    
    digitalWrite(leftm1, HIGH);
    digitalWrite(leftm2, LOW);
    digitalWrite(rightm1, LOW);
    digitalWrite(rightm2, HIGH);
    delay(200);  
    stop();
    server.send(200, "text/plain", "Forward");
}

void backward() {
    Serial.println("Moving backward");
    digitalWrite(left_EN, HIGH);
    digitalWrite(right_EN, HIGH);

    digitalWrite(leftm1, LOW);
    digitalWrite(leftm2, HIGH);
    digitalWrite(rightm1, HIGH);
    digitalWrite(rightm2, LOW);
    delay(200);
    stop();
    server.send(200, "text/plain", "Backward");
}

void left() {
    Serial.println("Turning left");
    digitalWrite(left_EN, HIGH);
    digitalWrite(right_EN, HIGH);

    digitalWrite(leftm1, HIGH);
    digitalWrite(leftm2, LOW);
    digitalWrite(rightm1, HIGH);
    digitalWrite(rightm2, LOW);
    delay(200);
    stop();
    server.send(200, "text/plain", "Left");
}

void right() {
    Serial.println("Turning right");
    digitalWrite(left_EN, HIGH);
    digitalWrite(right_EN, HIGH);
    
    digitalWrite(leftm1, LOW);
    digitalWrite(leftm2, HIGH);
    digitalWrite(rightm1, LOW);
    digitalWrite(rightm2, HIGH);
    delay(200);
    stop();
    server.send(200, "text/plain", "Right");
}

void stop() {
    Serial.println("Stopping");
    digitalWrite(left_EN, LOW);
    digitalWrite(right_EN, LOW);
    digitalWrite(leftm1, LOW);
    digitalWrite(leftm2, LOW);
    digitalWrite(rightm1, LOW);
    digitalWrite(rightm2, LOW);
    server.send(200, "text/plain", "Stop");
}
