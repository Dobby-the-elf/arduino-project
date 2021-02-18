// Include application, user and local libraries

//--------------Caution !!!-------------
//map mode目前還沒有算距離

//SCL for 21
//SDA for 20
//Buzzer for A0

#include "SPI.h"
#include "TFT_22_ILI9225.h"
#include "bitmap1.h"
#include <SoftwareSerial.h>
SoftwareSerial ArduinoUno(A8,A9);//(3,2) (12,11)
#include <math.h>
#include <Wire.h>
#include <string.h>
#define HMC5803L_Address 0x1E
#define X 3
#define Y 7
#define Z 5

#define TFT_RST 4   
#define TFT_RS  5   //Mega/Uno
#define TFT_CS  53  // 53/10
#define TFT_SDI 51  // 51/11
#define TFT_CLK 52  // 52/13
#define TFT_LED 6   // CAUTION: be sure it is connected to 5V/3.3V


#define TFT_BRIGHTNESS 50 // Initial brightness of TFT backlight (optional)
#define BLACK   0x0000
#define BLUE    0x001F
#define SKYBLUE 0x4CBE//0x1D14
#define GRAY    0x4208
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define r1 27
#define r2 54
#define r3 81
#define deg_interval 10

int home_x=33;
int home_y=62;
int last_home_x=33;
int last_home_y=62;
const float home_lat PROGMEM =22.99216; //i.e. 勝利東寧
const float home_lon PROGMEM =120.21771; //-0.01 for both lon and lat
const float ee_lat PROGMEM =22.99679;// 假設的現在位置（電機系館）
const float ee_lon PROGMEM =120.22208;
float current_lat=22.99654;// 假設的現在位置（電機系館）
float current_lon=120.22201; //-0.01 no
int last_lon=160;
int last_lat=153;

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

int joystick_button=0;
int joystick_state=0;
int last_joystick_state=0;
int immediate_joystick_state=0;
int cdf=0;

int bright;
bool mode=0;

const int freq PROGMEM =3700;

void Print_Direction(int degree);
void tooFar();//display
void setRader();//display
void setHomeFont();
void initPattern();
void Init_HMC5803L(void);//compass
int HMC5803L_Read(byte Axis);//compass
void getPosition();//from wifi
void updateCalibrate(float x, float y);
void getJoystick();
void homeMode();
void settingMode();
void introMode();
void mapMode();

// Use hardware SPI (faster - on Uno: 13-SCK, 12-MISO, 11-MOSI)
TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED, TFT_BRIGHTNESS);

// Variables and constants
//---------------CAUTION!-------------
//uint16_t x, y;
//boolean flag = false;


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
  pinMode(A0, OUTPUT); // Set buzzer
  //-----for screen-------
  bright=80;
  tft.setBacklightBrightness(bright); //0~255
  initPattern();
}


void loop() {
    //home的用法其實是current相對home
    float float_x=((current_lon-home_lon)*111320*cos(current_lat/57.296)/900*81);
    float float_y=((home_lat-current_lat)*110574/900*81);
    home_x=int(float_x);
    home_y=int(float_y);
    dist=int(900/81*sqrt(0.01*float_x*float_x+0.01*float_y*float_y)*10);
    /*
    home_x=int((home_lon-current_lon)*111320*cos(current_lat/57.296)/900*81);
    home_y=int((home_lat-current_lat)*110574/900*81);
    //convert lon&lat into meter scale,and then into pixel scale
    
    dist=int(900/81*sqrt(0.01*home_x*home_x+0.01*home_y*home_y)*10);
    */
    /*
    Serial.print(home_x);
    Serial.print(" ");
    Serial.print(home_y);
    Serial.print(" ");
    Serial.println(dist);
    */
    
    String str = String(dist);
    str=str+"m  ";
    tft.drawText(75, 205, str);
    if(dist<850){
      //tft.drawText(70, 205, str);
    }
    else{
      //tft.drawText(63, 205, str);
      home_x=int(950./dist*home_x);
      home_y=int(950./dist*home_y);
    }
    /*
    Serial.print("home_x:");
    Serial.println(home_x);
    Serial.print("home_y:");
    Serial.println(home_y);
    */
    if(mode==1)mapMode();
    else{
      for(int deg=5; deg<360; deg+=deg_interval){
        //uint8_t use 1 byte w.r.t. int uses 2 bytes, but uint8_t range from 0~255 only
          getPosition();
          Print_Direction(deg);
          if(deg==185) setRader();
          getJoystick();
          setHomeFont();
          if (joystick_button){
            if (joystick_state==1) homeMode();
            //if (joystick_state==2) compassMode();
            if (joystick_state==3) introMode();
            if (joystick_state==4) settingMode();
          }
          if(mode==1)break;
      }
      if(dist>850)tooFar();
    }
}


