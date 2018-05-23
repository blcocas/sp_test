#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <softPwm.h>

#define LED 1 //gpio 18
#define VIB 4 //gpio 23
#define SERVO 5 //gpio 24
#define TEMP 6 //gpio 25

void INT_handler(int);
void init();
int led_toggle();
int servo_toggle();
void temperature();
int vibration_input();

void INT_handler(int sig){
  printf("\nINT occured\n");
  init();
  exit(0);
}

void init(){
  digitalWrite(LED, LOW);
}

int main (){
  int mode;
  clock_t vibTime;//previous vibration input time

  if(wiringPiSetup() == -1){
    printf("Fail to setup WiringPi\n");
    exit(1);
  }
  pinMode(LED, OUTPUT);
  pinMode(VIB, INPUT);
  softPwmCreate(SERVO, 0, 200);//pin number, initial value, term

  while(1){
    mode = vibration_input();

    switch(mode){
      case 1 : printf("vibration mode 1\n");
	       break;
      case 2 : printf("vibration mode 2 : ");
	       led_toggle() ? printf("led light on\n") : printf("led light off\n"); 
	       break;
      case 3 : printf("vibration mode 3 : ");
	       servo_toggle() ? printf("door is open\n") : printf("door is closed\n");
	      break;
      case 4 : printf("vibration mode 4 : ");
	       temperature();
	       break;
      default : printf("vibration mode default\n");
   }
  }

  return 0;
}

int vibration_input(){
  int signal = 0;
  int count = 0;
  clock_t start;

  printf("waiting input\n");
  delay(500);
  while(!(signal = digitalRead(VIB))){}
  printf("First input!\n");
  start = clock();
  count++;
  signal = 0;
  delay(500);
  while(difftime(clock(), start) < 3000000){
    if(signal = digitalRead(VIB)) {
      count++;
      printf("%d input!\n", count);
      signal = 0;
      delay(500);
    }
  }
  printf("vibration number is %d\n", count);
  return count;
}

int led_toggle(){
  static int led_state = 0;//state 0 is led off, 1 is led on

  if(led_state == 1){
    digitalWrite(LED, LOW);
    led_state = 0;
  }else{
    digitalWrite(LED, HIGH);
    led_state = 1;
  }
  return led_state;
}

int servo_toggle(){
  static int servo_state = 0;

  if(servo_state == 0){
    softPwmWrite(SERVO, 15); //0 degree
    servo_state = 1;
  }else{
    softPwmWrite(SERVO, 24); //90 degree
    servo_state = 0;
  }
  return servo_state;
}

void temperature(){
 /* static int data[5] = {0, };
  int laststate = HIGH;
  unsigned int counter = 0;
  unsigned int j = 0, i;

  pinMode(TEMP, OUTPUT);
  digitalWrite(TEMP, LOW);
  delay(18);
  digitalWrite(TEMP, HIGH);
  delayMicroseconds(40);
  pinMode(TEMP, INPUT);

  for(i = 0; i < 85; i++){
    counter = 0;
    while(digitalRead(TEMP) == laststate){
      counter++;
      delay(1);
      if(counter == 255) break;
    }
    laststate = digitalRead(TEMP);
    if(counter == 255) break;

    if((i>=4) && (i%2 == 0)){
      data[j/8] <<=1;
      if(counter > 16) data[j/8] |=1;
      j++;
    }
  }
  if((j>=40)&&(data[4] == ((data[0] + data[1] + data[2] + data[3]) &0xFF))){
    printf("Humidity = %d.%d %% Temperature = %d.%d C\n", data[0], data[1], data[2], data[3]);
  }else{
    printf("Data not good, skip\n");
  }
  return;*/
}
