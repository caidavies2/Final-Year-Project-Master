<?php
header('content-type:text/html;charset=utf-8');
if (isSet($_POST["name"])) {
  $url = $_POST['name'];
}
else
{
  $url = "";
}
date_default_timezone_set("Europe/London");
$node = $_GET["node"];
$json = file_get_contents("storage/node-" . $node . ".json");
$dataEncoded = json_decode($json, true);
$iCount = count($dataEncoded);
$addItemNumber = $iCount + 1;
$length = strlen($url);
$extension = substr($url, ($length-3), $length);

$length = strlen($url);
$extension = substr($url, ($length-3), $length);
$data =file_get_contents($url); 
$meta = get_meta_tags($url);
$description;
$compressedURL;
$title = getTitle($data, $matches, $extension);
$title = substr($title, 0, strpos($title, ' ', 15));
$description = getDescription($data, $matches);
$description = substr($description, 0, strpos($description, ' ', 15)) . "...";
$tinyURL = tinyUrl($url);
// echo $title;

echo firstTitle($data);
echo "<br>";
echo $description;
echo "<br>";
echo $tinyURL;

// Uncomment

// echo $addItemNumber;

// // Adding new data:
// $dataEncoded[$addItemNumber] = array('Id' => $addItemNumber, 'Url' => $tinyURL, 'Title' => $title, 'description' => $description, 'Date:' => date('D j M Y'), 'Time:' => date('h:i:s'));

// // Writing modified data:
// file_put_contents("storage/node-" . $node . ".json", json_encode($dataEncoded, JSON_FORCE_OBJECT));

// End of uncomment

function getTitle($data, $matches, $extension)
{
  if(checkExtension($extension) == true)
  {
    return checkExtension($extension);
  }

  // else if(page_title($data))
  // {
  //   return page_title($data);
  // }

  // else if ($meta);
  // {
  //   return $meta['title'];
  // }

  else if(preg_match("/<title>(.+)<\/title>/i", $data, $matches))
  {
    return "$matches[1]";
  }
  else if(preg_match("/<h1>(.+)<\/h1>/i", $data, $matches))
  {
    return "$matches[1]";
  }

  else
  {
    return "";
  }

}

function getDescription($data, $matches)
{
  if(preg_match("/<p>(.+)<\/p>/i", $data, $matches))
  {
    return "$matches[1]";
  }
  else if(preg_match("/<h2>(.+)<\/h2>/i", $data, $matches))
  {
    return "$matches[1]";
  }
  else
  {
   return $meta['description'];
  }
}

function checkExtension($extension)
{
  if
    (
      $extension == "jpg"
      || $extension == "jpeg"
      || $extension == "png"
      || $extension == "gif"
      || $extension == "bmp"
      || $extension == "tiff"
      || $extension == "svg"
      || $extension == "raw") 
  {
    return "Image:";
  }
  
  else if (
    $extension == "mp3" 
    || $extension == "wav" 
    || $extension == "3gp" 
    || $extension == "ogg"
    || $extension == "aiff"
    || $extension == "m4a"
    )
  {
    return "Sound File:";
  }
  else {return false;}
}
function tinyUrl($lurl)
{

  $curl = curl_init(); 
  $post_data = array('format' => 'text',
   'apikey' => 'B969BC591B9AIEAA0766',
   'provider' => 'tinyurl_com',
   'url' => $lurl );
  $api_url = 'http://tiny-url.info/api/v1/create';
  curl_setopt($curl, CURLOPT_URL, $api_url);
  curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
  curl_setopt($curl, CURLOPT_POST, 1);
  curl_setopt($curl, CURLOPT_POSTFIELDS, $post_data);
  $result = curl_exec($curl);
  curl_close($curl);
  return $result;
}

function firstTitle($data) {

    if (!$data) return null;

    $matches = array();

    if (preg_match('/<title>(.*?)<\/title>/', $data, $matches)) {
        return $matches[1];
    }
    else {
        return null;
    }
}
?>