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

// global arrays for scheduling
var scheduleFillingJson = [];
var timeoutFillingArr = [];
var scheduleHeatingJson = [];
var timeoutHeatingArr = [];

function printWaterFillingSchedule() {
    document.getElementsByClassName("schedule-table")[0].innerHTML += "<tr>  <th> Time </th><th> Water Percentage </th> </tr>";
  for (let i = 0; i < scheduleFillingJson.length; i++) {

    document.getElementsByClassName("schedule-table")[0].innerHTML +=
      "<tr><td>" +
      scheduleFillingJson[i].time +
      "</td><td>" +
      scheduleFillingJson[i].waterLevel +
      "%</td></tr>";
  }
}

function printWaterHeatingSchedule() {
 document.getElementsByClassName("schedule-table-heat")[0].innerHTML += "<tr> <th> Time </th><th> Water Temperature </th></tr>";
  for (let i = 0; i < scheduleHeatingJson.length; i++) {


    document.getElementsByClassName("schedule-table-heat")[0].innerHTML +=
      "<tr><td>" +
      scheduleHeatingJson[i].time +
      "</td><td>" +
      scheduleHeatingJson[i].waterTemperature +
      "°C</td></tr>";
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

  timeoutFillingArr.push(
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

      scheduleFillingJson.shift();
      timeoutFillingArr.shift();
    }, new Date(time).getTime() - new Date().getTime())
  );

  // add the request to the schedule
  scheduleFillingJson.push({
    time: time,
    waterLevel: requiredWaterPercentage,
  });
  document.getElementsByClassName("schedule-table")[0].innerHTML = "";

  printWaterFillingSchedule();
}

// schedule heating water
function scheduleHeating() {
  // get the time from the input
  let time = document.getElementById("scheduled-time-heat").value;
  console.log(time);

  // get the water temp from the input
  let requiredWaterTemperature =
    document.getElementById("temperature-input").value;

  let waterTemperature = document.getElementById("temperature").textContent;

  timeoutHeatingArr.push(
    setTimeout(() => {
      console.log("Scheduled heating request sent");
      // send request to fill water
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
          waterTemperature +
          "&requiredWaterTemperature=" +
          requiredWaterTemperature
      );

      console.log(new Date());

      scheduleHeatingJson.shift();
      timeoutHeatingArr.shift();
    }, new Date(time).getTime() - new Date().getTime())
  );

// add the request to the schedule
  scheduleHeatingJson.push({
    time: time,
    waterTemperature: requiredWaterTemperature,
  });
  document.getElementsByClassName("schedule-table")[0].innerHTML = "";

  printWaterFillingSchedule();
}

// delete and remove timeout from schedule
function deleteFromWaterSchedule() {
  scheduleFillingJson.pop();
  clearTimeout(timeoutFillingArr.pop());
}

function deleteFromHeatSchedule() {
  scheduleHeatingJson.pop();
  clearTimeout(timeoutHeatingArr.pop());
}

// print the schedule after one second update

setInterval(() => {
  // print schedule
  document.getElementsByClassName("schedule-table")[0].innerHTML = "";
  printWaterFillingSchedule();

  // print heating schedule
  document.getElementsByClassName("schedule-table-heat")[0].innerHTML = "";
  printWaterHeatingSchedule();

  console.log(scheduleFillingJson);
  console.log(timeoutFillingArr);
  console.log(scheduleHeatingJson);
  console.log(timeoutHeatingArr);
}, 1000);