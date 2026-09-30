#define BLYNK_TEMPLATE_ID "TMPL30I3AvUb4"
#define BLYNK_TEMPLATE_NAME "Aquaproj"
#define BLYNK_AUTH_TOKEN "5ed0j-XpdwEVdzraVJLXrHRKRzbZ1v0I"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <HTTPClient.h>

// 🔹 WiFi credentials
char ssid[] = "*****";
char pass[] = "*********";

// 🔹 DHT
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// 🔹 Pins
#define TURBIDITY_PIN 34
#define PH_PIN 35
#define MOISTURE_PIN 32
#define BUZZER 2
#define RELAY1 25
#define RELAY2 26

BlynkTimer timer;

// 🔹 Thresholds
int turbidityThreshold = 50;   // adjust after calibration
int moistureThreshold = 3;     // low water level
float tempThreshold = 33;    // high temperature

// 🔹 Averaging function
int readStable(int pin) {
  int sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += analogRead(pin);
    delay(5);
  }
  return sum / 10;
}

// 🔹 Send data to Blynk + Server
void sendData() {

  int turbidity = readStable(TURBIDITY_PIN);
  turbidity = abs (map(turbidity, 4000, 0, 0, 100));
  delay(50);

  int phRaw = readStable(PH_PIN);
  delay(50);

  int moisture = readStable(MOISTURE_PIN);
  moisture = abs (map(moisture, 4000, 0, 0, 10));

  // 🔹 Convert to voltage
  float voltage = phRaw * (3.3 / 4095.0);

  // 🔹 pH formula
  float pHValue = 7 + ((2.5 - voltage) / 0.18);

  float temp = dht.readTemperature();

  // 🔹 Send to Blynk
  Blynk.virtualWrite(V0, turbidity);
  Blynk.virtualWrite(V1, pHValue);
  Blynk.virtualWrite(V2, moisture);
  Blynk.virtualWrite(V3, temp);

  // 🔹 Serial Monitor
  Serial.println("---- Blynk Data ----");
  Serial.print("Turbidity: "); Serial.println(turbidity);
  Serial.print("pH: "); Serial.println(pHValue);
  Serial.print("Moisture: "); Serial.println(moisture);
  Serial.print("Temp: "); Serial.println(temp);

  // 🔔 Buzzer Logic
  if (turbidity > turbidityThreshold || moisture > moistureThreshold || temp > tempThreshold) {
    digitalWrite(BUZZER, HIGH);
    Blynk.logEvent("alert", "Abnormal value detected, pls check in app or web interface");
  } else {
    digitalWrite(BUZZER, LOW);
  }

  // 🌐 Send data to server
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = "http://myiot.co.in/klaqua/update.php?";
    url += "turbidity=" + String(turbidity);
    url += "&ph=" + String(pHValue);
    url += "&moisture=" + String(moisture);
    url += "&temp=" + String(temp);

    http.begin(url);
    int httpResponseCode = http.GET();

    Serial.print("HTTP Response: ");
    Serial.println(httpResponseCode);

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

// 🔹 Relay 1 Control from App
BLYNK_WRITE(V4) {
  int value = param.asInt();
  digitalWrite(RELAY1, value);
}

// 🔹 Relay 2 Control from App
BLYNK_WRITE(V5) {
  int value = param.asInt();
  digitalWrite(RELAY2, value);
}

void setup() {
  Serial.begin(9600);

  dht.begin();

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  delay(1000);
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(BUZZER, LOW);

  // 🔹 ADC settings
  analogReadResolution(12);
  analogSetPinAttenuation(TURBIDITY_PIN, ADC_11db);
  analogSetPinAttenuation(PH_PIN, ADC_11db);
  analogSetPinAttenuation(MOISTURE_PIN, ADC_11db);

  // 🔹 Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // 🔹 Send data every 2 seconds
  timer.setInterval(2000L, sendData);
}

void loop() {
  Blynk.run();
  timer.run();
}