//也許可以拿來紀錄足跡
void mapMode(){// 176pixel->950m (home_lon-current_lon)*111320*cos(current_lat/57.296)/900*81   ((home_lat-current_lat)*110574/900*81);
  tft.fillScreen(BLACK);
  tft.setFont(Terminal12x16);
  //---Caution: Do NOT use a lots of setFont it uses a lot of momory----
  tft.drawText(6, 6, "Home");
  tft.drawText(120, 200, "Intro");
  tft.drawText(2, 200, "Set");
  tft.setFont(Terminal6x8);
  tft.drawRGBBitmap(0,30, myBitmap1, 175, 80);
  tft.drawRGBBitmap(0,110, myBitmap2, 175, 80);
  int lon;
  int lat;
  while(1){
    getJoystick();
    getPosition();
    setHomeFont();
    float float_x=((current_lon-home_lon)*111320*cos(current_lat/57.296)/900*81);
    float float_y=((home_lat-current_lat)*110574/900*81);
    home_x=int(float_x);
    home_y=int(float_y);
    dist=int(900/81*sqrt(0.01*float_x*float_x+0.01*float_y*float_y)*10);
    String str = String(dist);
    str=str+"m  ";
    tft.drawText(75, 205, str);
    
    if (joystick_button){
      if (joystick_state==1) homeMode();
      //if (joystick_state==2) compassMode();
      if (joystick_state==3) introMode();
      if (joystick_state==4) settingMode();
    }
    
    if(mode==0)break;
    
    //tft.fillCircle(150+(current_lon-ee_lon)*111320*cos(current_lat/57.296)/950*176,155-(current_lat-ee_lat)*110574/950*176,2,RED);
    lon = int(160+(current_lon-ee_lon)*111320*cos(current_lat/57.296)/950*176);
    lat = int(153-(current_lat-ee_lat)*110574/950*176);
    last_lon = int(lon*0.1+last_lon*0.9);
    last_lat = int(lat*0.1+last_lat*0.9);
    tft.fillCircle(last_lon,last_lat,1,RED);
    
    //Serial.println(152+(current_lon-ee_lon)*111320*cos(current_lat/57.296)/950*176);
    //Serial.println(154-(current_lat-ee_lat)*110574/950*176);
  }
}



void getJoystick(){
  int vrx, vry, sw;
  vrx = analogRead(A1);
  vry = analogRead(A2);
  sw = analogRead(A3);
  char buf[100];
  
  if(vrx>600) vrx=1;//700
  else if(vrx<400) vrx=-1;//300
  else vrx=0;
  
  if(vry>600) vry=1;
  else if(vry<400) vry=-1;
  else vry=0;
  
  bool adc=0;
  if(sw!=0) cdf=0;
  else cdf++;
  if(cdf>2)adc=1;
  
  //sprintf(buf, "VRx=%d, VRy=%d, SW=%d, state=%d", vrx, vry, adc, joystick_state);
  //Serial.println(buf);
  
  if(vrx==-1 && vry==-1) joystick_state=1;
  else if(vrx==1 && vry==-1) joystick_state=2;
  else if(vrx==1 && vry==1) joystick_state=3;
  else if(vrx==-1 && vry==1) joystick_state=4;
  else if(vrx==1 && vry==0) joystick_state=23;
  else if(vrx==-1 && vry==0) joystick_state=14;
  else if(vrx==0 && vry==-1) joystick_state=12;
  else if(vrx==0 && vry==1) joystick_state=34;
  
  immediate_joystick_state = joystick_state;
  if(vrx==0 && vry==0) immediate_joystick_state = 0;
  
  if(adc) joystick_button=1;
  else joystick_button=0;
}


