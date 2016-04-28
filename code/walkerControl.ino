Dynamixel Dxl (/* DXL bus on Serial1 (USART1) */ 1); // IDs below
static const byte DXL_RF = 10, DXL_RM =  5, DXL_RB = 1;
static const byte DXL_LF = 20, DXL_LM = 12, DXL_LB = 99;
static const byte DXL_G1 = 50, DXL_G2 = 51; // TODO set the gripper IDs

// For each motor: leg forward, leg backward, leg up, leg down
static const word FRNT_RF =    0, BACK_RF =  500, HOLD_RF = 125, HI_RF = 784, LO_RF = 223, FROT_RF = 0x190, BROT_RF = 0x590;
static const word FRNT_RM =    0, BACK_RM =  500, HOLD_RM = 350, HI_RM = 777, LO_RM = 230, FROT_RM = 0x190, BROT_RM = 0x590;
static const word FRNT_RB =  523, BACK_RB = 1023, HOLD_RB = 648, HI_RB = 223, LO_RB = 794, FROT_RB = 0x190, BROT_RB = 0x590;
static const word FRNT_LF = 1023, BACK_LF =  523, HOLD_LF = 673, HI_LF = 216, LO_LF = 777, FROT_LF = 0x590, BROT_LF = 0x190;
static const word FRNT_LM = 1023, BACK_LM =  523, HOLD_LM = 898, HI_LM = 220, LO_LM = 794, FROT_LM = 0x590, BROT_LM = 0x190;
static const word FRNT_LB =  500, BACK_LB =    0, HOLD_LB = 125, HI_LB = 798, LO_LB = 229, FROT_LB = 0x590, BROT_LB = 0x190;
static const word J_SPEED = 300;

static word sf_packet0[18] = { DXL_LF,   LO_LF, J_SPEED, DXL_RM,   LO_RM, J_SPEED, DXL_LB,   LO_LB, J_SPEED, DXL_RF, FRNT_RF, J_SPEED, DXL_LM, FRNT_LM, J_SPEED, DXL_RB, FRNT_RB, J_SPEED };
static word sf_packet1[18] = { DXL_LF, BACK_LF, J_SPEED, DXL_RM, BACK_RM, J_SPEED, DXL_LB, BACK_LB, J_SPEED, DXL_RF,   LO_RF, J_SPEED, DXL_LM,   LO_LM, J_SPEED, DXL_RB,   LO_RB, J_SPEED };
static word sf_packet2[6] = { DXL_LF, FROT_LF, DXL_RM, FROT_RM, DXL_LB, FROT_LB };
static word sb_packet2[6] = { DXL_LF, BROT_LF, DXL_RM, BROT_RM, DXL_LB, BROT_LB };
static word sf_packet3[18] = { DXL_LF, FRNT_LF, J_SPEED, DXL_RM, FRNT_RM, J_SPEED, DXL_LB, FRNT_LB, J_SPEED, DXL_RF,   LO_RF, J_SPEED, DXL_LM,   LO_LM, J_SPEED, DXL_RB,   LO_RB, J_SPEED };
static word sf_packet4[18] = { DXL_LF,   LO_LF, J_SPEED, DXL_RM,   LO_RM, J_SPEED, DXL_LB,   LO_LB, J_SPEED, DXL_RF, BACK_RF, J_SPEED, DXL_LM, BACK_LM, J_SPEED, DXL_RB, BACK_RB, J_SPEED };
static word sf_packet5[6] = { DXL_RF, FROT_RF, DXL_LM, FROT_LM, DXL_RB, FROT_RB };
static word sb_packet5[6] = { DXL_RF, BROT_RF, DXL_LM, BROT_LM, DXL_RB, BROT_RB };

static word hi_packet[12] = { DXL_RF, HI_RF, DXL_RM, HI_RM, DXL_RB, HI_RB, DXL_LF, HI_LF, DXL_LM, HI_LM, DXL_LB, HI_LB };
static word lo_packet[12] = { DXL_RF, LO_RF, DXL_RM, LO_RM, DXL_RB, LO_RB, DXL_LF, LO_LF, DXL_LM, LO_LM, DXL_LB, LO_LB };

void setup() {
  // Initialize the dynamixel bus:
  // Dynamixel 2.0 Baudrate -> 0: 9600, 1: 57600, 2: 115200, 3: 1Mbps
  Dxl.begin(3);
  Dxl.setPacketType(DXL_PACKET_TYPE2);
  delay(1000);
  
  pinMode(BOARD_LED_PIN, OUTPUT);
  digitalWrite(BOARD_LED_PIN, HIGH);
  Dxl.jointMode(BROADCAST_ID); Dxl.maxTorque(BROADCAST_ID, 1023);
  Dxl.syncWrite(30, 2, sf_packet0, 18);
  delay(5000);
  
  // TODO set initial positions of servos
 
  //Setup interrupt for keyboard input
  SerialUSB.println("Commands:");
  SerialUSB.println(" - q = left step  forward, e = right step  forward");
  SerialUSB.println(" - a = left step backward, d = right step backward");
  SerialUSB.println(" - w = full standing, s = full crouching");
  // TODO do we need a key to put the motors back in walking phase?
}

static byte walk_phase = 0;

