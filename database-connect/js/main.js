$(document).ready(function(){

var testData = "test sosd";

  $('button').click(function(){
      $.ajax({
     url: 'add.php?node=3', //This is the current doc
     type: "POST",
     data: {url:testData},
     success: function(data){
         console.log(data);
     }
});

  });

});