void settingMode(){
  tft.fillScreen(BLACK);
  tft.setFont(Terminal12x16);
  //---Caution: Do NOT use a lots of setFont it uses a lot of momory----
  tft.drawText(10, 6, "Setting");
  tft.drawText(70, 36, "Mode");
  tft.drawText(30, 56, "Rader");
  tft.drawText(110, 56, "Map");
  tft.drawText(10, 96, "Brightness");
  tft.drawText(6, 116, "-");
  tft.drawText(165, 116, "+");
  tft.drawLine(16, 116, 16+bright, 116, WHITE); 
  tft.drawLine(16, 116, 16+bright, 116, BLACK); 
  
  bool state=0;
  
  while(1){
    getJoystick();getJoystick();getJoystick();
    if (immediate_joystick_state==12 || immediate_joystick_state==34) state=!state;delay(120);
    if (immediate_joystick_state==23) {
      if(state==0) {
        mode=!mode;
        delay(80);
      }
      else if (bright<120)bright++;
    }
    if (immediate_joystick_state==14){
      if(state==0) {
        mode=!mode;
        delay(80);
      }
      else if (bright>50)bright--;
    }
    if(immediate_joystick_state != last_joystick_state){
      if (state==0) {
        tft.drawText(70, 36, "Mode", RED);
        tft.drawText(10, 96, "Brightness", WHITE);
        if (mode==0) tft.drawText(30, 56, "Rader", SKYBLUE);
        else tft.drawText(30, 56, "Rader", GRAY);
        if (mode==1) tft.drawText(110, 56, "Map", SKYBLUE);
        else tft.drawText(110, 56, "Map", GRAY);
      }
      else{
        tft.drawText(70, 36, "Mode", WHITE);
        tft.drawText(10, 96, "Brightness", RED);
      }
      last_joystick_state = immediate_joystick_state;
    }
    String str = String(bright);
    str=str+"  ";
    //tft.drawText(130, 96, str, BLACK);
    tft.drawText(130, 96, str, WHITE);
    tft.setBacklightBrightness(3*bright-130); 
    tft.drawLine(20, 124, 20+bright*2-100+4, 124, BLACK); 
    tft.drawLine(20, 124, 22+bright*2-100, 124, WHITE); 
    if(joystick_button) break;
  }
  if(mode==0)initPattern();
  if(mode==1)mapMode();
}

void homeMode(){
  tft.fillScreen(BLACK);
  while(1){
    tft.setFont(Terminal12x16);
    //---Caution: Do NOT use a lots of setFont it uses a lot of momory----
    tft.drawText(40, 15, "Home Info");
    tft.drawText(6, 60, "Distance");
    getPosition();
    String str = String(dist);
    tft.drawText(120, 60, str);
    tft.drawText(160, 60, "m");
    tft.drawText(6, 100, "Direction");
    float dir;
    dir = atan2(home_y,-home_x); //要算負的，但在算y(緯度)的時候算負的，所以要把他弄回來
    heading+=PI/2;
    if(dir < 0)dir += 2*PI;
    if(dir > 2*PI)dir -= 2*PI;
    
    if((dir*57.3 < 22.5)  && (dir*57.3 > 337.5 ))tft.drawText(56, 130,"     North");
    if((dir*57.3 > 22.5)  && (dir*57.3 < 67.5  ))tft.drawText(56, 130,"North-East");
    if((dir*57.3 > 67.5)  && (dir*57.3 < 112.5 ))tft.drawText(56, 130,"      East");
    if((dir*57.3 > 112.5) && (dir*57.3 < 157.5 ))tft.drawText(56, 130,"South-East");
    if((dir*57.3 > 157.5) && (dir*57.3 < 202.5 ))tft.drawText(56, 130,"     South");
    if((dir*57.3 > 202.5) && (dir*57.3 < 247.5 ))tft.drawText(56, 130,"South-West");
    if((dir*57.3 > 247.5) && (dir*57.3 < 292.5 ))tft.drawText(56, 130,"      West");
    if((dir*57.3 > 292.5) && (dir*57.3 < 337.5 ))tft.drawText(56, 130,"North-West");
    getJoystick();
    if(joystick_button) break;
  }
  if(mode==0)initPattern();
  if(mode==1)mapMode();
}

void introMode(){
  tft.fillScreen(BLACK);
  while(1){
    tft.setFont(Terminal12x16);
    tft.drawText(46, 12, "Personal");
    tft.drawText(33, 34, "Infomation");
    tft.drawText(6, 65, "Name");
    tft.drawText(6, 110, "Phone num");
    tft.drawText(6, 155, "Address");
    
    getJoystick();
    if(joystick_button) break;
  }
  if(mode==0)initPattern();
  if(mode==1)mapMode();
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
        if(abs(data-current_lat)<0.005 || n<4){
          current_lat=data;
        }
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
        if(abs(data-current_lon)<0.005 || n<4)current_lon=data;
        else Serial.println(" Error");
        n++;
      }
      data=0;
      i=0;
      }
      //delay(30);
  } 
}


