<?php
$node = $_GET["node"];
$json = file_get_contents("storage/node-" . $node . ".json");
$data = json_decode($json, true); 
print_r($data);
?>