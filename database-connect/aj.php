<?php
if (isSet($_POST["name"])) {
$userAnswer = $_POST['name'];
$host = "localhost";
$user = "root";
$pass = "";
$databaseName = "fyp";
$tableName = "links";

$con = mysql_connect($host,$user,$pass);
if(! $con)
{
  echo "could not connect, sorreh";
}
else
{
  echo "connected + ";
  echo json_encode($userAnswer);
  $db_select = mysql_select_db('fyp', $con);
   if (!$db_select) {
       die("Database selection failed:: " . mysql_error());
   }
}
// $sql = "INSERT INTO `links`(`url`) VALUES ('testingtesting')";
$sql = "INSERT INTO `links`(`url`) VALUES('" . $userAnswer . "')";
// $dbs = mysql_select_db($databaseName, $con);

$retval = mysql_query( $sql, $con );
if(! $retval )
{
  die('Could not enter data: ' . mysql_error());
}

$retcount = msql_query($sqlCount, $con);
if(! $retcount)
{
  die('could not count data: ' . mysql_error());
}

else
{
  echo 'entered data';
}
// $sql = 'INSERT INTO `links`(`url`) VALUES' . $userAnswer;
echo json_encode($userAnswer);
}
?>
