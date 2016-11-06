#include <LivoloTx.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

static const char* WIFI_SSID = "!MY_WIFI_SSID!";
static const char* WIFI_PASSWORD = "!MY_WIFI_SSID!";

static const char* PASSCODE = "!SECRET!";

static const uint16_t LIVOLO_REMOTE_ID = 6400;

static const int TX_PIN = D1;

ESP8266WebServer server(8080);

int nextCmd = 0;
int nextBtn = 0;

LivoloTx gLivolo(TX_PIN);

void onLights() {
  const String& strButton = server.arg("command");
  const String& strPassword = server.arg("secret");
  if (strPassword.equals(PASSCODE))
  {
    nextBtn = strButton.toInt();
    nextCmd = 1;
    server.send(200, "text/plain", "lights on");
  }
  else
  {
    server.send(403, "text/plain", "access denied");
  }
}

void onIndex() {
  server.send(200, "text/html", "<h1>This is the light switch!</h1>");
}

void connectWiFi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void setup()
{
  Serial.begin(115200);
  Serial.println();

  pinMode(TX_PIN, OUTPUT);
  digitalWrite(TX_PIN, LOW);
  
  server.on("/lights", HTTP_ANY, onLights);
  server.on("/", onIndex);
  server.begin();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED)
  {
    connectWiFi();
  }
  
  server.handleClient();

  if (nextCmd)
  {
    gLivolo.sendButton(LIVOLO_REMOTE_ID, nextBtn);
    nextCmd = 0;
    nextBtn = 0;
    Serial.println("lights on!");
  }
}

