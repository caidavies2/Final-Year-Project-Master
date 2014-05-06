        //MQTT Connection
        var val, stringVal;
        // var client = new Messaging.Client("75.101.161.236", 8083, "myclientid_" + parseInt(Math.random() * 100, 10));
        // var client = new Messaging.Client("146.185.174.52", 8080, "myclientid_" + parseInt(Math.random() * 100, 10));
        var client = new Messaging.Client("hazel.siddv.net", 8080, "myclientid_" + parseInt(Math.random() * 100, 10));
        var capturedUrl;
        var options = {

          //connection attempt timeout in seconds
          timeout: 3,

          //Gets Called if the connection has successfully been established
          onSuccess: function () {
            console.log("Connected to broker");
            client.subscribe("public/cai-fyp", {qos: 0});
            client.subscribe("public/cai-fyp/status", {qos: 0});
          },

          //Gets Called if the connection could not be established
          onFailure: function (message) {
            console.log("Connection failed: " + message.errorMessage);
          }
        };

        client.connect(options);

        var publish = function (payload, topic, qos) {
          var message = new Messaging.Message(payload);
          message.destinationName = topic;
          message.qos = qos;
          client.send(message);
        }
        client.onMessageArrived = function (message) {
          //Do something with the push message you received
          console.log(message.payloadString);
          handleMessage(message.destinationName, message.payloadString);
        };

        // Creating our leap controller

        var controller = new Leap.Controller();
        var trainer = new LeapTrainer.Controller({controller: controller});
        var chooseNotification = false;
        var numberOfFingers, lightOnFocus, numberOfHands;
        var previousNumberOfFingers = 0;
        var iCount;
        var start = false;
        var captured = false;
        controller.on( 'connect' , function(){

          console.log( 'Leap Successfully connected.' );

        });

        // Proving that a device can be connected
        controller.on( 'deviceConnected' , function() {

          console.log('A Leap device has been connected.');

        });

        // And that it can be disconnected
        controller.on( 'deviceDisconnected' , function() {

          console.log( 'A Leap device has been disconnected.' );

        });

        // When the controller is ready, spawn the unicorn!
        controller.on( 'frame' , function(frame){

          numberOfFingers = frame.fingers.length;
          numberOfHands = frame.hands.length;
          // Check the status of

          // if(numberOfHands == 1)
          // {
            // console.log(numberOfFingers);
            if(numberOfHands == 1 && numberOfFingers == 5)
            {
              
              // console.log('start');
              if(start !=true)
              {
                publish('activate','public/cai-fyp/fingerDetection', 0);
              }
              start = true;
              // console.log(start);
              // fingerStatus();
            }
          // }

          if(numberOfHands !== 1)
          {
            // console.log('stop');
            publish('not activated','public/cai-fyp/fingerDetection', 0);
            start = false;
            // console.log(start);
          }

          // Detect Fist

          if(start)
          {
            if(captured == false){
              if(detectFist(numberOfFingers, numberOfHands))
              {
                iCount = iCount + 1;

                if(iCount > 2)
                {
                capturedSound();
                // console.log(window.location.href);
                chrome.tabs.query({currentWindow: true, active: true}, function(tabs){
                  capturedUrl = tabs[0].url;
                  // sendDataToDatabase(tabs[0].url);
                  publish('captured','public/cai-fyp/status', 0);
                });
                captured = true;
                }
                
              }
              else
              {
                iCount = 0;
              }
            }
          }
        });
        function detectFist(fingers, hands)
        {
          if(fingers == 0  && hands == 1)
          {
            return true;
          }
          else {return false;}
        }

        function capturedSound()
        {
          document.write('<audio id="player" src="alert.mp3">');
          document.getElementById('player').play();
        }

        function sendTheUrl(urlString) {
          var req = new XMLHttpRequest();
          req.open("POST", "https://www.caidavies.me/workshops/fyp/aj.php", true);
          req.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
          req.send("name=" + urlString);
        }

        function sendDataToDatabase(urlString)
        {
          $.ajax({
          url: 'http://www.caidavies.me/workshops/fyp/aj.php', //This is the current doc
          type: "POST",
          data: {name:urlString},
          success: function(data){
           console.log(data);
         }
       })
        }

        function changeOverlayImage()
        {
          $('.grab-icon').fadeOut('slow');
          console.log('grab icon');
        }

        function handleMessage(topic, message)
        {
          if(topic == "public/cai-fyp/status" && message == "saved")
          {
            sendDataToDatabase(capturedUrl);
            captured = false;
          }
        }

        // connect controller
        controller.connect();

        setInterval(function(){

          console.log(iCount);

          }, 500);
