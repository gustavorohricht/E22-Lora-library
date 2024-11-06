
//Developed by Gustavo Rohricht Machado
//Code to set the E22 transceiver parameters
//It should be used to configure the transceiver in order to send and receive data

#include <HardwareSerial.h>
// Define the pins for software serial communication with the LoRa module
#define LORA_RX_PIN 16 // ESP32 pin connected to the LoRa module's TX pin
#define LORA_TX_PIN 17 // ESP32 pin connected to the LoRa module's RX pin
#define M0_PIN 4 //ESP32 pin connected to the LoRa module's M0 pin
#define M1_PIN 22 //ESP32 pin connected to the LoRa module's M1 pin

#define PIN_RECOVER 50 //Define a delay to let the pins set up, mainly M0 and M1 which need to be defined to specific values in order for configuration to work

#define PERMANENT 0xC0 //Saves the parameters to the module permanently
#define READ 0xC1 //Command to read the parameters from the module
#define TEMPORARY 0xC2 //Saves the parameters to the module temporarily, the parameters will be lost after power down

//REGISTERS
#define ADDH_REG 0x00
#define ADDL_REG 0x01
#define NETID_REG 0x02
#define REG0 0x03
#define REG1 0x04
#define CHANNEL_REG 0x05
#define REG3 0x06
#define CRYPT_H_REG 0x07
#define CRYPT_L_REG 0x08 

// Define the baud rate for serial communication
#define LORA_BAUD_RATE 9600

void WriteToRegister(uint8_t reg, uint8_t val);
void ReadRegister(uint8_t reg, uint8_t *val);
void PrintRegisterValue(uint8_t reg);

uint8_t loraRead[4];
uint8_t REG1_val = 0b00100000; //Datasheet page 15, this register should not be changed in general, do it only in very special cases
uint8_t REG3_val = 0b00000011;//Default value of the REG3 register


void setup() {
  // Initialize the serial monitor

  Serial.begin(9600);
  delay(PIN_RECOVER);
 
  // Initialize the software serial for LoRa communication
  Serial2.begin(9600);
  delay(PIN_RECOVER);

  // Configure M0 and M1 pins
  pinMode(M0_PIN, OUTPUT);
  pinMode(M1_PIN, OUTPUT);

  // Setting to Config Mode, datasheet page 12
  digitalWrite(M0_PIN, LOW);
  digitalWrite(M1_PIN, HIGH);
  delay(PIN_RECOVER);
  
  //Cleans the serial buffer right before writing/reading the registers
  //This is necessary in order to avoid reading/writing wrong values
  while(Serial2.available()) {
    Serial2.read();
  }

 /*
    Down there random values are set for testing purposes, change them if you want
    Note that all of the registers are 8 bit registers, so you can only use numbers between 0 and 255
    Setting any other value result might result in an error and undefined behavior of the transceiver

    Before turning on the transceiver, DO NOT FORGET TO PLUG THE ANTENNA IN, otherwise it might damage the transceiver permanently
    If you are using a microcontroller with 5V logic level, use resistors 4.7k-10k Ohms in the TX and AUX pins at least as says the datasheet in page 7
    ESP32 uses 3.3V logic level, so no resistors are needed
    
    Preferably plug the ground wire first, the E22 is an ESD device, so it could possibly be damaged by eletrostatic discharges
 */

  //Setting ADDH register
  //WriteToRegister(ADDH_REG, 0xFF); //Writing 0xFF to the ADDH register
  //Setting ADDL register
  //WriteToRegister(ADDL_REG, 0xFF); //Writing 0xFF to the ADDL register
  //Setting NETID register
  // WriteToRegister(NETID_REG, 0x45); //Writing 0x45 to the NETID register
  //Setting CHAN register
  //WriteToRegister(CHANNEL_REG, 65); //Setting channel 65 which is 915MHz channel

  /*
  The registers above are the most important ones to configure the way you configure them will depend on your
  transmission mode. I am using the default broadcast mode.

  In broadcast mode, the ADDH and ADDL are not important except that either the sender or the receiver must be 0xFFFF
  That is, ADDH 0xFF and ADDL 0xFF (datasheet page 8).

  However, the NETID and CHANNEL registers should be the same for both modules in order to be able to communicate
  The channels range from 0-80, and NETID ranges from 0-255 (0x00-0xFF). set them as your requirements

  */
  
  //Setting REG1 register
  //WriteToRegister(REG1,REG1_val);
  //Setting REG3 register
  //WriteToRegister(REG3, REG3_val);

  //Setting CRYPT_H and CRYPT_L registers they are for encryption of the data that is sent and write only registers, do not use them for reading
  //As it is not supported for security reasons as per datasheet page 18

  //These keys are used for message encryption, it is important that both devices have the same key otherwise they will not be able to communicate
  //If you are unsure about the keys, set both devices to the same key for precaution

  //WriteToRegister(CRYPT_H_REG, 0x02); //Writing 0x02 to the CRYPT_H register
  //WriteToRegister(CRYPT_L_REG, 0x03); //Writing 0x03 to the CRYPT_L register

  //Printing out the setting values
  
  Serial.print("ADDH: ");
  PrintRegisterValue(ADDH_REG);
  Serial.print("ADDL: ");
  PrintRegisterValue(ADDL_REG);
  Serial.print("NETID: ");
  PrintRegisterValue(NETID_REG);
  Serial.print("CHAN: ");
  PrintRegisterValue(CHANNEL_REG);
  Serial.print("REG0:");
  PrintRegisterValue(REG0);
  Serial.print("REG1:");
  PrintRegisterValue(REG1);
  Serial.print("REG3:");
  PrintRegisterValue(REG3);
  
}

