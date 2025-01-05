<?php
require("config.php");
if (!empty($_GET)) {
    $body_temp = $_GET['sensor'];
    $room_temp = $_GET['sensor'];
    $humidity = $_GET['sensor'];
    $pulse_rate = $_GET['sensor'];
    $oxy_lvl = $_GET['sensor'];
    $hashid = md5(microtime());
    $time = date('h:i a');
    $date = date('d-m-Y');
    $sql = mysqli_query($con, "INSERT INTO data(hashid,body_temp, room_temp, humidity, pulse_rate, oxy_lvl, distance, date, time) VALUES('$hashid','$body_temp', '$room_temp', '$humidity','$pulse_rate','$oxy_lvl','$date','$time')");
    if ($sql) {
        echo "Saved!";
    } else {
        echo "Error";
    }
}
/*
// Send an SMS using Twilio's REST API and PHP

// Required if your environment does not handle autoloading
require '"C:\xampp\htdocs\Sensor-ESP8266-PHP-API-main\twilio-php-main\src\Twilio\autoload.php"';

// Your Account SID and Auth Token from console.twilio.com
$sid = "ACef1ce576996f6bf4e9141a5f2eb45bdd";
$token = "01a818a38c41b22b8734ef7aa2b78f4c";
$client = new Twilio\Rest\Client($sid, $token);

// Use the Client to make requests to the Twilio REST API
$client->messages->create(
    // The number you'd like to send the message to
    '+919574456782',
    [
        // A Twilio phone number you purchased at https://console.twilio.com
        'from' => '+12187576648',
        // The body of the text message you'd like to send
        'body' => "Hey Jenny! Good luck on the bar exam!"
    ]
);
*/
?>