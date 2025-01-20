#include <Arduino.h>
#include <HardwareSerial.h>
#include <SPI.h>

/*
WARNING: IF USING AN ESP32
DO NOT USE THE PIN NUMBERS PRINTED ON THE BOARD
YOU MUST USE THE ACTUAL GPIO NUMBER
*/

#define PIN_RX 16   // Serial2 RX (connect this to the EBYTE Tx pin)
#define PIN_TX 17   // Serial2 TX pin (connect this to the EBYTE Rx pin)

#define PIN_M0 4   // D4 on the board (possibly pin 24)
#define PIN_M1 22 // D2 on the board (possibly called pin 22)
#define PIN_AX    // D15 on the board (possibly called pin 21)

#pragma pack(1)
typedef struct Data {
  //BMP280 data
    float temperature;
    float pressure;
    float altitude;

  //MPU6050 data
    float ax;
    float ay;
    float az;
    float gx;
    float gy;
    float gz;
    
    //GPS Data
    double latitude;
    double longitude;
  //Ejection
    int ejection = 0; //If true, the pyrowire is ejected and then the data is sent to the SD card
}data_t;
#pragma pack()

data_t Mydata;

void cleanBuffer();

void setup() {

  Serial.begin(9600);

  while(!Serial){
    delay(100);
  }
  while(Serial.available()){
    Serial.read();
  }

  Serial2.begin(9600);
  
  while(!Serial2){
    delay(100);
  }

//	pinMode(PIN_AX, INPUT);
	pinMode(PIN_M0, OUTPUT);
	pinMode(PIN_M1, OUTPUT);
  digitalWrite(PIN_M0, LOW);
	digitalWrite(PIN_M1,LOW);
  delay(50);
 
  cleanBuffer();
}

void loop() {

   Serial2.readBytes((uint8_t*)&Mydata,sizeof(Mydata));
   Serial.println(Mydata.altitude);
   Serial.println(Mydata.pressure);
   Serial.println(Mydata.temperature);
   Serial.println(Mydata.latitude);
   Serial.println(Mydata.longitude);
   Serial.println(Mydata.ejection);
   Serial.println(Mydata.ax);
   Serial.println(Mydata.ay);
   Serial.println(Mydata.az);
   Serial.println(Mydata.gx);
   Serial.println(Mydata.gy);
   Serial.println(Mydata.gz);

   cleanBuffer();

}


void cleanBuffer() {

  while(Serial2.available() > 0) {  
    Serial2.read();
  }
}
