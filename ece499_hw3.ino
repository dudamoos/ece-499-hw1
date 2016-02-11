#define DXL_SERIAL_BUS 1
#define DXL_A_ID 2 /* moves from 45 to 90 deg w/ 2 sec period */
#define DXL_A_LIM_LOWER 153 /* ~  45 degrees */
#define DXL_A_LIM_UPPER 307 /* ~  90 degrees */
#define DXL_A_SPEED      68 /* ~  45 deg/sec */
#define DXL_B_ID 3 /* moves from 0 to 180 deg w/ 1 sec period */
#define DXL_B_LIM_LOWER   0 /*     0 degrees */
#define DXL_B_LIM_UPPER 614 /* ~ 180 degrees */
#define DXL_B_SPEED     541 /* ~ 360 deg/sec */

Dynamixel Dxl(DXL_SERIAL_BUS);

void setup() {
  // put your setup code here, to run once:
    Dxl.begin(3);
    Dxl.jointMode(DXL_A_ID);
    Dxl.setPosition(DXL_A_ID, DXL_A_LIM_LOWER, DXL_A_SPEED);
    Dxl.jointMode(DXL_B_ID);
    Dxl.setPosition(DXL_B_ID, DXL_B_LIM_UPPER, DXL_B_SPEED);
    delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly: 
    Dxl.setPosition(DXL_A_ID, DXL_A_LIM_UPPER, DXL_A_SPEED);
    Dxl.setPosition(DXL_B_ID, DXL_B_LIM_UPPER, DXL_B_SPEED);
    delay(500);
    Dxl.setPosition(DXL_B_ID, DXL_B_LIM_LOWER, DXL_B_SPEED);
    delay(500);
    Dxl.setPosition(DXL_A_ID, DXL_A_LIM_LOWER, DXL_A_SPEED);
    Dxl.setPosition(DXL_B_ID, DXL_B_LIM_UPPER, DXL_B_SPEED);
    delay(500);
    Dxl.setPosition(DXL_B_ID, DXL_B_LIM_LOWER, DXL_B_SPEED);
    delay(500);
}

