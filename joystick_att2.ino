int joystick_state=0;
int joystick_button=0;
void setup() {
  Serial.begin(9600);
}
 
  int cdf=0;
void loop() {
  int vrx, vry, sw;
 
  vrx = analogRead(A1);
  vry = analogRead(A2);
  sw = analogRead(A3);
 
  char buf[100];
  
  int adc=0;
  if(sw!=0) cdf=0;
  else cdf++;
  if(cdf>2)adc=1;
  
  sprintf(buf, "VRx=%d, VRy=%d, SW=%d, ADC=%d, state=%d", vrx, vry, sw, adc, joystick_state);
  Serial.println(buf);
  
  if(vrx>600) vrx=1;
  else if(vrx<400) vrx=-1;
  else vrx=0;
  
  if(vry>600) vry=1;
  else if(vry<400) vry=-1;
  else vry=0;
  
  if(vrx==-1 && vry==-1) joystick_state=1;
  else if(vrx==1 && vry==-1) joystick_state=2;
  else if(vrx==1 && vry==1) joystick_state=3;
  else if(vrx==-1 && vry==1) joystick_state=4;
  else if(vrx==1 && vry==0) joystick_state=23;
  else if(vrx==-1 && vry==0) joystick_state=14;
  else if(vrx==0 && vry==-1) joystick_state=12;
  else if(vrx==0 && vry==1) joystick_state=34;
  
  //if(sw) joystick_button=1;
  //else joystick_button=0;
}
