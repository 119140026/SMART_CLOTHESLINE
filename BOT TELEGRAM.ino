#include <Servo.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

const char* ssid = "Maniac Nolep";
const char* password = "12345678907";

#define BOTtoken "5988302742:AAELqs3ksw-Hutvvkf1ei_223IN_tUZiYw4"

#define CHAT_ID "1861690898"

#define trig 5
#define echo 18
unsigned long duration;
unsigned int distance;

const int ledPin = 12;
const int buzzerPin = 13;
const int ldrPin = 33;
const int hujand = 14;


Servo servo1;
Servo servo2;
String jarak, hujan, cuaca, status="asyroful";

int pos1 = 90;
int pos2 = 90;
int kirimUltrasonic = 0;

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;



void setup() {
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
#ifdef ESP32
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // Add root certificate for api.telegram.org
#endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  servo1.attach(27);  //rusak
  servo2.attach(26);  //baru
  Serial.begin(115200);
  servo1.write(90);  //Posisi awal servo1
  servo2.write(90);

  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ldrPin, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  bot.sendMessage(CHAT_ID, "Welcome, To SMARTCLOTHESLINE", "");
}

void loop() {
  
  //Sensor Cahaya
  int ldrStatus = analogRead(ldrPin);

  if (digitalRead(hujand) == 1 && ldrStatus >= 500) {

    digitalWrite(buzzerPin, HIGH);
    digitalWrite(ledPin, HIGH);
    delay(100);

    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
    delay(100);

    cuaca = "Cuaca Mendung";
    String data = (String)cuaca;
    Serial.println(data);
    if (status != data) {
      bot.sendMessage(CHAT_ID, "Cuaca Mendung", "");
      status = data;
    }

  } else {

    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
    //Serial.println("CERAH");
  }

  //Sensor Hujan'
  if (digitalRead(hujand) == 0 && ldrStatus >= 500) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(ledPin, HIGH);
    delay(100);

    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
    delay(100);
    tutup();

    hujan = "Peringatan Hujan";
    String data = (String)hujan;
    Serial.println(data);
    if (status != data) {
      bot.sendMessage(CHAT_ID, "Peringatan Hujan", "");
      status = data;
    }
  } else {
    servo1.write(90);
    servo2.write(90);
    //Serial.println("CERAH");
  }

  //Sensor Ultrasonik
  digitalWrite(trig, LOW);
  delayMicroseconds(5);
  digitalWrite(trig, HIGH);
  delayMicroseconds(5);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = duration / 58.2;
  if (distance <= 40) {
    if (kirimUltrasonic == 0) {
      digitalWrite(ledPin, HIGH);
      jarak = "Ada pakaian jatuh";
      String data = (String)jarak;
      Serial.println(data);
      if (status != data) {
        bot.sendMessage(CHAT_ID, "Ada Pakaian Jatuh", "");
        status = data;
      }
    } else {
      digitalWrite(ledPin, LOW);
    }
    kirimUltrasonic++;
  }

  else if (distance > 40) {
    digitalWrite(ledPin, LOW);
    kirimUltrasonic = 0;
    jarak = "";
  }
}

void tutup() {
  for (int i = 0; i < 90; i++) {
    pos1++;
    servo1.write(pos1);
    pos2--;
    servo2.write(pos2);
    delay(15);
  }
}

