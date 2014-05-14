<?php

$url = "http://cl.ly/image/1p0S053T121L/Screen%20Shot%202014-04-30%20at%206.48.31%20PM.png";
$length = strlen($url);
$extension = substr($url, ($length-3), $length);
echo $extension;

echo pageTitle($url);
echo "<br>";
echo pageDescription($url);
echo "<br>";
echo tinyUrl($url);


function metaData($link, $type)
{

  //       // create curl resource
  // $ch = curl_init();

  //       // set url
  // curl_setopt($ch, CURLOPT_URL, $url);

  //       //return the transfer as a string
  // curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);



  //       // Alternatively, lie, and pretend to be a browser
  // curl_setopt($ch, CURLOPT_USERAGENT, 'Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)');

  //       // $output contains the output string
  // $output = curl_exec($ch);

  //       // close curl resource to free up system resources
  // curl_close($ch); 
  if($type == 'title')
  {
    $firstParagraphTag = strpos($output, "<div");
    $nextParagraphTag = strpos($output, "</div>");
    echo substr($output, $firstParagraphTag, $nextParagraphTag);
  }
  else if($type == 'description')
  {
        $firstParagraphTag = strpos($output, "<div");
        $nextParagraphTag = strpos($output, "</div>");
        echo substr($output, $firstParagraphTag, $nextParagraphTag);
  }

}

function pageTitle($page_url)
{
 
 $read_page=file_get_contents($page_url);
 preg_match("/<title.*?>[\n\r\s]*(.*)[\n\r\s]*<\/title>/", $read_page, $page_title);

if(pageExtension($url))
{
  pageExtension($extension);
}
 else if (isset($page_title[1]))
 {
  if ($page_title[1] == '')
  {
    return $page_url;
  }
  $page_title = $page_title[1];
  return trim($page_title);
}

else
{
    metaData($url, 'title');
}
}

function pageExtension($extension)
{

  if($extension == 'jpg' || $extension == 'png')
  {
    echo 'Image:';
  }
  return true;
}

function pageDescription($url)
{
  $tags = get_meta_tags($url);

  $description = $tags['description'];

  
  if(!description)
  {
      metaData($url, 'description');
  }
  else
  {
    return substr($description, 0, strpos($description, ' ', 50));
  }

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