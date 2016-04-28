Dynamixel Dxl (/* DXL bus on Serial1 (USART1) */ 1); // IDs below
static const byte DXL_RF = 10, DXL_RM =  5, DXL_RB = 1;
static const byte DXL_LF = 20, DXL_LM = 12, DXL_LB = 99;
static const byte DXL_G1 = 101, DXL_G2 = 2; // TODO set the gripper IDs

// For each motor: leg forward, leg backward, leg up, leg down
static const word FRNT_RF =    0, BACK_RF =  500, HOLD_RF = 125, HI_RF = 818, LO_RF = 205, FROT_RF = 0x190, BROT_RF = 0x590;
static const word FRNT_RM =    0, BACK_RM =  500, HOLD_RM = 350, HI_RM = 818, LO_RM = 205, FROT_RM = 0x190, BROT_RM = 0x590;
static const word FRNT_RB =  523, BACK_RB = 1023, HOLD_RB = 648, HI_RB = 205, LO_RB = 818, FROT_RB = 0x190, BROT_RB = 0x590;
static const word FRNT_LF = 1023, BACK_LF =  523, HOLD_LF = 673, HI_LF = 205, LO_LF = 818, FROT_LF = 0x590, BROT_LF = 0x190;
static const word FRNT_LM = 1023, BACK_LM =  523, HOLD_LM = 898, HI_LM = 205, LO_LM = 818, FROT_LM = 0x590, BROT_LM = 0x190;
static const word FRNT_LB =  500, BACK_LB =    0, HOLD_LB = 125, HI_LB = 818, LO_LB = 205, FROT_LB = 0x590, BROT_LB = 0x190;
static const word J_SPEED = 300;

static const word G1_TOP = 358, G1_FWD = 512, G1_MID = 665, G1_LOW = 818;
static const word G2_CLOSED = 120, G2_OPEN = 275;

static word sf_packet0[18] = { DXL_LF,   LO_LF, J_SPEED, DXL_RM,   LO_RM, J_SPEED, DXL_LB,   LO_LB, J_SPEED, DXL_RF, FRNT_RF, J_SPEED, DXL_LM, FRNT_LM, J_SPEED, DXL_RB, FRNT_RB, J_SPEED };
static word sf_packet1[18] = { DXL_LF, BACK_LF, J_SPEED, DXL_RM, BACK_RM, J_SPEED, DXL_LB, BACK_LB, J_SPEED, DXL_RF,   LO_RF, J_SPEED, DXL_LM,   LO_LM, J_SPEED, DXL_RB,   LO_RB, J_SPEED };
static word sf_packet2[6] = { DXL_LF, FROT_LF, DXL_RM, FROT_RM, DXL_LB, FROT_LB };
static word sb_packet2[6] = { DXL_LF, BROT_LF, DXL_RM, BROT_RM, DXL_LB, BROT_LB };
static word sf_packet3[18] = { DXL_LF, FRNT_LF, J_SPEED, DXL_RM, FRNT_RM, J_SPEED, DXL_LB, FRNT_LB, J_SPEED, DXL_RF,   LO_RF, J_SPEED, DXL_LM,   LO_LM, J_SPEED, DXL_RB,   LO_RB, J_SPEED };
static word sf_packet4[18] = { DXL_LF,   LO_LF, J_SPEED, DXL_RM,   LO_RM, J_SPEED, DXL_LB,   LO_LB, J_SPEED, DXL_RF, BACK_RF, J_SPEED, DXL_LM, BACK_LM, J_SPEED, DXL_RB, BACK_RB, J_SPEED };
static word sf_packet5[6] = { DXL_RF, FROT_RF, DXL_LM, FROT_LM, DXL_RB, FROT_RB };
static word sb_packet5[6] = { DXL_RF, BROT_RF, DXL_LM, BROT_LM, DXL_RB, BROT_RB };

