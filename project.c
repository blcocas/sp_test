#include <wiringPi.h>
#include <lcd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/kdev_t.h>
#include <string.h>

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
#define LED_DEV_FILE "/dev/ledtest_dev"



void INT_handler(int);
void init();
int led_toggle();
int vibration_input();
void button_toggle();

static char receive[7];
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
  delay(700);
  while(!(signal = digitalRead(VIB))){}
  printf("First input!\n");
  start = clock();
  count++;
  signal = 0;
  delay(700);
  while(difftime(clock(), start) < 3000000){
    if(signal = digitalRead(VIB)) {
      count++;
      printf("%d input!\n", count);
      signal = 0;
      delay(700);
    }
  }
  printf("vibration number is %d\n", count);
  return count;
}

int led_toggle(){
  static int led_state = 0;//state 0 is led off, 1 is led on
  int fd = 0;

  if((fd = open(LED_DEV_FILE, O_RDWR)) < 0) exit(1);
  sleep(2);

  if(led_state == 1){
    write(fd, "led_off", strlen("led_off")+1);
    led_state = 0;
    sleep(2);
  }else{
    write(fd, "led_on", strlen("led_on")+1);
    led_state = 1;
    sleep(2);
  }
  close(fd);
  return led_state;
}

void button_toggle(){

  int fd = 0;
  char buff[12];
  int on_off = 0;
  int pid = 0;
  int lcd = 0;

  fd = open("LED_DEV_FILE",O_RDONLY);

  while(1){ //
    read(fd,buff,12);

    if(strcmp(buff,"btn_toggle") == 0){
      switch(on_off){
        case 0 :
          pid = fork();
          on_off = 1;
          lcd = lcd_set();
          lcdPosition(lcd, 0, 0);
          lcdPrintf(lcd, "Actived");
          break;

        case 1 :
          char str1[20] = "kill -9 ";
          char str2[10];
          sprintf(str2,"%d",pid);
          strcat(str1,str2);
          printf("%s",str1);
          system(str1);
          on_off = 0;
          lcd = lcd_set();
          lcdPosition(lcd, 0, 0);
          lcdPrintf(lcd, "Deactived");
          break;

      }
    }
    if(pid == 0) {
      printf("child(%d), break!\n",getpid());
      break;
    }
  }
}

int lcd_set(){
  int lcd;
  if(lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0)){
    return lcd;
  }
  return lcd;
}

int main(){

    int mode;
    int lcd;
    int fd = 0;
    if(wiringPiSetup() == -1){
      printf("Fail to setup WiringPi\n");
      exit(1);
    }

    pinMode(LED, OUTPUT);
    pinMode(VIB, INPUT);
    if((fd = open(LED_DEV_FILE, O_RDONLY))<0){
      printf("Fail to open file\n");
    }
    


    button_toggle();

    while(1){
        mode = vibration_input();

        switch(mode){
          case 1 : printf("vibration mode 1\n");
             if(led_toggle()){
               printf("led light on\n");
               lcd = lcd_set();
               lcdPosition(lcd, 0, 0);
               lcdPrintf(lcd, "LED LIGHT ON!");
             }
             else{
               printf("led light off\n");
               lcd = lcd_set();
               lcdPosition(lcd, 0, 0);
               lcdPrintf(lcd, "LED LIGHT OFF!");
             }
    	       break;
          case 2 : printf("vibration mode 2 : ");
            printf("Temperature & Humidity");
            lcd = lcd_set();
            read_dht11_dat(lcd);
    	    break;
          case 3 : printf("vibration mode 3 : ");
    	       break;
          default : printf("vibration mode default\n");
        }
    }

      return 0;

}
