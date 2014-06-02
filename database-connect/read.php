<head><meta http-equiv="Content-Type" content="text/html;charset=utf-8"><head>
<?php
header('content-type:text/html;charset=utf-8');
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

for ($row = 1; $row <= count($data); $row++)
{
    echo '[it' . $data[$row]["Id"] . ']';
    echo '[id]';
    echo $data[$row]["Id"];
    echo '[id]';
    echo '[t]';
    echo $data[$row]["Title"];
    echo '[t]';
    echo '[de]';
    echo $data[$row]["description"];
    echo '[de]';
    echo '[u]';
    echo str_replace("http://", "", $data[$row]["Url"]);
    echo '[u]';
    echo '[d]';
    echo $data[$row]["Date:"];
    echo '[d]';
    echo '[ti]';
    echo $data[$row]["Time:"];
    echo '[ti]';
    echo '[/it]';
}

echo "[end]";
echo "~";

unlink("storage/node-" . $node . ".json");

// print_array($data);

function print_array($aArray) {
// Print a nicely formatted array representation:
  echo '<pre>';
  print_r($aArray);
  echo '</pre>';
}
?>