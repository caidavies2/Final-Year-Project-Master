$(document).ready(function(){

var testData = "http://www.amazon.co.uk";

  $('button').click(function(){
     $.ajax({
     url: 'add.php?node=2', //This is the current doc
     type: "POST",
     data: {name:testData},
     success: function(data){
         console.log(data);
     },
     error: function (xhr, ajaxOptions, thrownError) {
           console.log(xhr.status);
           console.log(xhr.responseText);
           console.log(thrownError);
       }
});
  });

});
