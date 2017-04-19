#include "Wire.h"
#include <TimerOne.h>
#include <MsTimer2.h>
#include <EEPROM.h>
#include "TM1636.h"
#include "TickTockShield.h"
#define LED3  4
#define LED2  3
#define LED1  2
#define KEY_3  11
#define KEY_2  10
#define KEY_1  9
TickTockShield ticktockshield;
int num;
extern int8_t disp[4];//store the index of the 4 digit to display.
int check;
unsigned char second = 0;
boolean flag;
boolean flag_error;

void setup() {
  #ifdef DEBUG
  Serial.begin(9600);
  #endif
  ticktockshield.init();
  init_pins();
  delay(1000);
  MsTimer2::set(250, Timer2ISR);//Timer2 generates an interrupt every 250ms
  MsTimer2::start();//Timer2 starts counting
}
void loop() {
  if(check<3) clockStart();
  else ticktockshield.alarming();
}

void clockStart(){
  if(flag){
    if(ticktockshield.scanKey() == KEY_1){
      flag = 0;
      if(digitalRead(LED1)== HIGH&&!flag_error){
        increaseNum();
        unWrong();
        check = 0;
      }
      else if(!flag_error){
        flag_error = 1;
        wrong();
        check++;
      }
    }
    else if(ticktockshield.scanKey() == KEY_2){
      flag = 0;
      if(digitalRead(LED2)== HIGH&&!flag_error){
        increaseNum();
        unWrong();
        check = 0;
      }
      else if(!flag_error){
        flag_error = 1;
        wrong();
        check++;
      }
    }
    else if(ticktockshield.scanKey() == KEY_3){
      flag = 0;
      if(digitalRead(LED3)== HIGH&&!flag_error){
        increaseNum();
        unWrong();
        check = 0;
      }
      else if(!flag_error){
        flag_error = 1;
        wrong();
        check++;
      }
    }
    else flag_error = 0;
  }
}

void init_pins(){
  num = 0;
  flag_error = 0;
  pinMode(LED3, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(KEY_1, INPUT);
  digitalWrite(KEY_1, HIGH);
  pinMode(KEY_2, INPUT);
  digitalWrite(KEY_2, HIGH);
  pinMode(KEY_3, INPUT);
  digitalWrite(KEY_3, HIGH);
  disp[0]=0;
  disp[1]=0;
  disp[2]=0;
  disp[3]=0;
  tm1636.display(disp);
}

inline void turnOn(int pinLED){
  digitalWrite(pinLED, HIGH);
}
inline void turnOff(int pinLED){
  digitalWrite(pinLED, LOW);
}
inline void turnOnAll(){
  turnOn(LED3);
  turnOn(LED2);
  turnOn(LED1);
}
inline void turnOffAll(){
  turnOff(LED3);
  turnOff(LED2);
  turnOff(LED1);
}

void increaseNum(){
  disp[3]++;
  if(disp[3]==10){
    disp[3]=0;
    disp[2]++;
  }
  if(disp[2]==10){
    disp[2]=0;
    disp[1]++;
  }
  tm1636.display(disp);
}
void wrong(){
  disp[0]++;
  tm1636.display(disp);
}
void unWrong(){
  disp[0] = 0;
  tm1636.display(disp);
}

/*void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(ALARM_BUZZER, HIGH);
    delayMicroseconds(tone);
    digitalWrite(ALARM_BUZZER, LOW);
    delayMicroseconds(tone);
  }
}*/

void Timer2ISR(){
  second ++;
  if(check<3){
    if(second%6==1){
      int LED = 2+rand()%3;
      turnOn(LED);
      flag = 1;
    }
    else if(second%6==0){
      turnOffAll();
      second = 0;
    }
  }else{
    if(second%2==1){
      turnOnAll();
    }
    else if(second%2==0){
      turnOffAll();
      second = 0;
    }
  }
}