static word pr_packet0[18] = { DXL_LF, FRNT_LF, J_SPEED, DXL_RM, BACK_RM, J_SPEED, DXL_LB, FRNT_LB, J_SPEED, DXL_RF,   LO_RF, J_SPEED, DXL_LM,   LO_LM, J_SPEED, DXL_RB,   LO_RB, J_SPEED };
static word pr_packet1[18] = { DXL_LF,   LO_LF, J_SPEED, DXL_RM,   LO_RM, J_SPEED, DXL_LB,   LO_LB, J_SPEED, DXL_RF, FRNT_RF, J_SPEED, DXL_LM, BACK_LM, J_SPEED, DXL_RB, FRNT_RB, J_SPEED };
static word pr_packet2[6] = { DXL_RF, BROT_RF, DXL_LM, FROT_LM, DXL_RB, BROT_RB };
static word pl_packet2[6] = { DXL_RF, FROT_RF, DXL_LM, BROT_LM, DXL_RB, FROT_RB };
static word pr_packet3[18] = { DXL_LF,   LO_LF, J_SPEED, DXL_RM,   LO_RM, J_SPEED, DXL_LB,   LO_LB, J_SPEED, DXL_RF, BACK_RF, J_SPEED, DXL_LM, FRNT_LM, J_SPEED, DXL_RB, BACK_RB, J_SPEED };
static word pr_packet4[18] = { DXL_LF, BACK_LF, J_SPEED, DXL_RM, FRNT_RM, J_SPEED, DXL_LB, BACK_LB, J_SPEED, DXL_RF,   LO_RF, J_SPEED, DXL_LM,   LO_LM, J_SPEED, DXL_RB,   LO_RB, J_SPEED };
static word pr_packet5[6] = { DXL_LF, FROT_LF, DXL_RM, BROT_RM, DXL_LB, FROT_LB };
static word pl_packet5[6] = { DXL_LF, BROT_LF, DXL_RM, FROT_RM, DXL_LB, BROT_LB };

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
  Dxl.setPosition(DXL_G1, G1_FWD, J_SPEED);
  Dxl.setPosition(DXL_G2, G2_OPEN, J_SPEED);
  delay(500);
  
  // TODO set initial positions of servos
 
  //Setup interrupt for keyboard input
  SerialUSB.println("Commands:");
  SerialUSB.println(" - w = step forward, s = step backward");
  SerialUSB.println(" - a = pivot left, d = pivot right");
  SerialUSB.println(" - r = full standing, f = full crouching");
  SerialUSB.println(" - t = close claw, g = open claw");
  SerialUSB.println(" - 1,2,3,4 = position claw top,fwd,mid,low");
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
      if (walk_phase > 1) {
        Dxl.syncWrite(30, 2, sf_packet0, 18); delay(500);
        walk_phase = 0;
      }
      if (walk_phase == 0) {
        Dxl.syncWrite(30, 2, sf_packet1, 18); delay(500);
        Dxl.wheelMode(DXL_LF); Dxl.wheelMode(DXL_RM); Dxl.wheelMode(DXL_LB); Dxl.syncWrite(32, 1, sf_packet2, 6); delay(800);
        Dxl.jointMode(BROADCAST_ID); Dxl.syncWrite(30, 2, sf_packet3, 18); delay(500);
        Dxl.jointMode(BROADCAST_ID); Dxl.syncWrite(30, 2, sf_packet3, 18); delay(200); // twice just to make sure it takes
        walk_phase = 1;
      } else {
        Dxl.syncWrite(30, 2, sf_packet4, 18); delay(500);
        Dxl.wheelMode(DXL_RF); Dxl.wheelMode(DXL_LM); Dxl.wheelMode(DXL_RB); Dxl.syncWrite(32, 1, sf_packet5, 6); delay(800);
        Dxl.jointMode(BROADCAST_ID); Dxl.syncWrite(30, 2, sf_packet0, 18); delay(500);
        Dxl.jointMode(BROADCAST_ID); Dxl.syncWrite(30, 2, sf_packet0, 18); delay(200); // twice just to make sure it takes
        walk_phase = 0;
      }
      digitalWrite(BOARD_LED_PIN, HIGH);
      return;
    case 's':
      digitalWrite(BOARD_LED_PIN, LOW);
      if (walk_phase > 1) {
        Dxl.syncWrite(30, 2, sf_packet0, 18); delay(500);
        walk_phase = 0;
      }
      if (walk_phase == 0) {
        Dxl.wheelMode(DXL_RF); Dxl.wheelMode(DXL_LM); Dxl.wheelMode(DXL_RB); Dxl.syncWrite(32, 1, sb_packet5, 6); delay(800);
        Dxl.jointMode(BROADCAST_ID); Dxl.syncWrite(30, 2, sf_packet4, 18); delay(500);
        Dxl.jointMode(BROADCAST_ID); Dxl.syncWrite(30, 2, sf_packet4, 18); delay(200); // twice just to make sure it takes
        Dxl.syncWrite(30, 2, sf_packet3, 18); delay(500);
        walk_phase = 1;
      } else {
        Dxl.wheelMode(DXL_LF); Dxl.wheelMode(DXL_RM); Dxl.wheelMode(DXL_LB); Dxl.syncWrite(32, 1, sb_packet2, 6); delay(800);
        Dxl.jointMode(BROADCAST_ID); Dxl.syncWrite(30, 2, sf_packet1, 18); delay(500);
        Dxl.jointMode(BROADCAST_ID); Dxl.syncWrite(30, 2, sf_packet1, 18); delay(200); // twice just to make sure it takes
        Dxl.syncWrite(30, 2, sf_packet0, 18); delay(500);
        walk_phase = 0;
      }
      digitalWrite(BOARD_LED_PIN, HIGH);
      return;
    case 'd':
      digitalWrite(BOARD_LED_PIN, LOW);
      if (walk_phase < 2 || walk_phase > 3) {
        Dxl.syncWrite(30, 2, pr_packet0, 18); delay(500);
        walk_phase = 2;
      }
      if (walk_phase == 2) {
        Dxl.syncWrite(30, 2, pr_packet1, 18); delay(500);
        Dxl.wheelMode(DXL_RF); Dxl.wheelMode(DXL_LM); Dxl.wheelMode(DXL_RB); Dxl.syncWrite(32, 1, pr_packet2, 6); delay(800);
        Dxl.jointMode(BROADCAST_ID); Dxl.syncWrite(30, 2, pr_packet3, 18); delay(500);
        Dxl.jointMode(BROADCAST_ID); Dxl.syncWrite(30, 2, pr_packet3, 18); delay(200); // twice just to make sure it takes
        walk_phase = 3;
      } else {
        Dxl.syncWrite(30, 2, pr_packet4, 18); delay(500);
        Dxl.wheelMode(DXL_LF); Dxl.wheelMode(DXL_RM); Dxl.wheelMode(DXL_LB); Dxl.syncWrite(32, 1, pr_packet5, 6); delay(800);
        Dxl.jointMode(BROADCAST_ID); Dxl.syncWrite(30, 2, pr_packet0, 18); delay(500);
        Dxl.jointMode(BROADCAST_ID); Dxl.syncWrite(30, 2, pr_packet0, 18); delay(200); // twice just to make sure it takes
        walk_phase = 2;
      }
      digitalWrite(BOARD_LED_PIN, HIGH);
      return;
    case 'a':
      digitalWrite(BOARD_LED_PIN, LOW);
      if (walk_phase < 4 || walk_phase > 5) {
        Dxl.syncWrite(30, 2, pr_packet1, 18); delay(500);
        walk_phase = 4;
      }
      if (walk_phase == 4) {
        Dxl.syncWrite(30, 2, pr_packet0, 18); delay(500);
        Dxl.wheelMode(DXL_LF); Dxl.wheelMode(DXL_RM); Dxl.wheelMode(DXL_LB); Dxl.syncWrite(32, 1, pl_packet5, 6); delay(800);
        Dxl.jointMode(BROADCAST_ID); Dxl.syncWrite(30, 2, pr_packet4, 18); delay(500);
        Dxl.jointMode(BROADCAST_ID); Dxl.syncWrite(30, 2, pr_packet4, 18); delay(200); // twice just to make sure it takes
        walk_phase = 5;
      } else {
        Dxl.syncWrite(30, 2, pr_packet3, 18); delay(500);
        Dxl.wheelMode(DXL_RF); Dxl.wheelMode(DXL_LM); Dxl.wheelMode(DXL_RB); Dxl.syncWrite(32, 1, pl_packet2, 6); delay(800);
        Dxl.jointMode(BROADCAST_ID); Dxl.syncWrite(30, 2, pr_packet1, 18); delay(500);
        Dxl.jointMode(BROADCAST_ID); Dxl.syncWrite(30, 2, pr_packet1, 18); delay(200); // twice just to make sure it takes
        walk_phase = 4;
      }
      digitalWrite(BOARD_LED_PIN, HIGH);
      return;
    case 'r':
      digitalWrite(BOARD_LED_PIN, LOW);
      Dxl.syncWrite(30, 1, lo_packet, 12); delay(500);
      walk_phase = 0;
      digitalWrite(BOARD_LED_PIN, HIGH);
      return;
    case 'f':
      digitalWrite(BOARD_LED_PIN, LOW);
      Dxl.syncWrite(30, 1, hi_packet, 12); delay(500);
      walk_phase = 0;
      digitalWrite(BOARD_LED_PIN, HIGH);
      return;
    case '1': digitalWrite(BOARD_LED_PIN, LOW); Dxl.setPosition(DXL_G1, G1_TOP, J_SPEED); digitalWrite(BOARD_LED_PIN, HIGH); return;
    case '2': digitalWrite(BOARD_LED_PIN, LOW); Dxl.setPosition(DXL_G1, G1_FWD, J_SPEED); digitalWrite(BOARD_LED_PIN, HIGH); return;
    case '3': digitalWrite(BOARD_LED_PIN, LOW); Dxl.setPosition(DXL_G1, G1_MID, J_SPEED); digitalWrite(BOARD_LED_PIN, HIGH); return;
    case '4': digitalWrite(BOARD_LED_PIN, LOW); Dxl.setPosition(DXL_G1, G1_LOW, J_SPEED); digitalWrite(BOARD_LED_PIN, HIGH); return;
    case 't': digitalWrite(BOARD_LED_PIN, LOW); Dxl.setPosition(DXL_G2, G2_CLOSED, J_SPEED); digitalWrite(BOARD_LED_PIN, HIGH); return;
    case 'g': digitalWrite(BOARD_LED_PIN, LOW); Dxl.setPosition(DXL_G2, G2_OPEN, J_SPEED); digitalWrite(BOARD_LED_PIN, HIGH); return;
    default:
      SerialUSB.println("Invalid command!");
      return;
  }
      //delay(5000);
}
