<?php

header("Content-Type: application/json");

include("../config/db.php");

$response = [];

$response["method"] = $_SERVER['REQUEST_METHOD'];
$response["post"] = $_POST;

if ($_SERVER['REQUEST_METHOD'] != "POST") {

    echo json_encode([
        "success" => false,
        "message" => "POST request required."
    ]);

    exit();
}

$device_code = $_POST['device_code'] ?? '';
$alert_type  = $_POST['alert_type'] ?? '';
$latitude    = $_POST['latitude'] ?? '0';
$longitude   = $_POST['longitude'] ?? '0';

if ($device_code == "" || $alert_type == "") {

    echo json_encode([
        "success" => false,
        "message" => "Missing device or alert type.",
        "received" => $_POST
    ]);

    exit();
}

$device_code = mysqli_real_escape_string($conn, $device_code);
$alert_type  = mysqli_real_escape_string($conn, $alert_type);
$latitude    = mysqli_real_escape_string($conn, $latitude);
$longitude   = mysqli_real_escape_string($conn, $longitude);

$getDevice = mysqli_query($conn, "
    SELECT *
    FROM devices
    WHERE device_code='$device_code'
    LIMIT 1
");

if (!$getDevice) {

    echo json_encode([
        "success" => false,
        "message" => "Device query failed.",
        "error" => mysqli_error($conn)
    ]);

    exit();
}

if (mysqli_num_rows($getDevice) == 0) {

    echo json_encode([
        "success" => false,
        "message" => "Device not registered.",
        "device_code" => $device_code
    ]);

    exit();
}

$device = mysqli_fetch_assoc($getDevice);

$device_id = $device['device_id'];

if ($alert_type == "fire") {
    $department = "Fire";
}
else if ($alert_type == "medical") {
    $department = "Medical";
}
else if ($alert_type == "police") {
    $department = "Police";
}
else {
    $department = "Emergency";
}

$sql = "
INSERT INTO alerts
(
    device_id,
    alert_type,
    department,
    latitude,
    longitude,
    status
)
VALUES
(
    '$device_id',
    '$alert_type',
    '$department',
    '$latitude',
    '$longitude',
    'pending'
)
";

$result = mysqli_query($conn, $sql);

if ($result) {

    echo json_encode([
        "success" => true,
        "message" => "Alert received successfully.",
        "device_id" => $device_id,
        "device_code" => $device_code,
        "alert_type" => $alert_type,
        "department" => $department,
        "latitude" => $latitude,
        "longitude" => $longitude
    ]);

}
else {

    echo json_encode([
        "success" => false,
        "message" => "Alert insert failed.",
        "error" => mysqli_error($conn)
    ]);
}

?>