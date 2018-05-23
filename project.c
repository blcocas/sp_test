#include <wiringPi.h>
#include <lcd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
//#include <softPwm.h>

#define MAXTIMINGS    85
#define DHTPIN        7  //wPi pin. physical num 7
#define LCD_RS 11
#define LCD_E 10
#define LCD_D4 6
#define LCD_D5 5
#define LCD_D6 4
#define LCD_D7 1
#define LED 27
#define VIB 26


void INT_handler(int);
void init();
int led_toggle();
int vibration_input();


int dht11_dat[5] = { 0, 0, 0, 0, 0 };


void read_dht11_dat(int lcd) {
    uint8_t laststate = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0, i;
    float f;

    dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

    pinMode(DHTPIN, OUTPUT);
    digitalWrite(DHTPIN, LOW);
    delay(18);
    digitalWrite(DHTPIN, HIGH);

    delayMicroseconds(40);

    pinMode(DHTPIN, INPUT);

    for (i = 0; i < MAXTIMINGS; i++) {
        counter = 0;
        while(digitalRead(DHTPIN) == laststate) {
            counter++;
            delayMicroseconds(1);
            if (counter ==255) break;
        }

        laststate = digitalRead(DHTPIN);

        if (counter == 255)
            break;

        if ((i >= 4) && (i % 2 == 0)) {
            dht11_dat[j/8] <<= 1;
            if (counter > 30)       // this check is how long the pulse width is low.
                dht11_dat[j/8] |= 1;
            j++;
        }
    }

    if ((j >= 40) && (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF ))) {
        f = dht11_dat[2] * 9. / 5. + 32;
        lcdPosition(lcd, 0, 0);
        lcdPrintf(lcd, "Humidity: %d.%d %%\n", dht11_dat[0], dht11_dat[1]);
        lcdPosition(lcd, 0, 1);
        lcdPrintf(lcd, "Temp: %d.0 C", dht11_dat[2]); //Uncomment for Celcuis

    }
    return;
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

void temperature(){
  int lcd;
  if(lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0)){
    return;
  }

  read_dht11_dat(lcd);
  return;
}

int main(){

    int mode;

    if(wiringPiSetup() == -1){
      printf("Fail to setup WiringPi\n");
      exit(1);
    }


    pinMode(LED, OUTPUT);
    pinMode(VIB, INPUT);

    while(1){
        mode = vibration_input();

        switch(mode){
          case 1 : printf("vibration mode 1\n");
    	       break;
          case 2 : printf("vibration mode 2 : ");
    	       led_toggle() ? printf("led light on\n") : printf("led light off\n");
    	       break;
          // case 3 : printf("vibration mode 3 : ");
    	    //    servo_toggle() ? printf("door is open\n") : printf("door is closed\n");
    	    //   break;
          case 3 : printf("vibration mode 4 : ");
    	       temperature();
    	       break;
          default : printf("vibration mode default\n");
        }
    }

      return 0;

}
