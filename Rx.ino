#define BLYNK_TEMPLATE_ID "TMPL3wtST4Sf9"
#define BLYNK_TEMPLATE_NAME "Project"
#define BLYNK_AUTH_TOKEN "bqay83F-zVGtYoxbhQWiLR7VgMrIJs3A"
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

char ssid[] = "Ace";
char pass[] = "password";

BlynkTimer timer;
RF24 radio(4, 5); // CE, CSN

struct sensor{
  float turbid;
  int   temp;
};
sensor Mdata;

const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();
}

void loop() {
  Blynk.run();
  timer.run();
  if (radio.available()) {
    radio.read(&Mdata, sizeof(Mdata));
    Serial.println(Mdata.temp);
    Serial.println(Mdata.turbid);
  }
  Blynk.virtualWrite(V0,Mdata.turbid);
  Blynk.virtualWrite(V1,Mdata.temp);
}