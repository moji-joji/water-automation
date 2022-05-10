#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "OneWire.h"
#include "DallasTemperature.h"
#include "secrets.hpp"
#include "waterlevel.hpp"
#include "watertemperature.hpp"
#include "index.h" //Web page header file

WebServer server(80);

// credentials
const char *ssid = Secrets::wifiSSID;
const char *password = Secrets::wifiPassword;

// main page route
void handleRoot()
{
    String htmlString = MAIN_page;             // html string made
    server.send(200, "text/html", htmlString); // send to webpage
}

void handleData()
{
    // create object for respose
    String responseObjectString = "{";

    // fill data in the object after reading sensor values
    // calculate distance of sensor from water level, this is the empty height of the tank
    // if we subtract percentage of empty height from 100, we will get percentage filled with water
    float waterDistance = Waterlevel::getWaterLevel();

    // as the sensor is 2.75cm away from the max water level, we need to subtract 2.75 from the distance
    waterDistance = waterDistance - 2.75;

    // total height of container is 8cm, so we find percentage according to it
    int waterPercentage = 100 - ((waterDistance) / 8) * 100;

    // if negative, set to 0
    waterPercentage = waterPercentage < 0 ? 0 : waterPercentage;

    String waterDistanceString = String(waterDistance);
    responseObjectString += "\"waterDistance\":" + waterDistanceString + ",";

    String waterPercentageStr = String(waterPercentage);
    responseObjectString += "\"waterPercentage\":" + waterPercentageStr;

    // get temperature from sensor
    float temperature = Watertemperature::getWaterTemperature();
    String temperatureStr = String(temperature);
    responseObjectString += ",\"waterTemperature\":" + temperatureStr;

    responseObjectString += "}";
    server.send(200, "text/plane", responseObjectString);
    // send response to client
}
void setup(void)
{
    Serial.begin(115200);

    // initialize temperature sensor bus
    Watertemperature::sensors.begin();

    pinMode(Waterlevel::trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(Waterlevel::echoPin, INPUT);  // Sets the echoPin as an Input

    Serial.println();

    // connect to wifi network
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    Serial.println("Connecting to ");
    Serial.print(ssid);

    // wait for connection
    while (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        Serial.print(".");
    }

    // If connection successful show IP address in serial monitor
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP()); // Print IP address assigned to ESP

    // main oage route
    server.on("/", handleRoot);

    // data recieving route
    server.on("/readData", handleData);

    // start server
    server.begin();
    Serial.println("HTTP server started");
}

// loop function
void loop(void)
{
    server.handleClient();
    delay(1);
}