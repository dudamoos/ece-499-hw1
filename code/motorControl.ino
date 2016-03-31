// Original code by Michael Kepler, modified by Wes Tarro

Dynamixel Dxl (/* DXL bus on Serial1 (USART1) */ 1); // IDs below
static const byte DXL_BOOM_ROT     = 101;
static const byte DXL_TROLLEY_MOVE = 100;
static const byte DXL_CLAW_PULLEY  = 102;
static const byte DXL_CLAW_PULLEY2 = 103;
static const byte DXL_CLAW_GRIP    = 104;

// TODO tune these
static const int grip_open     = 600;
static const int grip_closed   = 300;
static const int boom_speed    = 125; //50;
static const int trolley_speed = 200;
static const int pulley_speed  = 150;

static const int timeout = 2000; //in units of ms
static int timestamp = 0;

void setup() {
  // Initialize the dynamixel bus:
  // Dynamixel 2.0 Baudrate -> 0: 9600, 1: 57600, 2: 115200, 3: 1Mbps
  Dxl.begin(3);
  Dxl.setPacketType(DXL_PACKET_TYPE2);
  delay(1000);
  
  pinMode(BOARD_LED_PIN, OUTPUT);
  digitalWrite(BOARD_LED_PIN, HIGH);

  Dxl.wheelMode(DXL_BOOM_ROT);
  Dxl.wheelMode(DXL_TROLLEY_MOVE);
  Dxl.wheelMode(DXL_CLAW_PULLEY);
  Dxl.jointMode(DXL_CLAW_GRIP);
  delay(500);
 
  //Setup interrupt for keyboard input
  SerialUSB.println("Commands:");
  SerialUSB.println(" - Boom: j = left, l = right, n = stop");
  SerialUSB.println(" - Trolley: i = out, k = in, m = stop");
  SerialUSB.println(" - Pulley: e = up, d = down, c = stop");
  SerialUSB.println(" - Claw: s = open, f = close");
  SerialUSB.println(" - x is ignored and any other characters stop all motors!");
  SerialUSB.attachInterrupt(direction);
}

// desired format = "<boom><trolley><pulley><claw>", though each can be sent individually
void direction(byte* input, byte ncount) {
    for (byte i = 0; i < ncount; ++i) {
        switch (char(input[i])) { // MAKE SURE THE PC SOFTWARE SENDS THE STOP CHARACTERS WHEN NOT SENDING THE MOVE CHARACTERS!
            // char : <indicate stop or go cmd on led>    <command dynamixel over UART>
            case 'i': digitalWrite(BOARD_LED_PIN,  LOW); Dxl.goalSpeed(DXL_TROLLEY_MOVE, trolley_speed        ); break; // trolley move out
            case 'k': digitalWrite(BOARD_LED_PIN,  LOW); Dxl.goalSpeed(DXL_TROLLEY_MOVE, trolley_speed | 0x400); break; // trolley move in
            case 'm': digitalWrite(BOARD_LED_PIN, HIGH); Dxl.goalSpeed(DXL_TROLLEY_MOVE, 0                    ); break; // trolley stop
            case 'j': digitalWrite(BOARD_LED_PIN,  LOW); Dxl.goalSpeed(DXL_BOOM_ROT    , boom_speed           ); break; // boom pivot left
            case 'l': digitalWrite(BOARD_LED_PIN,  LOW); Dxl.goalSpeed(DXL_BOOM_ROT    , boom_speed    | 0x400); break; // boom pivot right
            case 'n': digitalWrite(BOARD_LED_PIN, HIGH); Dxl.goalSpeed(DXL_BOOM_ROT    , 0                    ); break; // boom stop
            case 'e': digitalWrite(BOARD_LED_PIN,  LOW); Dxl.goalSpeed(DXL_CLAW_PULLEY , pulley_speed         );
                                                         Dxl.goalSpeed(DXL_CLAW_PULLEY2, pulley_speed  | 0x400); break; // pulley lift up
            case 'd': digitalWrite(BOARD_LED_PIN,  LOW); Dxl.goalSpeed(DXL_CLAW_PULLEY , pulley_speed  | 0x400); 
                                                         Dxl.goalSpeed(DXL_CLAW_PULLEY2, pulley_speed         ); break; // pulley lower down
            case 'c': digitalWrite(BOARD_LED_PIN, HIGH); Dxl.goalSpeed(DXL_CLAW_PULLEY , 0                    );
                                                         Dxl.goalSpeed(DXL_CLAW_PULLEY2, 0                    ); break; // pulley stop
            case 's': digitalWrite(BOARD_LED_PIN,  LOW); Dxl.goalPosition(DXL_CLAW_GRIP, grip_open  ); break; // claw open   position!
            case 'f': digitalWrite(BOARD_LED_PIN,  LOW); Dxl.goalPosition(DXL_CLAW_GRIP, grip_closed); break; // claw closed position!
            case 'x': break;
            default : digitalWrite(BOARD_LED_PIN, HIGH); Dxl.goalSpeed(DXL_TROLLEY_MOVE, 0                    ); // stop all wheel mode servos
                                                         Dxl.goalSpeed(DXL_BOOM_ROT    , 0                    );
                                                         Dxl.goalSpeed(DXL_CLAW_PULLEY , 0                    );
                                                         Dxl.goalSpeed(DXL_CLAW_PULLEY2, 0                    ); break;
        }
    }
    timestamp = millis();
}

void loop(){  
  delay(200);
  if ((millis() - timestamp) > timeout){
    digitalWrite(BOARD_LED_PIN, HIGH);
    Dxl.goalSpeed(DXL_TROLLEY_MOVE, 0);
    Dxl.goalSpeed(DXL_BOOM_ROT    , 0);
    Dxl.goalSpeed(DXL_CLAW_PULLEY , 0);
  }
}