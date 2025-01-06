<?php
// Include the Twilio library
require 'C:\xampp\htdocs\Sensor-ESP8266-PHP-API-main\twilio-php-main\src\Twilio\autoload.php';

// Twilio credentials
$sid = "ACef1ce576996f6bf4e9141a5f2eb45bdd";
$token = "73e8609e6924ff95819d99dc79870a17";

// Function to send an SMS
function sendSMS($sid, $token, $to, $from, $message)
{
    try {
        $client = new Twilio\Rest\Client($sid, $token);
        $client->messages->create(
            $to,
            [
                'from' => $from,
                'body' => $message
            ]
        );
        echo "<script>console.log('SMS sent successfully!');</script>";
    } catch (Exception $e) {
        echo "<script>console.log('Error sending SMS: " . $e->getMessage() . "');</script>";
    }
}

// Function to check if a message has already been sent for a record
function isSMSSent($con, $recordId)
{
    $query = "SELECT sms_sent FROM data WHERE hashid = $recordId";
    $result = mysqli_query($con, $query);
    $row = mysqli_fetch_assoc($result);
    return $row['sms_sent'] == 1;
}

// Function to mark a record as SMS sent
function markSMSSent($con, $recordId)
{
    $query = "UPDATE data SET sms_sent = 1 WHERE hashid = $recordId";
    mysqli_query($con, $query);
}
?>