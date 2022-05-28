const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <script src="https://code.highcharts.com/highcharts.js"></script>
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
        margin-bottom: 10vh;
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

      .chart-water {
        width: 100%;

        margin-bottom: 10vh;
      }

      .chart-temperature {
        width: 100%;
      }

      .chart-water {
        width: 100%;
        margin-top: 10vh;
        margin-bottom: 10vh;
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
        <h4 id="eta-level">
          Estimated time to fill: <span id="waterfill-eta"></span>seconds
        </h4>
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

      <div class="schedule-filling" style="margin: 40px 0px 40px 0px">
        <input
          type="datetime-local"
          name="scheduled-time"
          id="scheduled-time"
        />
        <button class="btn" onclick="scheduleFilling()">
          Schedule Filling
        </button>

        <h3>
          Schedule:
          <table class="schedule-table"></table>
          <button class="btn-red" onclick="deleteFromWaterSchedule()">
            Delete last added
          </button>
        </h3>
      </div>

      <div class="temperature-card">
        <h2>Temperature Control</h2>
        <h4>Current Temperature: <span id="temperature"> </span></h4>
        °C
        <h4 id="eta-temperature">
          Estimated time to heat: <span id="waterheat-eta"></span>seconds
        </h4>
        <input id="temperature-input" type="range" min="20" max="50" />
        50°C
        <h3>Heat upto: <span id="display-temperature-input"></span>°C</h3>
        <button class="btn" onclick="heatWater()">Heat water</button>
        <button class="btn-red" onclick="stopHeating()">Stop heating</button>
      </div>

      <div id="chart-water" class="container"></div>

      <div id="chart-temperature" class="container"></div>
    </div>
  </body>

  <script>
    var prevWaterLevel = 0;
    var beingFilled = 0;
    var beingHeated = 0;
    var requiredWaterPercentage = 50;
    var requiredWaterTemperature = 35;
    var prevWaterTemperature = 0;
    var prevTempEta = 0;

    // Chart rendering
    var chartW = new Highcharts.Chart({
      chart: { renderTo: "chart-water" },
      title: { text: "Water Level" },
      series: [
        {
          showInLegend: false,
          data: [],
        },
      ],
      plotOptions: {
        line: { animation: false, dataLabels: { enabled: true } },
        series: { color: "#059e8a" },
      },
      xAxis: { type: "datetime", dateTimeLabelFormats: { second: "%H:%M:%S" } },
      yAxis: {
        title: { text: "%" },
        //title: { text: 'Temperature (Fahrenheit)' }
      },
      credits: { enabled: false },
    });

    // Chart rendering
    var chartT = new Highcharts.Chart({
      chart: { renderTo: "chart-temperature" },
      title: { text: "Water Temperature" },
      series: [
        {
          showInLegend: false,
          data: [],
        },
      ],
      plotOptions: {
        line: { animation: false, dataLabels: { enabled: true } },
        series: { color: "#059e8a" },
      },
      xAxis: { type: "datetime", dateTimeLabelFormats: { second: "%H:%M:%S" } },
      yAxis: {
        title: { text: "Temperature (Celsius)" },
        //title: { text: 'Temperature (Fahrenheit)' }
      },
      credits: { enabled: false },
    });

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
      // get and process all data to render the website accordingly

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

          // calculate estimated time to fill water

          // current Water percentage
          let waterPercentage = parseInt(responseObj.waterPercentage);

          // read the required water percentage
          requiredWaterPercentage = parseInt(requiredWaterPercentage);
          // calculate the difference between the current water percentage and the previous water percentage one second ago
          let waterFillRate = waterPercentage - prevWaterLevel;

          // number of seconds required to fill the water
          let waterFillEta =
            (requiredWaterPercentage - waterPercentage) / waterFillRate;

          waterFillEta = waterFillEta.toFixed(0);

          document.getElementById("waterfill-eta").innerHTML = waterFillEta;
          prevWaterLevel = waterPercentage;

          if (
            waterFillEta > 0 &&
            waterFillEta != Infinity &&
            parseInt(responseObj.waterIsFilling) == 1
          ) {
            document.getElementById("eta-level").style.display = "block";
          } else {
            document.getElementById("eta-level").style.display = "none";
          }

          // calculate estimated time to heat water

          // calculate estimated time to fill water

          // current Water temperature
          let waterTemperature = parseFloat(responseObj.waterTemperature);

          // read the required water percentage
          requiredWaterTemperature = parseFloat(requiredWaterTemperature);

          console.log(waterTemperature);
          console.log(requiredWaterTemperature);
          console.log(prevWaterTemperature);

          // calculate the difference between the current temp  and the previous temp, one second ago
          let waterHeatRate = waterTemperature - prevWaterTemperature;

          // number of seconds required to heat the water
          let waterHeatEta =
            (requiredWaterTemperature - waterTemperature) / waterHeatRate;

          waterHeatEta = waterHeatEta.toFixed(0);

          console.log(waterHeatEta);

          // update water Temperature
          prevWaterTemperature = waterTemperature;

          if (
            waterHeatEta > 0 &&
            waterHeatEta != Infinity &&
            parseInt(responseObj.waterIsHeating) == 1
          ) {
            document.getElementById("waterheat-eta").innerHTML = waterHeatEta;

            document.getElementById("eta-temperature").style.display = "block";
          }

          // if infinity or 0 we just show previous value however in real life we should show actual value
          else if (
            (waterHeatEta < 0 || waterHeatEta == Infinity) &&
            parseInt(responseObj.waterIsHeating) == 1
          ) {
            document.getElemen;
            asdtById("waterheat-eta").innerHTML = prevTempEta;
            document.getElementById("eta-temperature").style.display = "block";
          } else {
            document.getElementById("eta-temperature").style.display = "none";
          }

          if (waterHeatEta > 0 && waterHeatEta != Infinity) {
            prevTempEta = waterHeatEta;
          }
        }
      };
      xhttp.open("GET", "readData", true);
      xhttp.send();
    }

    // plot water level data
    setInterval(() => {
      var x = new Date().getTime(),
        y = parseInt(document.getElementById("water-percentage").textContent);
      //console.log(this.responseText);
      if (chartW.series[0].data.length > 40) {
        chartW.series[0].addPoint([x, y], true, true, true);
      } else {
        chartW.series[0].addPoint([x, y], true, false, true);
      }
    }, 1000);

    // plot temperature data
    setInterval(() => {
      var x = new Date().getTime(),
        y = parseFloat(document.getElementById("temperature").textContent);
      //console.log(this.responseText);
      if (chartT.series[0].data.length > 40) {
        chartT.series[0].addPoint([x, y], true, true, true);
      } else {
        chartT.series[0].addPoint([x, y], true, false, true);
      }
    }, 1000);

    function fillWater() {
      // fill water request according to slider value
      let waterPercentage =
        document.getElementById("water-percentage").textContent;

      requiredWaterPercentage = document.getElementById(
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

      // update global variable for tracking rate
      requiredWaterTemperature = requiredTemperature;

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

      xhttp.send(
        "waterTemperature=" +
          temperature +
          "&requiredWaterTemperature=" +
          requiredTemperature
      );
    }

    function stopHeating() {
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

    // global array
    var scheduleJson = [];
    var timeoutArr = [];

    function printWaterFillingSchedule() {
      for (let i = 0; i < scheduleJson.length; i++) {
        document.getElementsByClassName("schedule-table")[0].innerHTML +=
          "<tr><td>" +
          scheduleJson[i].time +
          "</td><td>" +
          scheduleJson[i].waterLevel +
          "</td></tr>";
      }
    }

    // schedule filling water
    function scheduleFilling() {
      // get the time from the input
      let time = document.getElementById("scheduled-time").value;
      console.log(time);

      // get the water level from the input
      let requiredWaterPercentage =
        document.getElementById("water-input").value;

      let waterLevel = document.getElementById("water-percentage").textContent;

      timeoutArr.push(
        setTimeout(() => {
          console.log("Scheduled filling request sent");
          // send request to fill water
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
              waterLevel +
              "&requiredWaterLevel=" +
              requiredWaterPercentage
          );

          console.log(new Date());

          scheduleJson.shift();
          timeoutArr.shift();
        }, new Date(time).getTime() - new Date().getTime())
      );

      // add the request to the schedule
      scheduleJson.push({
        time: time,
        waterLevel: requiredWaterPercentage,
      });
      document.getElementsByClassName("schedule-table")[0].innerHTML = "";

      printWaterFillingSchedule();
    }

    function deleteFromWaterSchedule() {
      scheduleJson.pop();
      clearTimeout(timeoutArr.pop());
    }
    setInterval(() => {
      // print schedule
      document.getElementsByClassName("schedule-table")[0].innerHTML = "";
      printWaterFillingSchedule();

      console.log(scheduleJson);

      console.log(timeoutArr);
    }, 1000);
  </script>
</html>

)=====";