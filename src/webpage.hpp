#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <WiFi.h>

namespace Webpage
{

    void displayHTML(WiFiClient client, float tankLevel, float temperature)
    {
        //       // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
        // and a content-type so the client knows what's coming, then a blank line:
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/html");
        client.println("Connection: close");
        client.println();

        // Display the HTML web page
        client.println("<!DOCTYPE html><html>");
        client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
        client.println("<link rel=\"icon\" href=\"data:,\">");
        client.println("<style>");

        // styling
        ///////////////////////////////////////////////////////
        client.println("*{");
        client.println(" margin: 0%;");
        client.println("padding: 0%;");
        client.println("box-sizing: border-box;");
        client.println("font-family:  'Trebuchet MS', 'Lucida Sans Unicode', 'Lucida Grande', 'Lucida Sans', Arial, sans-serif");
        client.println("}");

        client.println(".container{");
        client.println("width: 90%;");
        client.println("margin: 0 auto;");
        client.println("}");

        client.println(".nav {");
        client.println("display: flex;");
        client.println("flex-direction: column;");
        client.println("align-items: center;");
        client.println("width: 100%;");
        client.println("}");

        client.println(".nav h1{");
        client.println("text-align: center;");
        client.println("color: #1FA1D5;");
        client.println("}");

        client.println(".logo{");
        client.println("width:");
        client.println("150px;");
        client.println("}");

        client.println(".bold{");
        client.println("font-weight: bold;");

        client.println("}");

        client.println(".water-level{");
        client.println("margin-top: 10%;");
        client.println("display: flex;");
        client.println("flex-direction: column;");
        client.println("justify-content: space-between;");
        client.println("}");

        client.println(".btn{");
        client.println("cursor: pointer;");
        client.println("width: fit-content;");
        client.println("padding: 0.7em 1em;");
        client.println("background-color: #1FA1D5;");
        client.println("color: white;");
        client.println("font-size: 1em;");
        client.println("border: none;");
        client.println("box-shadow: 5px 5px black ;");

        client.println("}");

        client.println(".btn:active{");
        client.println("box-shadow: none;");
        client.println("margin: 5px 5px;");
        client.println("}");

        client.println(".tank {");
        client.println("width: 150px;");
        client.println("height: 300px;");
        client.println("border: 2px solid black;");
        client.println(" background: white;");
        client.println("position: relative;");

        client.println("display: inline-block;");
        client.println(" margin: 10px;");
        client.println("}");

        client.println(".tank .water {");
        client.println("position: absolute;");
        client.println("background: blue;");
        client.println("width: 100%;");
        client.println("bottom: 0;");
        client.println("}");

        client.println("</style>");
        /////////////////////////////////////////////////////////

        client.println("</head>");
        // Web Page Heading
        client.println("<body><h1>ESP32 Web Server</h1>");
        client.println("<div class='container' >");
        client.println(" <div class='nav'>");
        client.println("<img");
        client.println("   src='https://png.pngtree.com/template/20190316/ourlarge/pngtree-water-logo-image_79145.jpg'");
        client.println("   class='logo'");
        client.println("   alt=''");
        client.println(" />");
        client.println(" <h1>");
        client.println("  <span class='bold'> Water </span>");
        client.println("   Automation");
        client.println(" </h1>");
        client.println(" </div>");
        client.println(" <div class='water-level'>");
        client.println(" <h2>Water Level Control</h2>");
        client.println(" <h4>Current Level: ");
        client.println(tankLevel * 100);
        client.println("%</h4>");
        client.println("</h4> ");
        client.println("<div class = 'tank'>");
        client.println("<div class = 'water' style = 'height:");
        client.print(tankLevel * 100);

        client.print("%;'>");
        client.println("></ div>");
        client.println("</ div>");

        client.println("<button class='btn'>Fill Water</button>");
        client.println("  </div>");
        client.println(" </div>");

        client.println("</body></html>");

        // The HTTP response ends with another blank line
        client.println();
        // Break out of the while loop
    }

}

#endif