#include <TinyGPS++.h>
#include <HardwareSerial.h>

TinyGPSPlus gps;
HardwareSerial GPSSerial(1);

#define GPS_RX 33
#define GPS_TX 32

HardwareSerial GSMSerial(2);

#define GSM_RX 17
#define GSM_TX 16

#define FIRE_BUTTON     25
#define MEDICAL_BUTTON  26
#define POLICE_BUTTON   27

const char* SERVER_IP = "104.21.11.232";
const char* HOST = "wispy-cell-9d4f.jessicalara0619.workers.dev";

const char* DEVICE_CODE = "ESP001";

void setup()
{
  Serial.begin(115200);

  GPSSerial.begin(
    9600,
    SERIAL_8N1,
    GPS_RX,
    GPS_TX
  );

  GSMSerial.begin(
    9600,
    SERIAL_8N1,
    GSM_RX,
    GSM_TX
  );

  pinMode(FIRE_BUTTON, INPUT_PULLUP);
  pinMode(MEDICAL_BUTTON, INPUT_PULLUP);
  pinMode(POLICE_BUTTON, INPUT_PULLUP);

  delay(2000);

  Serial.println();
  Serial.println("==============================");
  Serial.println("       iResQ");
  Serial.println("       ESP32 ALERT SYSTEM");
  Serial.println("==============================");
  Serial.println();

  Serial.println("GPS + SIM800L starting...");
  Serial.println("Press a button to send an alert.");
  Serial.println();

  initializeGSM();
}

void loop()
{
  while (GPSSerial.available())
  {
    gps.encode(GPSSerial.read());
  }

  if (digitalRead(FIRE_BUTTON) == LOW)
  {
    Serial.println();
    Serial.println("!!! FIRE BUTTON PRESSED !!!");

    sendAlert("fire");

    delay(1500);
  }

  if (digitalRead(MEDICAL_BUTTON) == LOW)
  {
    Serial.println();
    Serial.println("!!! MEDICAL BUTTON PRESSED !!!");

    sendAlert("medical");

    delay(1500);
  }

  if (digitalRead(POLICE_BUTTON) == LOW)
  {
    Serial.println();
    Serial.println("!!! POLICE BUTTON PRESSED !!!");

    sendAlert("police");

    delay(1500);
  }
}

bool initializeGSM()
{
  Serial.println("[GSM] Initializing...");

  GSMSerial.println("AT");
  delay(500);

  GSMSerial.println("AT+CGATT=1");
  delay(1000);

  GSMSerial.println("AT+CIPSHUT");
  delay(1500);

  GSMSerial.println("AT+CSTT=\"internet\",\"\",\"\"");
  delay(1500);

  GSMSerial.println("AT+CIICR");
  delay(5000);

  GSMSerial.println("AT+CIFSR");
  delay(2000);

  Serial.println("[GSM] Initialization complete.");
  Serial.println();

  return true;
}

bool sendAlert(String alertType)
{
  Serial.println("[GPS] Looking for GPS fix...");

  unsigned long startTime = millis();

  while (millis() - startTime < 30000)
  {
    while (GPSSerial.available())
    {
      gps.encode(GPSSerial.read());
    }

    if (gps.location.isValid() &&
        gps.location.age() < 5000 &&
        gps.satellites.isValid() &&
        gps.satellites.value() >= 3)
    {
      break;
    }

    delay(100);
  }

  if (!gps.location.isValid())
  {
    Serial.println("[GPS] ERROR: No GPS fix.");
    Serial.println("Alert NOT sent.");
    return false;
  }

  double latitude = gps.location.lat();
  double longitude = gps.location.lng();

  Serial.println("[GPS] FIXED");
  Serial.print("[GPS] Latitude : ");
  Serial.println(latitude, 6);
  Serial.print("[GPS] Longitude: ");
  Serial.println(longitude, 6);

  Serial.println();
  Serial.println("[GSM] Preparing connection...");

  GSMSerial.println("AT+CIPSHUT");
  delay(1500);

  GSMSerial.println("AT+CSTT=\"internet\",\"\",\"\"");
  delay(1500);

  GSMSerial.println("AT+CIICR");
  delay(5000);

  GSMSerial.println("AT+CIFSR");
  delay(2000);

  Serial.println("[HTTP] Connecting to Cloudflare...");

  GSMSerial.print("AT+CIPSTART=\"TCP\",\"");
  GSMSerial.print(SERVER_IP);
  GSMSerial.println("\",\"80\"");

  unsigned long connectStart = millis();
  bool connected = false;

  while (millis() - connectStart < 15000)
  {
    if (GSMSerial.available())
    {
      String response = GSMSerial.readString();
      Serial.print(response);

      if (response.indexOf("CONNECT OK") >= 0)
      {
        connected = true;
        break;
      }
    }

    delay(100);
  }

  if (!connected)
  {
    Serial.println("[HTTP] ERROR: TCP connection failed.");
    return false;
  }

  String body =
    "device_code=" + String(DEVICE_CODE) +
    "&alert_type=" + alertType +
    "&latitude=" + String(latitude, 6) +
    "&longitude=" + String(longitude, 6);

  Serial.println();
  Serial.println("[HTTP] Sending alert...");
  Serial.println("[HTTP] Data:");
  Serial.println(body);

  GSMSerial.println("AT+CIPSEND");

  unsigned long sendStart = millis();
  bool promptReceived = false;

  while (millis() - sendStart < 5000)
  {
    if (GSMSerial.available())
    {
      String response = GSMSerial.readString();
      Serial.print(response);

      if (response.indexOf(">") >= 0)
      {
        promptReceived = true;
        break;
      }
    }

    delay(100);
  }

  if (!promptReceived)
  {
    Serial.println("[HTTP] ERROR: No CIPSEND prompt.");
    GSMSerial.println("AT+CIPCLOSE");
    return false;
  }

  String request =
    "POST / HTTP/1.1\r\n"
    "Host: " + String(HOST) + "\r\n"
    "Content-Type: application/x-www-form-urlencoded\r\n"
    "Content-Length: " + String(body.length()) + "\r\n"
    "Connection: close\r\n"
    "\r\n" +
    body;

  Serial.println("[HTTP] Request sent.");
  Serial.println(request);

  GSMSerial.print(request);
  GSMSerial.write(26);

  unsigned long responseStart = millis();
  bool serverAccepted = false;

  while (millis() - responseStart < 15000)
  {
    if (GSMSerial.available())
    {
      String response = GSMSerial.readString();
      Serial.print(response);

      if (response.indexOf("200 OK") >= 0)
      {
        serverAccepted = true;
      }
    }

    delay(100);
  }

  GSMSerial.println("AT+CIPCLOSE");
  delay(1000);

  if (serverAccepted)
  {
    Serial.println();
    Serial.println("==============================");
    Serial.println("      SERVER ACCEPTED ALERT");
    Serial.println("==============================");

    return true;
  }

  Serial.println();
  Serial.println("[HTTP] ERROR: Server did not return 200 OK.");

  return false;
}