void loop() {
  // Your main loop code here
}

void WriteToRegister(uint8_t reg, uint8_t val) {

  /*
    This function writes an 8 bit value to the specified register it follows the commands given in the datasheet page 13
   
    It first writes 0xC0 to the serial port, which will save the value permanently, if you want to save it temporarily, use 0xC2
   
    Then it defines the register you want to write to, and the size of the data you want to write which will be 0x01 (1 byte) 
  
    Then it finally writes the given value to the register
  */

	Serial2.write(PERMANENT);
	Serial2.write(reg);
	Serial2.write(0x01);
	Serial2.write(val);
  delay(10);
  uint8_t buffer[4];
	Serial2.readBytes(buffer, 4);
  /*
    This part is for testing purposes, it prints out the value that was written to the register

    You can remove this part if you want, but do not remove the Serial2.readBytes(buffer, 4) part, it is there to clean the buffer
    If you comment it out the next printing statements will print out previously written values
  */

  Serial.println(buffer[0],HEX);
  Serial.println(buffer[1], HEX);
  Serial.println(buffer[2], HEX);
  Serial.println(buffer[3], BIN);
  
	delay(PIN_RECOVER);
}

void ReadRegister(uint8_t reg, uint8_t *val) {

	uint8_t buffer[4];

  /*
    This function reads an 8 bit value from the specified register it follows the commands given in the datasheet page 13

    It first writes 0xC1 to the serial port, which is the command to read from the register

    Then it defines the register you want to read from, and the size of the data you want to read which will be 0x01 (1 byte)

    If you want to read more than 1 byte, you can change 0x01 to the number of bytes you want to read, just change the size of the buffer vector
  */

	Serial2.write(READ);
  Serial2.write(reg);
  Serial2.write(0x01);
  delay(10);
	Serial2.readBytes(buffer, 4);
  //Note that the last byte is the value that was read, the first 3 values are  only repetitions of the values you wrote to the serial port
	*val = buffer[3]; 

	delay(PIN_RECOVER);

}

void PrintRegisterValue(uint8_t reg) {
  
  /*
    This function prints out the value of the specified register
    The values that is the one you want to see is always the last value that was written 

    Channels are printed in decimal format as they are defined in datasheet page 15 which is the REG2 register, so it is easier to know if you set correctly

    Adress/Network ID are printed in hexadecimal format as it is the usual way to display them

    And the rest of the registers are printed in binary format because it is easier to analyse them using the datasheet as your guide since they define more than one parameter
    If less than 8 bits are shown, it means that the left most bits are 0
    For example, if it prints out 11, it represents the binary value 00000011
  */

  uint8_t buffer[4];

  Serial2.write(READ);
	Serial2.write(reg);
  Serial2.write(0x01);
  delay(50);
  Serial2.readBytes(buffer, 4);

  if(reg == CHANNEL_REG)
    {
    Serial.println(buffer[0],HEX);
    Serial.println(buffer[1], HEX);
    Serial.println(buffer[2], HEX);
    Serial.println(buffer[3]);
  }
  
  else if(reg == REG0 || reg == REG1 || reg == REG3)
  {
    Serial.println(buffer[0],HEX);
    Serial.println(buffer[1], HEX);
    Serial.println(buffer[2], HEX);
    Serial.println(buffer[3],BIN);
  }

  else
    {
    Serial.println(buffer[0],HEX);
    Serial.println(buffer[1], HEX);
    Serial.println(buffer[2], HEX);
    Serial.println(buffer[3],HEX);
  }

}
