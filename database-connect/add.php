<?php
  // if (isSet($_POST["name"])) {
  $url = $_POST['name'];
date_default_timezone_set("Europe/London");
// $url = "www.google.com";

$node = $_GET["node"];
$json = file_get_contents("storage/node-" . $node . ".json");
$data = json_decode($json, true);
$iCount = count($data);
$addItemNumber = $iCount + 1;

// Adding new data:
$data[$addItemNumber] = array('Id' => $addItemNumber, 'Url' => get_tiny_url($url), 'Title' => getTitle($url), 'Description' => getMetaDescription($url), 'Date:' => date('D j M Y'), 'Time:' => date('h:i:s'));

// Writing modified data:
file_put_contents("storage/node-" . $node . ".json", json_encode($data, JSON_FORCE_OBJECT));

function getTitle($Url){
    $str = file_get_contents($Url);
    if(strlen($str)>0){
        preg_match("/\<title\>(.*)\<\/title\>/",$str,$title);
        return $title[0];
    }
}

//gets the data from a URL  
function get_tiny_url($url)  
{  
	$ch = curl_init();  
	$timeout = 5;  
	curl_setopt($ch,CURLOPT_URL,'http://tinyurl.com/api-create.php?url='.$url);  
	curl_setopt($ch,CURLOPT_RETURNTRANSFER,1);  
	curl_setopt($ch,CURLOPT_CONNECTTIMEOUT,$timeout);  
	$data = curl_exec($ch);  
	curl_close($ch);  
	return $data;  
}

function getMetaDescription($content) {
    $metaDescription = false;
    $metaDescriptionPatterns = array("/]*>/Ui", "/]*>/Ui");
    foreach ($metaDescriptionPatterns as $pattern) {
        if (preg_match($pattern, $content, $match))
             $metaDescription = $match[1];
             break;
    }
    return $metaDescription;
// }
}
?>