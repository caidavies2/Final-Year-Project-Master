<?php

$url = "http://en.wikipedia.org/wiki/Audio_file_format";
$length = strlen($url);
$extension = substr($url, ($length-3), $length);
$data=file_get_contents($url); 
$meta = get_meta_tags($url);

if(checkExtension($extension) == true)
{
  echo "<br>";
}
else
{
  getTitle($data, $matches);
  echo "<br>";
}
getDescription($data, $matches);
tinyUrl($url);

function getTitle($data, $matches)
{
  if(preg_match("/<title>(.+)<\/title>/i", $data, $matches))
  {
    echo "$matches[1]";
  }
  else if(preg_match("/<h1>(.+)<\/h1>/i", $data, $matches))
  {
    echo "$matches[1]";
  }

  else
  {
    echo $meta['title'];
    // Put in code to 'link 3 etc' if no title or h1 tags.
  }

}

function getDescription($data, $matches)
{
  if(preg_match("/<p>(.+)<\/p>/i", $data, $matches))
  {
    echo "$matches[1]";
  }
  else if(preg_match("/<h2>(.+)<\/h2>/i", $data, $matches))
  {
    echo "$matches[1]";
  }
  else
  {
    $meta['description'];
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
    echo "Image:";
    return true;
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
    echo "Sound File:";
    return true;
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
  echo $result;
  echo "<br>";
}
?>