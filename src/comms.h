////// Wifi server //////
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
const char* ssid = "Brain Damage";
const char* password = "pink floyd";
AsyncWebServer server(80);

////// Wifi client //////
#include <HTTPClient.h>
const char* serverNameLevel = "http://192.168.4.1/level";

int slice;
int level;
double slice_levels[NUM_SLICES];

String readLevel() {
  char message[3];
  int average = 0;
  for (int i=0; i<NUM_SLICES; i++)
  {
    average += slice_levels[i];
  }
  sprintf(message, "%d", (int)(average / NUM_SLICES));
  return message;
}

void setupWifiServer() {
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/level", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", readLevel().c_str()); });

  server.on("/level", HTTP_POST, [](AsyncWebServerRequest *request)
            {
        char *ptr;
        if (request->hasParam("slice", true) && request->hasParam("level", true))
        {
          slice = strtol(request->getParam("slice", true)->value().c_str(), &ptr, 10);
          slice_levels[level] = strtol(request->getParam("level", true)->value().c_str(), &ptr, 10);
        }
        else
        {
          request->send(400, "text/plain", "slice and level parameters needed");
          return;
        }
        request->send(200, "text/plain", "Dentrite level set"); });

  // Start server
  server.begin();
}

void setupWifiClient() {
    WiFi.begin(ssid, password);
    Serial.println("Connecting");
    while(WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
}

String httpGETRequest(const char* serverName) {
  HTTPClient http;

  // Your IP address with path or Domain name with URL path
  http.begin(serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "--";

  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}