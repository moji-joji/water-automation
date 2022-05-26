const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Water Automation</title>
    <style>
      * {
        margin: 0%;
        padding: 0%;
        box-sizing: border-box;
        font-family: "Trebuchet MS", "Lucida Sans Unicode", "Lucida Grande",
          "Lucida Sans", Arial, sans-serif;
      }

      .container {
        width: 90%;
        margin: 0 auto;
      }

      .nav {
        display: flex;
        flex-direction: column;
        align-items: center;
        width: 100%;
      }

      .nav h1 {
        text-align: center;
        color: #1fa1d5;
      }

      .logo {
        width: 150px;
      }

      .bold {
        font-weight: bold;
      }

      .water-level {
        margin-top: 10%;
        display: flex;
        flex-direction: column;
        justify-content: space-between;
      }

      .water-level > * {
        margin-top: 5vh;
      }

      .water-level > h4 {
        color: red;
      }

      .btn,
      .btn-red {
        cursor: pointer;
        width: fit-content;
        padding: 0.7em 1em;
        background-color: #1fa1d5;
        color: white;
        font-size: 1em;
        border: none;
        box-shadow: 5px 5px black;
      }

      .btn-red {
        background-color: red;
      }

      .btn:active,
      .btn-red:active {
        box-shadow: none;
      }

      .tank {
        width: 150px;
        height: 300px;
        border: 2px solid black;
        background: white;
        position: relative;

        display: inline-block;
        margin: 10px;
      }

      .tank .water {
        position: absolute;
        background: blue;
        width: 100%;
        bottom: 0;
        border-radius: 10px;
      }

      .temperature-card {
        width: 100%;
        margin-top: 10vh;
      }
      .temperature-card > h4 {
        color: red;
      }

      .temperature-card > * {
        margin-top: 5vh;
      }

      .flex-container {
        display: flex;
        flex-direction: row;
        justify-content: space-between;
        align-items: center;
        width: 20%;
      }

      input[type="range"][orient="vertical"] {
        writing-mode: bt-lr; /* IE */
        -webkit-appearance: slider-vertical; /* Chromium */
        width: 8px;
        height: 300px;
        padding: 0 5px;
        margin-top: 10px;
      }
      #display-water-input {
        margin: 150px 0 0px 10px;
        color: #1fa1d5;
        font-weight: bold;
        font-size: 16px;
      }
    </style>
    <link rel="stylesheet" href="style.css" />
  </head>
  <body>
    <div class="container">
      <div class="nav">
        <img
          src="https://png.pngtree.com/template/20190316/ourlarge/pngtree-water-logo-image_79145.jpg"
          class="logo"
          alt=""
        />
        <h1>
          <span class="bold"> Water </span>
          Automation
        </h1>
      </div>
      <div class="water-level">
        <h2>Water Level Control</h2>
        <h4>Current Level: <span id="water-percentage"> </span></h4>
        <div style="display: flex">
          <div class="tank">
            <div class="water" id="blue-water" style="height: 0%"></div>
          </div>
          <input id="water-input" type="range" orient="vertical" />
          <span id="display-water-input">50</span
          ><span id="display-water-input">%</span>
        </div>
      </div>

      <button class="btn" onclick="fillWater()">Fill Water</button>

      <button class="btn-red" onclick="stopWater()">Stop Filling Water</button>

      <div class="temperature-card">
        <h2>Temperature Control</h2>
        <h4>Current Temperature: <span id="temperature"> </span></h4>
        20°C
        <input id="temperature-input" type="range" min="20" max="50" />
        50°C
        <h3>Heat upto: <span id="display-temperature-input"></span>°C</h3>
        <button class="btn" onclick="heatWater()">Heat water</button>
        <button class="btn-red" onclick="stopHeating()">Stop heating</button>

      </div>
    </div>
  </body>

  <script>
    // get data from site
    document.getElementById("display-temperature-input").innerHTML =
      document.getElementById("temperature-input").value;

    document
      .getElementById("water-input")
      .addEventListener("input", function (e) {
        document.getElementById("display-water-input").innerHTML =
          e.target.value;
        console.log("slider moved");
      });

    document
      .getElementById("temperature-input")
      .addEventListener("input", function (e) {
        document.getElementById("display-temperature-input").innerHTML =
          e.target.value;
        console.log("slider moved");
      });

    // getData from ESP each interval  1 second
    setInterval(function () {
      // Call a function repetatively with 1 Second interval
      getData();
    }, 1000); //1000mSeconds update rate

    function getData() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          const responseObj = JSON.parse(this.responseText);

          // render data from response
          document.getElementById("water-percentage").textContent =
            responseObj.waterPercentage + "%";
          document.getElementById("blue-water").style.height =
            responseObj.waterPercentage + "%";

          document.getElementById("temperature").textContent =
            responseObj.waterTemperature + "°C";
          console.log(responseObj.waterTemperature);

          console.log(this.responseText);
        }
      };
      xhttp.open("GET", "readData", true);
      xhttp.send();
    }

    function fillWater() {
      // fill water request according to slider value
      let waterPercentage =
        document.getElementById("water-percentage").textContent;

      let requiredWaterPercentage = document.getElementById(
        "display-water-input"
      ).textContent;
      console.log(requiredWaterPercentage);

      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          console.log(this.responseText);
        }
      };
      xhttp.open("POST", "fillWater", true);
      xhttp.setRequestHeader(
        "Content-type",
        "application/x-www-form-urlencoded"
      );

      xhttp.send(
        "waterLevel=" +
          waterPercentage +
          "&requiredWaterLevel=" +
          requiredWaterPercentage
      );

      console.log("Water filled request sent");
    }

    function stopWater() {
      // emergency stop request
      console.log("Stop Water button clicked");
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          console.log(this.responseText);
        }
      };
      xhttp.open("POST", "stopWater", true);
      xhttp.setRequestHeader(
        "Content-type",
        "application/x-www-form-urlencoded"
      );

      xhttp.send();

      console.log("Water stopped request sent");
    }

    function heatWater() {
      // current water level
      let temperature = document.getElementById("temperature").textContent;

      // the required temperature of water to be heated
      let requiredTemperature = document.getElementById(
        "display-temperature-input"
      ).textContent;
      console.log(requiredTemperature);

      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          console.log(this.responseText);
        }
      };
      xhttp.open("POST", "heatWater", true);
      xhttp.setRequestHeader(
        "Content-type",
        "application/x-www-form-urlencoded"
      );

      // xhttp.send(
      //   "temperature=" +
      //     temperature +
      //     "&requiredTemperature=" +
      //     requiredTemperature
      // );
   
      xhttp.send(
        "temperature=" +
          25.23 +
          "&requiredTemperature=" +
          35.32
      );
   
    }

    function stopHeating(){
      // emergency stop request
      console.log("Stop heating button clicked");
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          console.log(this.responseText);
        }
      };
      xhttp.open("POST", "stopHeating", true);
      xhttp.setRequestHeader(
        "Content-type",
        "application/x-www-form-urlencoded"
      );

      xhttp.send();

      console.log("Heating stopped request sent");
    }
  </script>
</html>

)=====";