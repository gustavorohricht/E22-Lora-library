#include <Arduino.h>
#include <HardwareSerial.h>
#include <SPI.h>

#define PIN_RX 16   // Serial2 RX (connect this to the EBYTE Tx pin)
#define PIN_TX 17   // Serial2 TX pin (connect this to the EBYTE Rx pin)

#define PIN_M0 4   // D4 on the board (possibly pin 24)
#define PIN_M1 22 // D2 on the board (possibly called pin 22)
#define PIN_AX    // D15 on the board (possibly called pin 21)


// i recommend putting this code in a .h file and including it
// from both the receiver and sender modules
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

data_t Mystruct;

size_t FixedPointTransmit(data_t* TransmitData);
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
  Serial.println("Starting Reader");

//	pinMode(PIN_AX, INPUT);
	pinMode(PIN_M0, OUTPUT);
	pinMode(PIN_M1, OUTPUT);
  digitalWrite(PIN_M0, LOW);
	digitalWrite(PIN_M1,LOW);
  delay(50);

  cleanBuffer();
}

void loop() {
 
	  Mystruct.temperature = 32.4;
	  Mystruct.pressure = 101034;
	  Mystruct.altitude = 20.4;
	  Mystruct.ax = 3.45;
	  Mystruct.ay = 354.6;
	  Mystruct.az = 34.5;
	  Mystruct.gx = 180.5;
	  Mystruct.gy = 235.645;
	  Mystruct.gz = 23.56;
	  Mystruct.latitude = 8190381;
	  Mystruct.longitude = 2131283;
	  Mystruct.ejection = 1;
 
    size_t sent;
 
    sent = FixedPointTransmit(&Mystruct);
 
    Serial.println(sent);

}

void cleanBuffer() {

  while(Serial2.available() > 0) {  
    Serial2.read();
  }
}
size_t FixedPointTransmit(data_t* TransmitData) {

  size_t sent =  Serial2.write((uint8_t*)TransmitData, sizeof(*TransmitData));
  delay(100);
  cleanBuffer();
  return sent;
}
