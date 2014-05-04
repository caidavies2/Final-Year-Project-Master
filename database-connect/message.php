<?php

$host = "localhost";
$user = "root";
$pass = "";
$databaseName = "fyp";
$tableName = "links";

$con = mysql_connect($host,$user,$pass);
$dbs = mysql_select_db($databaseName, $con);

$result = mysql_query("SELECT * FROM $tableName");
$data = array();

while ($row = mysql_fetch_array($result))
{
echo $row['id'];
echo "<br />";
echo $row['url'];
}
?>
