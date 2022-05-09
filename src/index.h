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

      .btn {
        cursor: pointer;
        width: fit-content;
        padding: 0.7em 1em;
        background-color: #1fa1d5;
        color: white;
        font-size: 1em;
        border: none;
        box-shadow: 5px 5px black;
      }

      .btn:active {
        box-shadow: none;
        margin: 5px 5px;
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
        <div class="tank">
          <div class="water" id="blue-water" style="height: 0%"></div>
        </div>

        <button class="btn">Fill Water</button>
      </div>
    </div>
  </body>

  <script>
    setInterval(function () {
      // Call a function repetatively with 2 Second interval
      getData();
    }, 1000); //2000mSeconds update rate

    function getData() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            const responseObj = JSON.parse(this.responseText);

          document.getElementById("water-percentage").textContent =
            responseObj.waterPercentage;
document.getElementById("blue-water").style.height = responseObj.waterPercentage  + "%";

          console.log(this.responseText);
        }
      };
      xhttp.open("GET", "readData", true);
      xhttp.send();
    }
  </script>
</html>

)=====";