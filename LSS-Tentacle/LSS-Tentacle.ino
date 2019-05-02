/* Author:      RB-ENantel / Eric Nantel - DiaLFonZo
** Date:        2019-05-02
** Description: LSS-Tentacle Control with a 2 axis Joystick & 1 Potentiometer for the base
** Blog Link:   https://www.robotshop.com/community/robots/show/simple-tentacule-w-lynxmotion-smart-servo-lss
*/

#define baudLSS (115200)

// Speed limiter in Degree per Second https://www.robotshop.com/info/wiki/lynxmotion/view/lynxmotion-smart-servo/lss-communication-protocol/#H12.MaxSpeedinDegrees28SD29
#define baseSpeed (85)
#define tentaculeSpeed (60)

// Axis range in 1/10° on either side of the Zero (base / x axis / y axis)
#define lssRangeK (1750)
#define lssRangeX (550)
#define lssRangeY (900)

// Refresh rate of the project in miliseconds using Millis to avoid any stop in the code
#define refreshRate (50)            
unsigned long lastRefresh = 0; 

// Assigment of the joystick pins & potentiometer (base / x axis / y axis)
#define kPin  2
#define xPin  3
#define yPin  4

// Variables used to store the values of the analog inputs (base / x axis / y axis)
int kVal;
int xVal;
int yVal;

// Variables used to offset the inperfection of the joysticks analog values (base / x axis / y axis)
int kZero;
int xZero;
int yZero; 

// Output buffer for effecient print
char lssOutput[100];

void setup() {
  // Starting serial at the given Baudrate
  Serial.begin(baudLSS);
  
  // Sending a Reset on all servos to start fresh and reset the servos without power cycling
  Serial.println("#254RESET");

  // Read and store the joysticks for the offsets
  kZero = 512;  //Set manually since we use a potentiometer without spring center return
  xZero = analogRead(xPin);
  yZero = analogRead(yPin);
}

void loop() {
  if (millis() - lastRefresh >= refreshRate) {
    lastRefresh = millis();
    joystickRead();
    lssUpdate();
  }
}

// Function to read and format the input values
void joystickRead() {

  // reads & store the value of the potentiometer (value between 0 and 1023)
  kVal = analogRead(kPin);
  xVal = analogRead(xPin);
  yVal = analogRead(yPin);
  
  // make sure to avoid any unwanted values out of the 0 to 1023 range
  kVal = constrain(kVal, 0, 1023);
  xVal = constrain(xVal, 0, 1023);    
  yVal = constrain(yVal, 0, 1023);

  // mapping the values to either side of the zero
  if (kVal <= kZero) {
    kVal = map(kVal, 0, kZero, -lssRangeK, 0);
  }
  else {
    kVal = map(kVal, kZero, 1023, 0, lssRangeK);
  }
  if (xVal <= xZero) {
    xVal = map(xVal, 0, xZero, lssRangeX, 0);
  }
  else {
    xVal = map(xVal, xZero, 1023, 0, -lssRangeX);
  }
  if (yVal <= yZero) {
    yVal = map(yVal, 0, yZero, lssRangeY, 0);
  }
  else {
    yVal = map(yVal, yZero, 1023, 0, -lssRangeY);
  } 
}

// Function to update the serial outputs to the servos
void lssUpdate() {
  sprintf(lssOutput, "#%u D%d SD%u\r #%u D%d SD%u\r #%u D%d SD%u\r #%u D%d SD%u\r #%u D%d SD%u\r",
    1,kVal,baseSpeed,
    2,xVal,tentaculeSpeed,
    3,yVal,tentaculeSpeed,
    4,xVal,tentaculeSpeed,
    5,yVal,tentaculeSpeed
  );
  Serial.println(lssOutput);
}
