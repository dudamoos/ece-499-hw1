Dynamixel Dxl (/* DXL bus on Serial1 (USART1) */ 1); // IDs below
static const byte DXL_DPAD = 1, DXL_FIRE = 2;

static const word FIRE_DOWN = 525; // 150 deg -> 512 ticks
static const word FIRE_UP   = 477; // 140 deg -> 477.4 ticks
static const word DPAD_LEFT = 650; // 160 deg -> 545.6 ticks
static const word DPAD_MID  = 512; // 150 deg -> 512 ticks
static const word DPAD_RITE = 370; // 140 deg -> 477.4 ticks

void setup() {
    Dxl.begin(3);
    Dxl.setPacketType(DXL_PACKET_TYPE2);
    delay(200);
    
    pinMode(BOARD_LED_PIN, OUTPUT);
    digitalWrite(BOARD_LED_PIN, LOW);
    Dxl.jointMode(BROADCAST_ID);
    Dxl.goalPosition(DXL_FIRE, FIRE_UP  );
    Dxl.goalPosition(DXL_DPAD, DPAD_MID );
    delay(500);
    digitalWrite(BOARD_LED_PIN, HIGH);
    
    Timer1.pause();
    Timer1.setMode(1, TIMER_OUTPUT_COMPARE);
    Timer1.setCompare(1, Timer1.setPeriod(500000));
    Timer1.attachInterrupt(1, darkenLed);
    Timer1.refresh();
    
    SerialUSB.println("Commands:");
    SerialUSB.println("- f = press fire button, s = release fire button");
    SerialUSB.println("- l, r, m = d-pad press left, right, none (mid)");
    SerialUSB.attachInterrupt(recvCmd);
}

void recvCmd(byte* input, byte count) {
    SerialUSB.write(input, count); SerialUSB.println();
    Timer1.refresh();
    Timer1.resume();
    switch(input[0]) {
        case 'f': digitalWrite(BOARD_LED_PIN, LOW); Dxl.goalPosition(DXL_FIRE, FIRE_DOWN); break;
        case 's': digitalWrite(BOARD_LED_PIN, LOW); Dxl.goalPosition(DXL_FIRE, FIRE_UP  ); break;
        case 'l': digitalWrite(BOARD_LED_PIN, LOW); Dxl.goalPosition(DXL_DPAD, DPAD_LEFT); break;
        case 'r': digitalWrite(BOARD_LED_PIN, LOW); Dxl.goalPosition(DXL_DPAD, DPAD_RITE); break;
        case 'm': digitalWrite(BOARD_LED_PIN, LOW); Dxl.goalPosition(DXL_DPAD, DPAD_MID ); break;
        default: digitalWrite(BOARD_LED_PIN, HIGH); SerialUSB.println("Unknown command!"); break;
    }
}

void darkenLed() { digitalWrite(BOARD_LED_PIN, HIGH); Timer1.pause(); }

void loop() {
    delay(1000); // If only I had low power mode ... sigh.
}
