//
//    FILE: HX_kitchen_scale.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: HX711 demo
//     URL: https://github.com/RobTillaart/HX711


#include "HX711.h"
#include <SPI.h>
#include "SD.h"

//#include <virtuabotixRTC.h>
// Creation of the Real Time Clock Object
virtuabotixRTC myRTC(8, 2, 5);

File Data;
HX711 scale;
int CS =4;

uint8_t dataPin = 6;
uint8_t clockPin = 7;

float calibration_weight = 1155;                            //KG
#define ignition_button 3                                   //When pressed the countdown will start
#define ignition 9
#define start_countdown_led 10															//RED LED
#define safe_led	0																					//GREEN LED
#define e_stop 1                                            //The e-stop will halt the current process and take the program back to the main loop start.
int countdown;
int counter;

bool ignition_button_status = true;
bool safe = true;


//uint8_t dataPin  = 19;//for esp32
//uint8_t clockPin = 18;//for esp32

void setup()
{
 
  // Set the current date, and time in the following format:
  // seconds, minutes, hours, day of the week, day of the month, month, year
  //myRTC.setDS1302Time(0, 04, 19, 1, 9, 4, 2022);
  Serial.begin(9600);
  Serial.println(__FILE__);
  Serial.print("LIBRARY VERSION: ");
  Serial.println(HX711_LIB_VERSION);
  Serial.println();

  scale.begin(dataPin, clockPin);

  Serial.print("UNITS: ");
  Serial.println(scale.get_units(10));

  Serial.println("\nEmpty the scale, you have 5 seconds...");
  //while(!Serial.available());
  for(counter = 0; counter == 6; counter++){
    digitalWrite(safe_led,HIGH);
    digitalWrite(start_countdown_led,HIGH);
    delay(500);
    digitalWrite(safe_led,LOW);
    digitalWrite(start_countdown_led,LOW);
    delay(500);
  }
  while(Serial.available()) Serial.read();


  scale.tare();
  Serial.print("UNITS: ");
  Serial.println(scale.get_units(10));


  Serial.println("\nPlace the calibration weight as spesified on the scale, you have 5 seconds....");
  //while(!Serial.available());
  for(counter = 0; counter == 6; counter++){
    digitalWrite(safe_led,HIGH);
    digitalWrite(start_countdown_led,HIGH);
    delay(500);
    digitalWrite(safe_led,LOW);
    digitalWrite(start_countdown_led,LOW);
    delay(500);
  }
  while(Serial.available()) Serial.read();

  scale.calibrate_scale(calibration_weight, 5);
  Serial.print("UNITS: ");
  Serial.println(scale.get_units(10));

  Serial.println("\nScale is calibrated, continue in 5 seconds");
  // Serial.println(scale.get_scale());
  // Serial.println(scale.get_offset());
  //while(!Serial.available());
  for(counter = 0; counter == 6; counter++){
    digitalWrite(safe_led,HIGH);
    digitalWrite(start_countdown_led,HIGH);
    delay(500);
    digitalWrite(safe_led,LOW);
    digitalWrite(start_countdown_led,LOW);
    delay(500);
  }
  while(Serial.available()) Serial.read();
  
  pinMode(10,OUTPUT);
  SD.begin(CS);

//Ignition setup
  Serial.println("The ignition button is ready...");
  pinMode(ignition_button,INPUT_PULLUP);
  pinMode(ignition,OUTPUT);
	pinMode(start_countdown_led,OUTPUT);
	pinMode(safe_led,OUTPUT);
	pinMode(e_stop,INPUT_PULLUP);
	
}


void loop()
{
	ignition_button_status = digitalRead(ignition_button);
	safe = digitalRead(e_stop);
				if(safe == true){
							digitalWrite(safe_led,HIGH);
							}

  if (ignition_button_status == false && safe == true){
		for(countdown=10; countdown==0; countdown--){
				safe = digitalRead(e_stop);
				if(safe == false){
          Serial.print("e-stop was pressed");
				  break;}
				digitalWrite(start_countdown_led,HIGH);
				delay(500);
				digitalWrite(start_countdown_led,LOW);
				delay(500);
		}

	safe = digitalRead(e_stop);
	if(safe == true){
			digitalWrite(start_countdown_led,LOW);
			digitalWrite(safe_led,LOW);
			Serial.print("FIRE!!!");
			digitalWrite(ignition,HIGH);
			}
	
	}

 while(ignition_button_status == true){
    safe = digitalRead(e_stop);
				if(safe == false){
				  Serial.print("e-stop was pressed");
				  break;}
 		myRTC.updateTime();
  	Serial.print("Current Date / Time: ");
 		Serial.print(myRTC.dayofmonth);
 		Serial.print("/");
  	Serial.print(myRTC.month);
  	Serial.print("/");
  	Serial.print(myRTC.year);
  	Serial.print("  ");
  	Serial.print(myRTC.hours);
  	Serial.print(":");
  	Serial.print(myRTC.minutes);
  	Serial.print(":");
  	Serial.print(myRTC.seconds);

  	Serial.print(" UNITS: ");
  	Serial.println(scale.get_units(0.001));

   	Data = SD.open("data.txt", FILE_WRITE);
    if (Data){ 
      Serial.print("Current Date / Time: ");
   		Data.print(myRTC.dayofmonth);
  	 	Data.print("/");
   		Data.print(myRTC.month);
   		Data.print("/");
   		Data.print(myRTC.year);
   		Data.print("  ");
   		Data.print(myRTC.hours);
   		Data.print(":");
   		Data.print(myRTC.minutes);
   		Data.print(":");
   		Data.println(myRTC.seconds);

   		Data.println(scale.get_units(0.001));
   		Data.close();
   		Serial.print("ok ");
    
  }
  else{Serial.print("error ");}
	digitalWrite(ignition,LOW);
 }
}


// -- END OF FILE --
