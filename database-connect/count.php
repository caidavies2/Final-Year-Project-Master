<?php
$node = $_GET["node"];
$json = file_get_contents("storage/node-" . $node . ".json");
$data = json_decode($json, true);	
$iCount = count($data);
echo "[$iCount]";
?>