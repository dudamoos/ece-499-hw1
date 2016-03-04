// Code by Michael Kepler

/* Serial device defines for dxl bus */
#define DXL_BUS_SERIAL1 1  //Dynamixel on Serial1(USART1)  <-OpenCM9.04

/* Dynamixel ID defines */
#define lw 100
#define rw 101

Dynamixel Dxl(DXL_BUS_SERIAL1); 
 
int timeout = 2000; //in units of ms
int timestamp = 0;

void setup() {
  // Initialize the dynamixel bus:
  // Dynamixel 2.0 Baudrate -> 0: 9600, 1: 57600, 2: 115200, 3: 1Mbps  
  Dxl.begin(3);
  Dxl.setPacketType(DXL_PACKET_TYPE2);
  delay(1000);
  
  pinMode(BOARD_LED_PIN, OUTPUT);
  digitalWrite(BOARD_LED_PIN, HIGH);

  Dxl.wheelMode(lw); //jointMode() is to use position mode
  Dxl.wheelMode(rw); //jointMode() is to use position mode 
  delay(500);
 
  //Setup interrupt for keyboard input
  SerialUSB.println("To change speed or direction, type a command:");
  SerialUSB.println("Command Structure is DirectionSpeed, ex: j324");
  SerialUSB.println("Direction: i:Forward j:Left k:Backward l:Right");
  SerialUSB.println("Speed range: 0 <--> 850");
  SerialUSB.attachInterrupt(control);
}

void control(byte* input, byte ncount){
  
  SerialUSB.println();
  SerialUSB.println("Received Command");
  //Get Direction
  char direction = input[0];
  
  //Get the Speed
  char charSpeed[(ncount-1)];
  SerialUSB.print("Input Buffer:");
  for(int i = 0; i < ncount-1; i++){
    charSpeed[i] = char(input[i+1]);
    SerialUSB.print(char(input[i+1]));
  }
   SerialUSB.println();
   SerialUSB.print("charSpeed:");
  for(int i = 0; i < ncount-1; i++){
    SerialUSB.print(charSpeed[i]);
  }
  
  int speed = atoi(charSpeed);
  SerialUSB.println();
  SerialUSB.print("Integer Speed: ");
  SerialUSB.println(speed);
  
  //Make sure speed is within range
  if (speed < 0){
    SerialUSB.println("Speed cannot be less than 0. Setting speed to 0.");
    speed = 0;
  }
  else if (speed > 1023){
    SerialUSB.println("Speed cannot be greater than 850. Setting speed to 850.");
    speed = 850;
  }
  
  /*Forward*/
  if (direction == 'i'){
    digitalWrite(BOARD_LED_PIN, LOW);
    SerialUSB.println("Direction: Forward");
    Dxl.goalSpeed(lw, speed);
    Dxl.goalSpeed(rw, speed|0x400);
    
  }
  /*Backward*/
  else if (direction == 'k'){ 
    digitalWrite(BOARD_LED_PIN, LOW);
    SerialUSB.println("Direction: Backward");
    Dxl.goalSpeed(lw, speed | 0x400);
    Dxl.goalSpeed(rw, speed);
    
  }
  /*Pivot Left*/
  else if (direction == 'j'){
    digitalWrite(BOARD_LED_PIN, LOW);
    Dxl.goalSpeed(lw, speed | 0x400);
    SerialUSB.println("Direction: Pivot Left");
    Dxl.goalSpeed(rw, speed | 0x400);
   
  }
  /*Pivot Right*/
  else if (direction == 'l'){
    digitalWrite(BOARD_LED_PIN, LOW);
    Dxl.goalSpeed(lw, speed);
    SerialUSB.println("Direction: Pivot Right");
    Dxl.goalSpeed(rw, speed); 
  }
  /*Stop*/
  else{
    digitalWrite(BOARD_LED_PIN, HIGH);
    SerialUSB.println("Direction: Stop");
    Dxl.goalSpeed(lw, 0);
    Dxl.goalSpeed(rw, 0);
  }
  timestamp = millis();
}

 
void loop(){  
  delay(200);
  if ((millis() - timestamp) > timeout){
    digitalWrite(BOARD_LED_PIN, HIGH);
    Dxl.goalSpeed(lw, 0);
    Dxl.goalSpeed(rw, 0); 
  }
  
}
