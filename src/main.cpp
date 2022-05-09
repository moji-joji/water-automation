#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "OneWire.h"
#include "DallasTemperature.h"
#include "secrets.hpp"
#include "webpage.hpp"
#include "waterlevel.hpp"
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
    int waterPercentage = 100 - (Waterlevel::getWaterLevel() / 10) * 100;

    String waterPercentageStr = String(waterPercentage);
    responseObjectString += "\"waterPercentage\":" + waterPercentageStr;

    responseObjectString += "}";
    server.send(200, "text/plane", responseObjectString);
    // send response to client
}
void setup(void)
{
    Serial.begin(115200);
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