<?php
$node = $_GET["node"];
$json = file_get_contents("storage/node-" . $node . ".json");
$data = json_decode($json, true); 


// print_array($data);

foreach ($data as $innerArray) {
    //  Check type
    if (is_array($innerArray)){
        //  Scan through inner loop
        foreach ($innerArray as $value) {
            echo $value;
            echo "<br/>";
        }
    }else{
        // one, two, three
        echo $innerArray;
    }
}


function print_array($aArray) {
// Print a nicely formatted array representation:
  echo '<pre>';
  print_r($aArray);
  echo '</pre>';
}
?>