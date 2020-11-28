#include <math.h>  
int A=240;
void setup() {
Serial.begin(2000000); // Baud rate. Please match the baud rate in serial monitor to this value to get correct output
}

void loop() {
  float f=60; // frequency of source
  float fs=1000; // sampling frequency
  float w=2*PI*f; // angular frequency
  float pow_sum=0;
  float prev_V=0;
  float prev_I=0;
  float prev_t=0;
  float I_sum=0;
  float V_sum=0;
  float max_V=0;
  float max_I=0;
  float max_Vt=0;
  float max_It=0;
  if(Serial.available()>0){
     int input=Serial.read(); //to increment or decrement voltage amplitude so that code can be tested in reaal time
     if(input=='h') A+=5; // press 'h' in serial monitor to increment amplitude by 5 units
     if(input=='l') A-=5; // press 'l' in serial monitor to decrement amplitude by 5 units
    }
  for(float t=0; t<=1/f; t+=1/fs){
    float V=A*sin(w*t); // sinuosoidal voltage
    float I=5*sin(w*t-0.571); // sinuosoidal current
    V_sum+=(t-prev_t)*0.5*(V*V+prev_V*prev_V); // Integrating voltage for RMS
    I_sum+=(t-prev_t)*0.5*(I*I+prev_I*prev_I); // Integrating current for RMS
    pow_sum+=(t-prev_t)*0.5*(V*I+prev_V*prev_I); // Integrating product of current and voltage
    if(max_V<V){  // for finding if positive or negative phase difference
       max_V=V;
       max_Vt=t;
      }
    if(max_I<I){
       max_V=V;
       max_It=t;
      }
    prev_V=V; // current becomes previous for next cycle
    prev_I=I;
    prev_t=t;
  }
  float real_pow=pow_sum*f;
  float RMS_V=sqrt(V_sum*f);
  float RMS_I=sqrt(I_sum*f);
  float pow_fac=real_pow/(RMS_V*RMS_I);
  float phase=acos(pow_fac)*180/PI;
  float react_pow=(RMS_V*RMS_I)*sin(acos(pow_fac));
  if(max_Vt>max_It) phase*=-1;
  Serial.print("real power = ");
  Serial.println(real_pow);
  Serial.print("reactive power = ");
  Serial.println(react_pow);
  Serial.print("RMS voltage = ");
  Serial.println(RMS_V);
  Serial.print("RMS current = ");
  Serial.println(RMS_I);
  Serial.print("power factor = ");
  Serial.println(pow_fac);
  Serial.print("phase difference = ");
  Serial.println(phase);
  delay(10); // delay so that output is visible
}
