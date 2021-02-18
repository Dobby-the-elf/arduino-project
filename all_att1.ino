// Include application, user and local libraries

//----------- by Dobby ------------
//Vcc for Vcc
//GND gor GND
//SCL for A5/Mega:21
//SDA for A4/Mega:20

#include "SPI.h"
#include "TFT_22_ILI9225.h"
#include <SoftwareSerial.h>
SoftwareSerial ArduinoUno(12,11);//(3,2)
#include <math.h>
#include <Wire.h>
#include <string.h>
#define HMC5803L_Address 0x1E
#define X 3
#define Y 7
#define Z 5

#define TFT_RST 8   
#define TFT_RS  9   //Mega/Uno
#define TFT_CS  10  // 53/10
#define TFT_SDI 51  // 51/11
#define TFT_CLK 52  // 52/13
#define TFT_LED 0   // CAUTION: be sure it is connected to 5V/3.3V


#define TFT_BRIGHTNESS 200 // Initial brightness of TFT backlight (optional)
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define r1 27
#define r2 54
#define r3 81

int home_x=33;
int home_y=62;
float home_lat=22.99216; //i.e. 勝利東寧
float home_lon=120.21771;
float current_lat=22.98654;// 假設的現在位置（電機系館）
float current_lon=120.21201;// -0.01

float max_x=-999.9;
float max_y=-999.9;
float min_x=999.9;
float min_y=999.9;
float magneticCalibrateX;
float magneticCalibrateY;

float heading;
float last_heading=0;

int dist;


int arr[9];
int digit1=0;
int digit2=0;
int i=0;  //非小數位
int receive1;
float data;
int n=0;

void tooFar();//display
void setRader();//display
void Init_HMC5803L(void);//compass
int HMC5803L_Read(byte Axis);//compass
void getPosition();//from wifi
void updateCalibrate(float x, float y);

// Use hardware SPI (faster - on Uno: 13-SCK, 12-MISO, 11-MOSI)
TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED, TFT_BRIGHTNESS);

// Variables and constants
//---------------CAUTION!-------------
//uint16_t x, y;
//boolean flag = false;

void Print_Direction(float deg){
    //-----------for screen------------
    tft.drawText(88-27-6, 110, "300");
    //Since "300" is too close to the center（會被擦掉）
    
    //-----------for compass------------
    float x=HMC5803L_Read(X);
    float y=HMC5803L_Read(Y);
    //read x, y process increase 0.3sec/loop
    
    updateCalibrate(x, y);
    heading = atan2(y-magneticCalibrateY, x-magneticCalibrateX);
    
    float declinationAngle = 0.0716;
    heading += declinationAngle;
    if(heading < 0)heading += 2*PI;
    if(heading > 2*PI)heading -= 2*PI;
    //headingDegrees = heading * 180/M_PI; 
    //Serial.println(headingDegrees);
    //Serial.println(heading);n
    
    tft.drawLine(88,110,88+79*cos(deg),110+79*sin(deg),GREEN); 
    
    heading+=PI/2;
    if(heading < 0)heading += 2*PI;
    if(heading > 2*PI)heading -= 2*PI;
    //-----------additional compass-------------176*220(use 160*20 as center)
    if(abs(last_heading-heading)>0.024){
      tft.drawTriangle(160-15*sin(last_heading),20-15*cos(last_heading),160-6*cos(last_heading),20+6*sin(last_heading),160+6*cos(last_heading),20-6*sin(last_heading),BLACK);
      tft.drawTriangle(160+15*sin(last_heading),20+15*cos(last_heading),160-6*cos(last_heading),20+6*sin(last_heading),160+6*cos(last_heading),20-6*sin(last_heading),BLACK);
      tft.drawTriangle(160-15*sin(heading),20-15*cos(heading),160-6*cos(heading),20+6*sin(heading),160+6*cos(heading),20-6*sin(heading),WHITE);
      tft.drawTriangle(160+15*sin(heading),20+15*cos(heading),160-6*cos(heading),20+6*sin(heading),160+6*cos(heading),20-6*sin(heading),RED);
      last_heading=heading;
    }
    heading-=PI/2;
    if(heading < 0)heading += 2*PI;
    if(heading > 2*PI)heading -= 2*PI;
  
    
    for(uint8_t k=0;k<3;k++){
      tft.fillCircle(88+home_x*cos(heading)+home_y*sin(heading),110-home_x*sin(heading)+home_y*cos(heading),3,RED);
    }
    //delay(10);
    
    tft.drawLine(88,110,88+79*cos(deg),110+79*sin(deg),BLACK); 
    
    tft.fillCircle(88+home_x*cos(heading)+home_y*sin(heading),110-home_x*sin(heading)+home_y*cos(heading),3,BLACK);

}


//---------------useful function-----------
//tft.setBackgroundColor(COLOR_BLACK);
//tft.clear();
//tft.drawRectangle(0, 0, tft.maxX() - 1, tft.maxY() - 1, COLOR_WHITE);
//tft.setFont(Terminal6x8);
//tft.setFont(Terminal11x16);
//tft.setFont(Terminal12x16);
//tft.drawText(10, 10, "hello!");
//Serial.print(F("string")); this saves a lot of memory

