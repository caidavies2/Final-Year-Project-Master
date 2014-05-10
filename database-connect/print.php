<?php
$link = mysqli_connect("localhost", "root", "", "fyp");

/* check connection */
if (mysqli_connect_errno()) {
    printf("Connect failed: %s\n", mysqli_connect_error());
    exit();
}
if ($result = mysqli_query($link, "SELECT * FROM links")) {

    // $row_cnt = mysqli_fetch_assoc($result);

    // printf($row_cnt['nr']);

    // mysqli_free_result($result);

  while($row = $result->fetch_array())
  {
  echo $row['url'];
  echo "<br />";
  }
}
/* close connection */
mysqli_close($link);
?>