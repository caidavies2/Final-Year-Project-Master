        $(document).ready(function(){
          $("body").wrapInner("<div id='fyp-container'></div>");
          $("#fyp-container").after("<div id='fyp-overlay'><div class='grab-icon animated'></div></div>");});
        var fistUrl = chrome.extension.getURL('icon.png');
        $('#fyp-overlay .grab-icon').attr('src', fistUrl);
        console.log(fistUrl);

        //MQTT Connection
        var val, stringVal;
        // var client = new Messaging.Client("75.101.161.236", 8083, "myclientid_" + parseInt(Math.random() * 100, 10));
        // var client = new Messaging.Client("146.185.174.52", 8080, "myclientid_" + parseInt(Math.random() * 100, 10));
        var client = new Messaging.Client("hazel.siddv.net", 8080, "myclientid_" + parseInt(Math.random() * 100, 10));
        var handState;
        var testData = "monkeh";
        var options = {
          //connection attempt timeout in seconds
          timeout: 3,

          //Gets Called if the connection has successfully been established
          onSuccess: function () {
            console.log("Connected to broker");
            client.subscribe("public/cai-fyp/fingerDetection", {qos: 0});
            client.subscribe("public/cai-fyp/");
          },

          //Gets Called if the connection could not be established
          onFailure: function (message) {
            console.log("Connection failed: " + message.errorMessage);
          }
        };

        client.connect(options);

        client.onMessageArrived = function (message) {
          handleMessage(message.destinationName, message.payloadString);

        };


        function handIn()
        {
          handState = true;
          $('#fyp-container').addClass('notification-on');
          // console.log("Hand In");

          setTimeout(function(){
            showNotification(handState);
          }, 500)
        }

        function handOut()
        {
          handState = false;
          if ($('#fyp-container').hasClass('notification-on')){
            $('#fyp-container').removeClass('notification-on');
          } 
          removeNotification();
        }

        function showNotification(hand)
        {
          if(hand)
          {
            $("#fyp-overlay").fadeToggle('fast', function(){
              $('.grab-icon').addClass('rubberBand');
            });
          }
        }

        function capturedNotification()
        {
          $('.grab-icon').addClass('grab-notification');

        }

        function removeNotification()
        {
          $("#fyp-overlay").fadeOut('fast');
            // $('.grab-icon').removeClass('grab-notification');
          }

          function handleMessage(topic, message)
          {


            if(topic == "public/cai-fyp/fingerDetection")
            {
              if(message == "activate")
              {
                handIn();
              }
              else if(message == "not activated")
              {
                handOut();
              }
              else if(message == "captured")
              {
                capturedNotification();
              }
            }
            else if (topic == "public/cai-fyp/")
            {
              

            }

          }




