<?php
$link = mysql_connect("localhost", "root", "");
mysql_select_db("fyp", $link);
$result = mysql_query("TRUNCATE TABLE links", $link);
$num_rows = mysql_num_rows($result);
echo "[$num_rows]";
?>

