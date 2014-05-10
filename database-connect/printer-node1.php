<?php
$link = mysql_connect("localhost", "", "");
mysql_select_db("fyp", $link);
$result = mysql_query("SELECT * FROM links", $link);
$url;
$num_rows = mysql_num_rows($result);

while($row = mysql_fetch_array($result)) {
  $url = $row['url'];
}

function getTitle($Url){
    $str = file_get_contents($Url);
    if(strlen($str)>0){
        preg_match("/\<title\>(.*)\<\/title\>/",$str,$title);
        return $title[1];
    }
    else
    {
    	return "false";
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
		
		echo getTitle("www.amazon.co.uk");
		echo get_tiny_url("www.amazon.co.uk");
?>

