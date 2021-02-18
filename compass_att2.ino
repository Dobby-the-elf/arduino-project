
//----------- by Dobby ------------
//Vcc for Vcc
//GND gor GND
//SCL for A5
//SDA for A4
/* Include the standard Wire library */
#include <Wire.h>

/* The I2C address of the module */
#define HMC5803L_Address 0x1E

/* Register address for the X Y and Z data */
#define X 3
#define Y 7
#define Z 5


float max_x=-999.9;
float max_y=-999.9;
float min_x=999.9;
float min_y=999.9;
float magneticCalibrateX;
float magneticCalibrateY;

float heading;
float last_heading=0;

int dist;

void updateCalibrate(float x, float y){
  if(x>max_x)max_x=x;
  if(y>max_y)max_y=y;
  if(x<min_x)min_x=x;
  if(y<min_y)min_y=y;
  magneticCalibrateX=0.5*(max_x+min_x);
  magneticCalibrateY=0.5*(max_y+min_y);
}

void setup()
{
  Serial.begin(9600);
  /* Initialise the Wire library */
  Wire.begin();

  /* Initialise the module */
  Init_HMC5803L();
}

void loop()
{
  /* Read each sensor axis data and output to the serial port */
  Serial.print(HMC5803L_Read(X));
  Serial.print(" ");
  Serial.print(HMC5803L_Read(Y));
  Serial.print(" ");
  Serial.print(HMC5803L_Read(Z));
  Serial.print(" angle:");
  float x=HMC5803L_Read(X);
  float y=HMC5803L_Read(Y);
  
  updateCalibrate(x, y);
  heading = atan2(y-magneticCalibrateY, x-magneticCalibrateX);
  
  //float heading = atan2(HMC5803L_Read(Y)+500, HMC5803L_Read(X)+200);
  //float declinationAngle = 0.0404;
  //heading += declinationAngle;

  // Correct for when signs are reversed.
  if(heading < 0)heading += 2*PI;
  if(heading > 2*PI)heading -= 2*PI;
  float headingDegrees = heading * 180/M_PI; 
  Serial.println(headingDegrees);
  /* Wait a little before reading again */
  delay(200);
}


/* This function will initialise the module and only needs to be run once
   after the module is first powered up or reset */
void Init_HMC5803L(void)
{
  /* Set the module to 8x averaging and 15Hz measurement rate */
  Wire.beginTransmission(HMC5803L_Address);
  Wire.write(0x00);
  Wire.write(0x70);
         
  /* Set a gain of 5 */
  Wire.write(0x01);
  Wire.write(0xA0);
  Wire.endTransmission();
}


/* This function will read once from one of the 3 axis data registers
and return the 16 bit signed result. */
int HMC5803L_Read(byte Axis)
{
  int Result;

  /* Initiate a single measurement */
  Wire.beginTransmission(HMC5803L_Address);
  Wire.write(0x02);
  Wire.write(0x01);
  Wire.endTransmission();
  delay(6);

  /* Move modules the resiger pointer to one of the axis data registers */
  Wire.beginTransmission(HMC5803L_Address);
  Wire.write(Axis);
  Wire.endTransmission();
 
  /* Read the data from registers (there are two 8 bit registers for each axis) */
  Wire.requestFrom(HMC5803L_Address, 2);
  Result = Wire.read() << 8;
  Result |= Wire.read();

  return Result;
}
