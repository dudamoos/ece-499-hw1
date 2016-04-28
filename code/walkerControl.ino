// Original code by Michael Kepler, modified by Wes Tarro

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

static word lsf_packet1[9] = { DXL_LF, BACK_LF, J_SPEED, DXL_RM, BACK_RM, J_SPEED, DXL_LB, BACK_LB, J_SPEED };
static word lsf_packet2[6] = { DXL_LF, FROT_LF, DXL_RM, FROT_RM, DXL_LB, FROT_LB };
static word lsf_packet3[9] = { DXL_LF, FRNT_LF, J_SPEED, DXL_RM, FRNT_RM, J_SPEED, DXL_LB, FRNT_LB, J_SPEED };
static word lsf_packet4[9] = { DXL_LF, HOLD_LF, J_SPEED, DXL_RM, HOLD_RM, J_SPEED, DXL_LB, HOLD_LB, J_SPEED };

static word rsf_packet1[9] = { DXL_RF, BACK_RF, J_SPEED, DXL_LM, BACK_LM, J_SPEED, DXL_RB, BACK_RB, J_SPEED };
static word rsf_packet2[6] = { DXL_RF, FROT_RF, DXL_LM, FROT_LM, DXL_RB, FROT_RB };
static word rsf_packet3[9] = { DXL_RF, FRNT_RF, J_SPEED, DXL_LM, FRNT_LM, J_SPEED, DXL_RB, FRNT_RB, J_SPEED };
static word rsf_packet4[9] = { DXL_RF, HOLD_RF, J_SPEED, DXL_LM, HOLD_LM, J_SPEED, DXL_RB, HOLD_RB, J_SPEED };

static word lsb_packet1[9] = { DXL_LF, FRNT_LF, J_SPEED, DXL_RM, FRNT_RM, J_SPEED, DXL_LB, FRNT_LB, J_SPEED };
static word lsb_packet2[6] = { DXL_LF, BROT_LF, DXL_RM, BROT_RM, DXL_LB, BROT_LB };
static word lsb_packet3[9] = { DXL_LF, BACK_LF, J_SPEED, DXL_RM, BACK_RM, J_SPEED, DXL_LB, BACK_LB, J_SPEED };

static word rsb_packet1[9] = { DXL_RF, FRNT_RF, J_SPEED, DXL_LM, FRNT_LM, J_SPEED, DXL_RB, FRNT_RB, J_SPEED };
static word rsb_packet2[6] = { DXL_RF, BROT_RF, DXL_LM, BROT_LM, DXL_RB, BROT_RB };
static word rsb_packet3[9] = { DXL_RF, BACK_RF, J_SPEED, DXL_LM, BACK_LM, J_SPEED, DXL_RB, BACK_RB, J_SPEED };

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
  delay(500);
  
  // TODO set initial positions of servos
 
  //Setup interrupt for keyboard input
  SerialUSB.println("Commands:");
  SerialUSB.println(" - q = left step  forward, e = right step  forward");
  SerialUSB.println(" - a = left step backward, d = right step backward");
  SerialUSB.println(" - w = full standing, s = full crouching");
  // TODO do we need a key to put the motors back in walking phase?
}

void loop(){  
  delay(200);
  if (!SerialUSB.available()) return;
  
  switch (char(SerialUSB.read())) {
    case 'q': // left step forward
      digitalWrite(BOARD_LED_PIN, LOW);
      Dxl.syncWrite(30, 2, lsf_packet1, 9); delay(500); // push forward // David used a delay of 1000ms
      Dxl.wheelMode(DXL_LF); Dxl.wheelMode(DXL_RM); Dxl.wheelMode(DXL_LB); // enable wheel mode
      Dxl.syncWrite(32, 1, lsf_packet2, 6); delay(800); // move legs around // was originally 600
      Dxl.jointMode(DXL_LF); Dxl.jointMode(DXL_RM); Dxl.jointMode(DXL_LB); // enable joint mode
      Dxl.syncWrite(30, 2, lsf_packet3, 9); delay(500); // ensure legs in correct position
      Dxl.syncWrite(30, 2, lsf_packet4, 9); delay(500); // ensure legs in correct position
      digitalWrite(BOARD_LED_PIN, HIGH);
      return;
    case 'e': // right step forward
      digitalWrite(BOARD_LED_PIN, LOW);
      Dxl.syncWrite(30, 2, rsf_packet1, 9); delay(500); // push forward // David used a delay of 1000ms
      Dxl.wheelMode(DXL_RF); Dxl.wheelMode(DXL_LM); Dxl.wheelMode(DXL_RB); // enable wheel mode
      Dxl.syncWrite(32, 1, rsf_packet2, 6); delay(800); // move legs around // was originally 600
      Dxl.jointMode(DXL_RF); Dxl.jointMode(DXL_LM); Dxl.jointMode(DXL_RB); // enable joint mode
      Dxl.syncWrite(30, 2, rsf_packet3, 9); delay(500); // ensure legs in correct position
      Dxl.syncWrite(30, 2, rsf_packet4, 9); delay(500); // ensure legs in correct position
      digitalWrite(BOARD_LED_PIN, HIGH);
      return;
    case 'a': // left step backward
      digitalWrite(BOARD_LED_PIN, LOW);
      Dxl.syncWrite(30, 2, lsb_packet1, 9); delay(500); // push forward // David used a delay of 1000ms
      Dxl.wheelMode(DXL_LF); Dxl.wheelMode(DXL_RM); Dxl.wheelMode(DXL_LB); // enable wheel mode
      Dxl.syncWrite(32, 1, lsb_packet2, 6); delay(800); // move legs around // was originally 600
      Dxl.jointMode(DXL_LF); Dxl.jointMode(DXL_RM); Dxl.jointMode(DXL_LB); // enable joint mode
      Dxl.syncWrite(30, 2, lsb_packet3, 9); delay(500); // ensure legs in correct position
      Dxl.syncWrite(30, 2, lsb_packet1, 9); delay(500); // ensure legs in correct position
      digitalWrite(BOARD_LED_PIN, HIGH);
      return;
    case 'd': // right step backward
      digitalWrite(BOARD_LED_PIN, LOW);
      Dxl.syncWrite(30, 2, rsb_packet1, 9); delay(500); // push forward // David used a delay of 1000ms
      Dxl.wheelMode(DXL_RF); Dxl.wheelMode(DXL_LM); Dxl.wheelMode(DXL_RB); // enable wheel mode
      Dxl.syncWrite(32, 1, rsb_packet2, 6); delay(800); // move legs around // was originally 600
      Dxl.jointMode(DXL_RF); Dxl.jointMode(DXL_LM); Dxl.jointMode(DXL_RB); // enable joint mode
      Dxl.syncWrite(30, 2, rsb_packet3, 9); delay(500); // ensure legs in correct position
      digitalWrite(BOARD_LED_PIN, HIGH);
      return;
    case 'w':
      digitalWrite(BOARD_LED_PIN, LOW);
      Dxl.syncWrite(30, 1, lo_packet, 12); delay(500);
      digitalWrite(BOARD_LED_PIN, HIGH);
      return;
    case 's':
      digitalWrite(BOARD_LED_PIN, LOW);
      Dxl.syncWrite(30, 1, hi_packet, 12); delay(500);
      digitalWrite(BOARD_LED_PIN, HIGH);
      return;
    default:
      SerialUSB.println("Invalid command!");
      return;
  }
}