void loop() {
  // put your main code here, to run repeatedly:
  delay(200);
  if (!SerialUSB.available()) return;
  
  switch (char(SerialUSB.read())) {
    case 'w':
      digitalWrite(BOARD_LED_PIN, LOW);
      if (walk_phase == 0) {
        Dxl.syncWrite(30, 2, sf_packet0, 18); delay(500);
        walk_phase = 1;
      }
      if (walk_phase == 1) {
        Dxl.syncWrite(30, 2, sf_packet1, 18); delay(500);
        Dxl.wheelMode(DXL_LF); Dxl.wheelMode(DXL_RM); Dxl.wheelMode(DXL_LB); Dxl.syncWrite(32, 1, sf_packet2, 6); delay(800);
        Dxl.jointMode(BROADCAST_ID); Dxl.syncWrite(30, 2, sf_packet3, 18); delay(500);
        Dxl.jointMode(BROADCAST_ID); Dxl.syncWrite(30, 2, sf_packet3, 18); delay(200); // twice just to make sure it takes
        walk_phase = 2;
      } else {
        Dxl.syncWrite(30, 2, sf_packet4, 18); delay(500);
        Dxl.wheelMode(DXL_RF); Dxl.wheelMode(DXL_LM); Dxl.wheelMode(DXL_RB); Dxl.syncWrite(32, 1, sf_packet5, 6); delay(800);
        Dxl.jointMode(BROADCAST_ID); Dxl.syncWrite(30, 2, sf_packet0, 18); delay(500);
        Dxl.jointMode(BROADCAST_ID); Dxl.syncWrite(30, 2, sf_packet0, 18); delay(200); // twice just to make sure it takes
        walk_phase = 1;
      }
      digitalWrite(BOARD_LED_PIN, HIGH);
      return;
    case 's':
      digitalWrite(BOARD_LED_PIN, LOW);
      if (walk_phase == 0) {
        Dxl.syncWrite(30, 2, sf_packet0, 18); delay(500);
        walk_phase = 1;
      }
      if (walk_phase == 1) {
        Dxl.wheelMode(DXL_RF); Dxl.wheelMode(DXL_LM); Dxl.wheelMode(DXL_RB); Dxl.syncWrite(32, 1, sb_packet5, 6); delay(800);
        Dxl.jointMode(BROADCAST_ID); Dxl.syncWrite(30, 2, sf_packet4, 18); delay(500);
        Dxl.jointMode(BROADCAST_ID); Dxl.syncWrite(30, 2, sf_packet4, 18); delay(200); // twice just to make sure it takes
        Dxl.syncWrite(30, 2, sf_packet3, 18); delay(500);
        walk_phase = 2;
      } else {
        Dxl.wheelMode(DXL_LF); Dxl.wheelMode(DXL_RM); Dxl.wheelMode(DXL_LB); Dxl.syncWrite(32, 1, sb_packet2, 6); delay(800);
        Dxl.jointMode(BROADCAST_ID); Dxl.syncWrite(30, 2, sf_packet1, 18); delay(500);
        Dxl.jointMode(BROADCAST_ID); Dxl.syncWrite(30, 2, sf_packet1, 18); delay(200); // twice just to make sure it takes
        Dxl.syncWrite(30, 2, sf_packet0, 18); delay(500);
        walk_phase = 1;
      }
      digitalWrite(BOARD_LED_PIN, HIGH);
      return;
    case 'r':
      digitalWrite(BOARD_LED_PIN, LOW);
      Dxl.syncWrite(30, 1, lo_packet, 12); delay(500);
      digitalWrite(BOARD_LED_PIN, HIGH);
      return;
    case 'f':
      digitalWrite(BOARD_LED_PIN, LOW);
      Dxl.syncWrite(30, 1, hi_packet, 12); delay(500);
      digitalWrite(BOARD_LED_PIN, HIGH);
      return;
    default:
      SerialUSB.println("Invalid command!");
      return;
  }
      //delay(5000);
      
//      Dxl.syncWrite(30, 2, sf_packet1, 12); delay(500); // push forward // David used a delay of 1000ms
//      Dxl.wheelMode(DXL_LF); Dxl.wheelMode(DXL_RF); Dxl.wheelMode(DXL_LM); Dxl.wheelMode(DXL_RM); // enable wheel mode
//      Dxl.syncWrite(30, 1, sf_packet2, 8); delay(800); // push forward // David used a delay of 1000ms
//      Dxl.jointMode(DXL_LF); Dxl.jointMode(DXL_RF); Dxl.jointMode(DXL_LM); Dxl.jointMode(DXL_RM); // enable joint mode
//      Dxl.syncWrite(30, 2, sf_packet3, 12); delay(200); // ensure legs in correct position
//      Dxl.syncWrite(30, 2, sf_packet4, 6); delay(500); // switch moving halves // David used a delay of 1000ms
//      Dxl.wheelMode(DXL_LB); Dxl.wheelMode(DXL_RB); Dxl.wheelMode(DXL_LM); Dxl.wheelMode(DXL_RM); // enable wheel mode
//      Dxl.syncWrite(30, 1, sf_packet5, 6); delay(800); // push forward // David used a delay of 1000ms
//      Dxl.jointMode(DXL_LB); Dxl.jointMode(DXL_RB); Dxl.jointMode(DXL_LM); Dxl.jointMode(DXL_RM); // enable joint mode
//      Dxl.syncWrite(30, 2, sf_packet6, 12); delay(200); // ensure legs in correct position
//      Dxl.syncWrite(30, 2, sf_packet7, 12); delay(500); // push forward // David used a delay of 1000ms
//      digitalWrite(BOARD_LED_PIN, HIGH);
}
