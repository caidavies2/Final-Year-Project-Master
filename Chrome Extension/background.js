        //MQTT Connection
        var val, stringVal, capturedUrl, timerInstance;
        // var client = new Messaging.Client("75.101.161.236", 8083, "myclientid_" + parseInt(Math.random() * 100, 10));
        // var client = new Messaging.Client("146.185.174.52", 8080, "myclientid_" + parseInt(Math.random() * 100, 10));
        var client = new Messaging.Client("hazel.siddv.net", 8080, "myclientid_" + parseInt(Math.random() * 100, 10));
        var nodeTimer = 0;
        var options = {
          //connection attempt timeout in seconds
          timeout: 3,

          //Gets Called if the connection has successfully been established
          onSuccess: function () {
            console.log("Connected to broker");
            client.subscribe("public/cai-fyp", {qos: 0});
            client.subscribe("public/cai-fyp/status", {qos: 0});
            client.subscribe("public/cai-fyp/node-1/status", {qos:0});
            client.subscribe("public/cai-fyp/node-2/status", {qos:0});
            client.subscribe("public/cai-fyp/node-3/status", {qos:0});
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

          if(numberOfHands !== 1)
          {
            // console.log('stop');
            publish('not activated','public/cai-fyp/fingerDetection', 0);
            start = false;
            // console.log(start);
            iCount = 0;
          }


          else if(numberOfHands == 1 && numberOfFingers == 5)
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

          // Detect Fist

          if(start == true)
          {
            if(captured == false){
              if((detectFist(numberOfFingers, numberOfHands)) == true)
              {
                iCount = iCount + 1;

                if(iCount >= 4)
                {
                  captured = true;
                  capturedSound();
                  startTimer();
                  chrome.tabs.query({currentWindow: true, active: true}, function(tabs){
                    capturedUrl = tabs[0].url;
                    publish('captured','public/cai-fyp/status', 0);
                    publish('captured', 'public/cai-fyp/fingerDetection',0);                    
                  });
                  
                }
                
              }
            }

            else{captured == false;}
          }
        });

        function startTimer()
        {          
            timerInstance = setInterval(function(){
            nodeTimer++;
            var timeLeft = (60 - nodeTimer);
            chrome.browserAction.setBadgeText({text: timeLeft.toString()});
            if(nodeTimer == 60)
            {
              captured = false;
              clearTimer();
              publish('cleared','public/cai-fyp/status', 0);
              nodeTimer = 0;
            }
            else if(timeLeft <= 10)
            {                            
              chrome.browserAction.setBadgeBackgroundColor({color:"#c43825"});
            }
            else
            {
              chrome.browserAction.setBadgeBackgroundColor({color:"#77cc33"});
            }            
            }, 1000);

        }

        function clearTimer()
        {
          clearInterval(timerInstance);
          chrome.browserAction.setBadgeText({text: ""});
          nodeTimer = 0;
        }

        function detectFist(fingers, hands)
        {
          if(fingers == 1  && hands == 1 || fingers == 0 && hands == 0)
          {
            return true;
          }
          else {return false;}
        }

        function capturedSound()
        {
          document.write('<audio id="player" src="camera.wav">');
          document.getElementById('player').play();
        }
        function sendDataToDatabase(urlString, node)
        {
          $.ajax({
            url: 'http://www.hivenodes.com/fyp/add.php?node=' + node,
            type: "POST",
            data: {name:urlString},
            success: function(data){
             console.log(data);
             console.log('success');
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
          if(topic == "public/cai-fyp/node-1/status" && message == "saved")
          {          
            sendDataToDatabase(capturedUrl, "1");
            captured = false;
            clearTimer();
          }
          else if(topic == "public/cai-fyp/node-2/status" && message == "saved")
          {
            sendDataToDatabase(capturedUrl, "2");
            captured = false;
            clearTimer();
          }

          else if(topic == "public/cai-fyp/node-3/status" && message == "saved")
          {
            sendDataToDatabase(capturedUrl, "3");
            captured = false;
            clearTimer();
          }
        }

        // connect controller
        controller.connect();

        chrome.browserAction.onClicked.addListener(function callback(tab){
        clearTimer();
        captured =false;
        publish('cleared','public/cai-fyp/status', 0);
        });


