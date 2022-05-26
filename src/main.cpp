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

const int waterLed = 23;
const int wifiLed = 22;
const int heatingLed = 21;

u8_t currentWaterPercentage = 5;
u8_t requiredWaterPercentage = 10;

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

    // if negative, set to 0, if more than 100 set to 100
    waterPercentage = waterPercentage < 0 ? 0 : waterPercentage;
    waterPercentage = waterPercentage > 100 ? 100 : waterPercentage;

    currentWaterPercentage = waterPercentage; // currentWaterPercentage is a global variable

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

void stopWater()
{
    // stop water
    digitalWrite(waterLed, LOW);
    server.send(200, "text/plane", "Water stopped");
}

void startWater()
{
    // start water
    digitalWrite(waterLed, HIGH);
    server.send(200, "text/plane", "Water started");
}

void fillWater()
{

    // update global variable of required water percentage
    currentWaterPercentage = server.arg("waterLevel").toInt();
    requiredWaterPercentage = server.arg("requiredWaterLevel").toInt();

    String responseObjectString = "{ \"requiredWaterPercentage\":" + String(requiredWaterPercentage) + ";" +
                                  "\"currentWaterPercentage\":" + String(currentWaterPercentage) + "}";

    // fill water
    Serial.println("fillWater()");
    Serial.println(currentWaterPercentage);
    Serial.println(requiredWaterPercentage);

    Serial.println("fillWater()");

    // turn on water pump if button pressed and required waterLevel is higher than current water level
    requiredWaterPercentage > currentWaterPercentage ? startWater() : stopWater();

    Serial.println();
    server.send(200, "text/plane", responseObjectString);
}

void checkConnection()
{
    if (!WiFi.isConnected())
    {
        digitalWrite(wifiLed, LOW);
        Serial.println("WiFi connection lost, reconnecting...");
        WiFi.begin(ssid, password);

        // wait for connection
        while (WiFi.waitForConnectResult() != WL_CONNECTED)
        {
            Serial.print(".");
        }
    }
    else
    {

        digitalWrite(wifiLed, HIGH);
    }
}

void updateState()
{
    if (currentWaterPercentage >= requiredWaterPercentage)
    {
        stopWater();
    }
}

void setup(void)
{
    Serial.begin(115200);

    // ESP 32 onboard LED
    pinMode(2, OUTPUT);

    // status leds
    pinMode(waterLed, OUTPUT);
    pinMode(wifiLed, OUTPUT);
    pinMode(heatingLed, OUTPUT);

    stopWater();
    digitalWrite(wifiLed, LOW);
    digitalWrite(heatingLed, LOW);

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

    // main page route
    server.on("/", handleRoot);

    // data recieving route
    server.on("/readData", handleData);

    // fill water route
    server.on("/fillWater", fillWater);

    // stop water
    server.on("/stopWater", stopWater);

    // start server
    server.begin();
    Serial.println("HTTP server started");
}

// loop function
void loop(void)
{
    checkConnection(); // check if wifi is still connected
    server.handleClient();
    updateState(); // check state of water and temperature and update accordingly
    delay(1);
}