//Code to allow the crane to operate after activated by push button
#include <Ultrasonic.h>
#define DXL_BUS_SERIAL1 1
//OpenCM Pins
Ultrasonic distSensor(8,7);  //Specify Trig and Echo pins (Trig,Echo)
const int button = 15;
//Motor IDs
const int midTrolleyId = 100;
const int rightTrolleyId = 102;
const int leftTrolleyId = 103;
const int claw = 104;
//Trolley Control Parameters
const int inwardLimit = 9;
const int outwardLimit = 47;
const int open = 600; 
const int close = 350; 
const int trolleySpeed = 150;
const int wait = 2000;

Dynamixel Dxl(DXL_BUS_SERIAL1); 

void setup(){
  float reading;
  SerialUSB.begin();
  
  //Pin Setup
  pinMode(button, INPUT_PULLUP);
  pinMode(BOARD_LED_PIN, OUTPUT);
  
  //Motor Setup
  Dxl.begin(3);
  Dxl.setPacketType(DXL_PACKET_TYPE2);
  delay(1000);
  
  Dxl.jointMode(leftTrolleyId); //jointMode() is to use position mode
  Dxl.jointMode(rightTrolleyId);
  Dxl.jointMode(claw);
  Dxl.wheelMode(midTrolleyId);
  Dxl.maxTorque(1023, 0xFE);
  delay(1000);
  
  //Have the claw start lifted
  Dxl.goalPosition(leftTrolleyId, 1023); 
  Dxl.goalPosition(rightTrolleyId, 0);
  SerialUSB.println("Raising Claw");
  delay(wait);
  
  //Open the claw
  Dxl.goalPosition(claw, open);
  
  //Move the trolley to the starting position
  reading = getDistance();
  SerialUSB.println(reading);
  if ((reading - inwardLimit) > 0){
    SerialUSB.println("Trolley is too far out...Moving In");
    moveIn();
  }
  else{
    SerialUSB.println("Trolley is too far in...Moving Out");
    moveOut();
  }
  Dxl.goalSpeed(midTrolleyId, 0);
  delay(1000);
  
}

void loop(){
  //Read the state of the pushbutton value:
  int buttonState = digitalRead(button);
  //Is button pushed?
  if(buttonState==LOW){
    digitalWrite(BOARD_LED_PIN, LOW);
    activateClaw();
  }
  //Button is not pushed
  if(buttonState==HIGH){// if button is released, means GND(LOW) is connected to BOARD_BUTTON_PIN
    digitalWrite(BOARD_LED_PIN, HIGH);
    SerialUSB.println("Waiting to be activated");
  }
}

void activateClaw(){ 
  float reading = 0;
  
  //Move the claw out---------------------------------------
  moveOut();
  Dxl.goalSpeed(midTrolleyId, 0);
  delay(wait);
  
  //Drop the claw-------------------------------------------
  Dxl.goalPosition(leftTrolleyId, 0); 
  Dxl.goalPosition(rightTrolleyId, 1023);
  SerialUSB.println("Dropping Claw");
  delay(wait);
  
  //Close the claw------------------------------------------
  Dxl.goalPosition(claw, close);
  SerialUSB.println("Closing Claw");
  delay(wait);
  
  //Raise the claw-------------------------------------------
  Dxl.goalPosition(leftTrolleyId, 1023); 
  Dxl.goalPosition(rightTrolleyId, 0);
  SerialUSB.println("Raising Claw");
  delay(wait);
  
  //Move the trolley inwards---------------------------------
  reading = getDistance();
  moveIn();
  Dxl.goalSpeed(midTrolleyId, 0);
  delay(wait);
  
  //Open the claw---------------------------------------------
  SerialUSB.println("Open Claw");
  Dxl.goalPosition(claw, open);
  delay(wait);
}

void moveIn(){
  float reading = getDistance();
  SerialUSB.println("Moving Inwards");
  while (reading > inwardLimit) {
    Dxl.goalSpeed(midTrolleyId, trolleySpeed|0x400);
    reading = getDistance();
    SerialUSB.print("Distance ");
    SerialUSB.println(reading);
  }
}

void moveOut(){
  float reading = getDistance();
  SerialUSB.println("Moving Outwards");
  while (reading < outwardLimit) {
    Dxl.goalSpeed(midTrolleyId, trolleySpeed);
    reading = getDistance();
    SerialUSB.print("Distance ");
    SerialUSB.println(reading);
  }
}

//Returns the average of 20 distance readings
float getDistance(){
  int numReadings = 40;
  int readingSum = 0;
  int distance = 0;
  int count = 0;
  float readingAvg = 0;
  
  for( int i = 0; i < numReadings; i++){
      distance = distSensor.readDistance();
      if (distance < 65){
        readingSum = readingSum + distance;
        count = count+1;
      }
      else{
      }
      delay(20);
  }
  
  readingAvg = readingSum/count;
  return(readingAvg);
}





