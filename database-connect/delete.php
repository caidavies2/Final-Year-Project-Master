<?php
$node = $_GET["node"];
unlink("storage/node-" . $node . ".json");
?>

