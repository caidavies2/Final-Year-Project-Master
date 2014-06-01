<?php
$node = $_GET["node"];
$json = file_get_contents("storage/node-" . $node . ".json");
$data = json_decode($json, true); 

// foreach ($data as $innerArray) {
//     //  Check type
//     if (is_array($innerArray)){
//         //  Scan through inner loop
//         foreach ($innerArray as $value) {
//             print_r($innerArray);
//             echo $value;
//             echo "<br/>";
//         }
//     }else{
//         // one, two, three
//         echo $innerArray;
//     }
// }

print_r($data);

// print_array($data);

function print_array($aArray) {
// Print a nicely formatted array representation:
  echo '<pre>';
  print_r($aArray);
  echo '</pre>';
}
?>