void Print_Direction(int degree){
    float deg=float(degree)/57.296;
    
    float x=HMC5803L_Read(X);
    float y=HMC5803L_Read(Y);
    //read x, y process increase 0.3sec/loop
    
    updateCalibrate(x, y);
    heading = atan2(y-magneticCalibrateY, x-magneticCalibrateX);
    
    float declinationAngle = 0.1; //0.0716 theorically
    heading += declinationAngle;
    if(heading < 0)heading += 2*PI;
    if(heading > 2*PI)heading -= 2*PI;
    //headingDegrees = heading * 180/M_PI; 
    //Serial.println(headingDegrees);
    //Serial.println(heading);n
    
    tft.drawLine(88,110,88+79*cos(deg-float(deg_interval)/57.296),110+79*sin(deg-float(deg_interval)/57.296),BLACK); 
    tft.drawLine(88,110,88+79*cos(deg),110+79*sin(deg),GREEN); 
    
    heading+=PI/2;
    if(heading < 0)heading += 2*PI;
    if(heading > 2*PI)heading -= 2*PI;
    //-----------additional compass-------------176*220(use 160*20 as center)
    if(abs(last_heading-heading)>0.012){
      tft.drawTriangle(160-15*sin(last_heading),20-15*cos(last_heading),160-6*cos(last_heading),20+6*sin(last_heading),160+6*cos(last_heading),20-6*sin(last_heading),BLACK);
      tft.drawTriangle(160+15*sin(last_heading),20+15*cos(last_heading),160-6*cos(last_heading),20+6*sin(last_heading),160+6*cos(last_heading),20-6*sin(last_heading),BLACK);
      tft.drawTriangle(160-15*sin(heading),20-15*cos(heading),160-6*cos(heading),20+6*sin(heading),160+6*cos(heading),20-6*sin(heading),WHITE);
      tft.drawTriangle(160+15*sin(heading),20+15*cos(heading),160-6*cos(heading),20+6*sin(heading),160+6*cos(heading),20-6*sin(heading),RED);
      
      tft.fillCircle(88+last_home_x*cos(last_heading)+last_home_y*sin(last_heading),110-last_home_x*sin(last_heading)+last_home_y*cos(last_heading),3,BLACK);
      tft.fillCircle(88+home_x*cos(heading)+home_y*sin(heading),110-home_x*sin(heading)+home_y*cos(heading),3,RED);
      
      last_heading=heading;
      last_home_x=home_x;
      last_home_y=home_y;
    }
    else tft.fillCircle(88+last_home_x*cos(last_heading)+last_home_y*sin(last_heading),110-last_home_x*sin(last_heading)+last_home_y*cos(last_heading),3,RED);
    heading-=PI/2;
    if(heading < 0)heading += 2*PI;
    if(heading > 2*PI)heading -= 2*PI;

}


void initPattern(){
  tft.fillScreen(BLACK);
  setRader();
  tft.setFont(Terminal12x16);
  //---Caution: Do NOT use a lots of setFont it uses a lot of momory----
  tft.drawText(6, 6, "Home");
  tft.drawText(120, 200, "Intro");
  tft.drawText(2, 200, "Set");
  tft.setFont(Terminal6x8);
}

void setHomeFont(){
  if(joystick_state != last_joystick_state){
    //note that last_joystick_state are used in setHomeFont and settingMode for different state
    //Here for joystick_state, and settingMode for immediate_joystick_state
    tft.setFont(Terminal12x16);
    if (joystick_state==1) tft.drawText(6, 6, "Home", RED);
    else tft.drawText(6, 6, "Home", WHITE);
    if (joystick_state==3) tft.drawText(120, 200, "Intro", RED);
    else tft.drawText(120, 200, "Intro", WHITE);
    if (joystick_state==4) tft.drawText(2, 200, "Set", RED);
    else tft.drawText(2, 200, "Set", WHITE);
    tft.setFont(Terminal6x8);
    last_joystick_state = joystick_state;
  }
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
  tft.setFont(Terminal6x8);
  tft.drawText(88-27-6, 110, "300");
  tft.drawText(88-54-6, 110, "600");
  tft.drawText(88-81-6, 110, "900");
}

void tooFar(){//可能會需要Timer.h
  setRader();
  for(uint8_t i=0;i<20;i++){
    tft.setBackgroundColor(RED);
    //tft.setFont(Terminal6x8);
    //Font has already set by setRader function
    tft.setFont(Terminal12x16);
    tft.drawText(50, 100, "Too far",BLACK);
    tft.setBackgroundColor(BLACK);
    tft.drawText(50, 100, "Too far",BLACK);
  }
  for(int i=0;i<2;i++){
    tone(A0, 3700); // Send 3.7KHz sound signal...
    delay(100);     // ...for X sec
    noTone(A0);     // Stop sound...
    delay(100); 
  }
  tft.setFont(Terminal6x8);
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
