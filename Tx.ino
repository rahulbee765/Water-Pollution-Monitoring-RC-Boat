#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";
int TSP = A0;
float TSV;

struct sensor{
  float turbid;
  int   temp;
};
sensor M;
void setup() {
  sensors.begin();
  Serial.begin(9600); // for debugging purposes
  pinMode(TSP, INPUT);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
}

void loop() {
  TSV = 0; 
  for(int i=0; i<500; i++){
    TSV +=analogRead(TSP);
  }
  TSV = TSV/500;    
  Serial.print(TSV);
  Serial.println(" V  ");
  float turbidity = map(TSV, 0,700, 100, 0);
  Serial.print(turbidity);
  Serial.println("tur ");
  sensors.requestTemperatures();
  M.turbid=turbidity;//45;//
  M.temp=sensors.getTempCByIndex(0);//89;//
  Serial.print("Temperature: ");
  Serial.print(M.temp);
  radio.write(&M, sizeof(M));
}