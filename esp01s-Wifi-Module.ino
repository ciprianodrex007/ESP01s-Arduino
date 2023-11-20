#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST " "
#define FIREBASE_AUTH " "
// WiFi credentials
#define WIFI_SSID "Your-Wifi-Name"
#define WIFI_PASSWORD "Your-Password"

// data var
int temperature;
int humidity;
int ppm;
int lux;
float pH;
float ec;
float waterTemp;
float waterLevel;
// switch var
int lightSwitch;
int ppmUp = 0;
int ppmDown = 0;
int pHUp = 0;
int pHDown = 0;
int waterSwitch;

void setup() {
  Serial.begin(115200);
  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  connectToWiFi();
}

void loop() {
    getDataInFirebase();
    sendDataToFirebase();
  reconnectWiFi();
  delay(1700);
}

void connectToWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("Connected to Wi-Fi");
}

bool reconnectWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    // Reconnect to Wi-Fi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
      delay(500);
      attempts++;
    }
    return WiFi.status() == WL_CONNECTED;
  }
  return false;
}


void sendDataToFirebase() {
  if (Serial.available() > 0) {
  String dataFromArduino = Serial.readStringUntil('\n');

  if (parseArduinoData(dataFromArduino)) {
  // Send simulated sensor data to Firebase
  Firebase.setInt("sensor/temperature", temperature);
  Firebase.setInt("sensor/humidity", humidity);
  Firebase.setInt("sensor/lux", lux);
  Firebase.setInt("sensor/ppm", ppm);
  Firebase.setFloat("sensor/pH", pH);
  Firebase.setFloat("sensor/ec", ec);
  Firebase.setFloat("sensor/waterTemp", waterTemp);
  Firebase.setFloat("sensor/waterLevel", waterLevel);
  }
  else {
    Serial.println("failed to send");
  }
  }
}

bool parseArduinoData(String data) {
  return sscanf(data.c_str(), "%d,%d,%d,%d,%f,%f,%f,%f",
                &temperature, &humidity, &lux, &ppm, &pH, &ec, &waterTemp, &waterLevel) == 8;
}


void getDataInFirebase() {
  
  lightSwitch = Firebase.getInt("switch/lightSwitch");
  ppmUp = Firebase.getInt("switch/ppmUp");
  ppmDown = Firebase.getInt("switch/ppmDown");
  pHUp = Firebase.getInt("switch/pHUp");
  pHDown = Firebase.getInt("switch/pHDown");
  waterSwitch = Firebase.getInt("switch/waterSwitch");
  sendDataToArduino();
}

void sendDataToArduino() {
  Serial.print(lightSwitch);
  Serial.print(",");
  Serial.print(ppmUp);
  Serial.print(",");
  Serial.print(ppmDown);
  Serial.print(",");
  Serial.print(pHUp);
  Serial.print(",");
  Serial.print(pHDown);
  Serial.print(",");
  Serial.print(waterSwitch);
  Serial.println();
}
