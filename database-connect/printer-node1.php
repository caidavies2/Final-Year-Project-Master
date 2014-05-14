<?php

$curl = curl_init(); 
    $post_data = array('format' => 'json',
                       'apikey' => 'YOUR_API_KEY',
                       'provider' => 'PROVIDER_STRING',
                       'url' => $lurl );
    $api_url = 'http://tiny-url.info/api/v1/create';
    curl_setopt($curl, CURLOPT_URL, $api_url);
    curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($curl, CURLOPT_POST, 1);
    curl_setopt($curl, CURLOPT_POSTFIELDS, $post_data);
    $result = curl_exec($curl);
    curl_close($curl);

    echo $result;

?>