//-------------memory(SRAM) optimiztion-------------
// use function call instead of repeated code
// use local variable instead of golbal
// find function that uses a lot of memory


// Setup
void setup() {
  tft.begin();
  Serial.begin(9600);
  ArduinoUno.begin(4800);
  Wire.begin();
  /* Initialise the module */
  Init_HMC5803L();
  //-----for screen-------
  tft.fillScreen(BLACK);
  setRader();
  tft.setFont(Terminal12x16);
  //---Caution: Do NOT use a lots of setFont it uses a lot of momory----
  tft.drawText(6, 6, "Home");
  tft.drawText(120, 200, "Intro");
  tft.setFont(Terminal6x8);
  tft.drawText(2, 205, "Distance:");
  tft.drawText(94, 205, "m");
}


void loop() {
    home_x=int((home_lon-current_lon)*111320*cos(current_lat/57.296)/900*81);
    home_y=int((home_lat-current_lat)*110574/900*81);
    //convert lon&lat into meter scale,and then into pixel scale
  
    dist=int(900/81*sqrt(0.01*home_x*home_x+0.01*home_y*home_y)*10);
    String str = String(dist);
    //string dist("Dobby")
    if(dist<999){
      tft.drawText(70, 205, str);
    }
    else{
      tft.drawText(63, 205, str);
      home_x=int(950./dist*home_x);
      home_y=int(950./dist*home_y);
      tooFar();
    }
  
    /*
    Serial.print("home_x:");
    Serial.println(home_x);
    Serial.print("home_y:");
    Serial.println(home_y);
    */
    setRader();
    tft.drawText(88-54-6, 110, "600");
    tft.drawText(88-81-6, 110, "900");
    
    for(int deg=1; deg<360; deg+=10){
      //uint8_t use 1 byte w.r.t. int uses 2 bytes, but uint8_t range from 0~255 only
        //-----for screen-------
        getPosition();
        Print_Direction(float(deg)/57.296);
        //Serial.println(round(27*cos(float(deg)/57.3)));
      
    }
}

void getPosition(){
  while(ArduinoUno.available()>0){
    receive1=ArduinoUno.read()-48;
    if(receive1!=(-11)){  //非截止訊號
      if(receive1==(-2)){ ////接收到小數點
        digit1=i;          //整數共幾位(index表示會差1)
      }       
      else{ 
        arr[i]=receive1;
        i++;
        digit2=i;//紀錄存取目前位數的下一個位置

      }
    }
    if(receive1==(-11)){//截止訊號
      i=0;
      while(i<digit2){
        if(i<digit1){
          data*=10.;
          data+=float(arr[i]);
          }
        else{
          data+=float(arr[i])*(pow(10.,digit1-i-1));
        }
        i++;   
      }
      if(n%2==0){
        Serial.print("\nlatitude:");
        Serial.println(data,6);
        if(abs(data-current_lat)<0.005 || n<2)current_lat=data;
        else Serial.println(" Error");
        //如果十秒的時間內移動超過500m會視為error
        n++;
        data=0;
        i=0;
        break;
      }
      if(n%2==1){
        Serial.print("longitude:");
        Serial.println(data,6);
        if(abs(data-current_lon)<0.005 || n<2)current_lon=data;
        else Serial.println(" Error");
        n++;
      }
      data=0;
      i=0;
      }
      //delay(30);
  } 
}


void tooFar(){//可能會需要Timer.h
  setRader();
  tft.fillCircle(88+home_x*cos(heading)+home_y*sin(heading),110-home_x*sin(heading)+home_y*cos(heading),3,RED);
  for(uint8_t i=0;i<20;i++){
    tft.setBackgroundColor(RED);
    //tft.setFont(Terminal6x8);
    //Font has already set by setRader function
    tft.setFont(Terminal12x16);
    tft.drawText(50, 100, "Too far",BLACK);
    tft.setBackgroundColor(BLACK);
    tft.drawText(50, 100, "Too far",BLACK);
  }
  tft.fillCircle(88+home_x*cos(heading)+home_y*sin(heading),110-home_x*sin(heading)+home_y*cos(heading),3,BLACK);
  tft.setFont(Terminal6x8);
}


void setRader(){
  //tft.Set_Draw_color(GREEN);
  tft.drawLine(88,110+67,88,110+87,GREEN); 
  tft.drawLine(88+67,110,88+87,110,GREEN);
  tft.drawLine(88,110-87,88,110-67,GREEN); 
  tft.drawLine(88-87,110,88-67,110,GREEN);
  tft.drawCircle(88,110,r1,GREEN);
  tft.drawCircle(88,110,r2,GREEN);
  tft.drawCircle(88,110,r3,GREEN);
  tft.setBackgroundColor(BLACK);
  tft.drawText(88-27-6, 110, "300");
  tft.drawText(88-54-6, 110, "600");
  tft.drawText(88-81-6, 110, "900");
}


void Init_HMC5803L(void){
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
int HMC5803L_Read(byte Axis){
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

void updateCalibrate(float x, float y){
  if(x>max_x)max_x=x;
  if(y>max_y)max_y=y;
  if(x<min_x)min_x=x;
  if(y<min_y)min_y=y;
  magneticCalibrateX=0.5*(max_x+min_x);
  magneticCalibrateY=0.5*(max_y+min_y);
}
