#include <Arduino.h>
#include <PID_v1.h>
#include <DHT.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include "raum.h"
#include "sensor.h"

// Connect to the WiFi
const char *ssid = "WLAN-6BTGBV";
const char *password = "7046701041798220";
const char *mqtt_server = "192.168.2.133";

//MQTT initalisieren
WiFiClient espClient;
PubSubClient client(espClient);

//Schaltzeitintervall
//unsigned long WindowSize = 330000; //Switching intervall
unsigned long WindowSize = 700000; //Switching intervall
//unsigned long WindowSize = 10000; //Switching intervall
unsigned long windowStartTime;

Sensor sensor_martin;

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");
  payload[length] = '\0';
  char test = 'te';
  sensor_martin.ReadDataFromSensor(payload, length);
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    String random_hash = String(random(6));
    String client_name = "Heizungsregler_" + random_hash;
    if (client.connect(client_name.c_str()))
    {
      Serial.println("connected");
      // ... and subscribe to topic

      /////////////////IstTemperaturen///////////////////////
      //JSONPATH:$.AM2301.Temperature
      //Wohnzimmer
      client.subscribe("tele/Esstisch/SENSOR");
      client.subscribe("tele/Heineken/SENSOR");
      client.subscribe("tele/Wandleuchte/SENSOR");
      //Kueche
      client.subscribe("tele/Kuechenleuchte/SENSOR");
      //Wachzimmer
      client.subscribe("tele/Schreibtisch/SENSOR");
      //Schlafzimmer
      client.subscribe("tele/Schlafzimmer/SENSOR");
      //Bad
      client.subscribe("tele/Bad/SENSOR");
      //Martin
      client.subscribe("tele/Lavalampe/SENSOR");

      /////////////////SollTemperaturen//////////////////////
      //Wohnzimmer
      client.subscribe("cmnd/Wandleuchte/SollTemperatur");
      client.subscribe("cmnd/Esstisch/SollTemperatur");
      client.subscribe("cmnd/Heineken/SollTemperatur");
      client.subscribe("cmnd/Wohnzimmer/SollTemperatur");
      //Kueche
      client.subscribe("cmnd/Kuechenleuchte/SollTemperatur");
      //Wachzimmer
      client.subscribe("cmnd/Schreibtisch/SollTemperatur");
      //Schlafzimmer
      client.subscribe("cmnd/Schlafzimmer/SollTemperatur");
      //Bad
      client.subscribe("cmnd/Bad/SollTemperatur");
      //Martin
      client.subscribe("cmnd/Lavalampe/SollTemperatur");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  //Serial starten
  Serial.begin(115200);
  Serial.println("Heizungsregler DBWGWG starten!");

  windowStartTime = millis();

  //Wifi starten
  WiFi.mode(WIFI_STA);
  //WiFi.begin(ssid, password);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);

  //Warten bis Wifi verbunden ist
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  //Wifi connected
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  //Initialize OTA update
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
    {
      type = "sketch";
    }
    else
    { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
    {
      Serial.println("Auth Failed");
    }
    else if (error == OTA_BEGIN_ERROR)
    {
      Serial.println("Begin Failed");
    }
    else if (error == OTA_CONNECT_ERROR)
    {
      Serial.println("Connect Failed");
    }
    else if (error == OTA_RECEIVE_ERROR)
    {
      Serial.println("Receive Failed");
    }
    else if (error == OTA_END_ERROR)
    {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();

  sensor_martin.SetMqttChannel("tele / Esstisch / SENSOR");

  //Zu MQTT Server verbinden
  client.setServer(mqtt_server, 1883);
  //  Serial.println("DebugSetup1");
  client.setCallback(callback);
  //  Serial.println("DebugSetup2");

  // Raum raum_Martin = new Raum();
}

void loop()
{
  //Serial.println("Verbinde MQTT");
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  // const char msg = 'Hello World';
  // client.publish("TestTopic", &msg);
}
