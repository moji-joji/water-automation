
#include <Arduino.h>
#include <WiFi.h>
#include "secrets.hpp"
#include "webpage.hpp"

// wifi info
const char *ssid = Secrets::wifiSSID;
const char *password = Secrets::wifiPassword;

const int trigPin = 5;
const int echoPin = 18;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

// define sound speed in cm/uS
#define SOUND_SPEED 0.034
// #define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

// variable to store http request
String header;

// Set web server port number to 80
WiFiServer server(80);

void setup()
{
  Serial.begin(115200);     // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);  // Sets the echoPin as an Input

  // connect to wifi network
  Serial.print("Connecting: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    // keep trying to connect
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  // server is started
}

void loop()
{

  // server is listening for client
  WiFiClient client = server.available();

  // if request recieved, save data
  if (client)
  {                                // If a new client connects,
    Serial.println("New Client."); // print a message out in the serial port
    String currentLine = "";       // make a String to hold incoming data from the client
    while (client.connected())
    { // loop while the client's connected
      if (client.available())
      {                         // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        Serial.write(c);        // print it out the serial monitor
        header += c;
        if (c == '\n')
        { // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if (client)
            { // If a new client connects,
              currentTime = millis();
              previousTime = currentTime;
              Serial.println("New Client."); // print a message out in the serial port
              String currentLine = "";       // make a String to hold incoming data from the client
              while (client.connected() && currentTime - previousTime <= timeoutTime)
              { // loop while the client's connected
                currentTime = millis();
                if (client.available())
                {                         // if there's bytes to read from the client,
                  char c = client.read(); // read a byte, then
                  Serial.write(c);        // print it out the serial monitor
                  header += c;
                  if (c == '\n')
                  { // if the byte is a newline character
                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0)
                    {
                      Webpage::displayHTML(client);
                      break;
                    }
                    else
                    { // if you got a newline, then clear currentLine
                      currentLine = "";
                    }
                  }
                  else if (c != '\r')
                  {                   // if you got anything else but a carriage return character,
                    currentLine += c; // add it to the end of the currentLine
                  }
                }
              }
              // Clear the header variable
              header = "";
              // Close the connection
              client.stop();
              Serial.println("Client disconnected.");
              Serial.println("");
            }

            //////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////

            // Clears the trigPin
            digitalWrite(trigPin, LOW);
            delayMicroseconds(2);
            // Sets the trigPin on HIGH state for 10 micro seconds
            digitalWrite(trigPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(trigPin, LOW);

            // Reads the echoPin, returns the sound wave travel time in microseconds
            duration = pulseIn(echoPin, HIGH);

            // Calculate the distance
            distanceCm = duration * SOUND_SPEED / 2;

            // Convert to inches
            // distanceInch = distanceCm * CM_TO_INCH;

            // Prints the distance in the Serial Monitor

            Serial.print("Distance : ");
            Serial.print(distanceCm);
            Serial.println(" cm");

            delay(1000);
          }
        }
      }
    }
  }
}
