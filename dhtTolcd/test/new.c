#include <wiringPi.h>
#include <lcd.h>
#include <stdio.h>
#include <stdint.h>
#define MAXTIMINGS    85
#define DHTPIN        7  //wPi pin. physical num 7
#define LCD_RS 11
#define LCD_E 10
#define LCD_D4 6
#define LCD_D5 5
#define LCD_D6 4
#define LCD_D7 1

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
        // printf("Humidity = %d.%d %% Temperature = %d.%d C (%.1f F)\n",
        // dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f);
        lcdPosition(lcd,0,0);
        lcdPuts(lcd, "HELLO WORLD");
    } else {
        // printf("Data not good, skip \n");
				lcdPosition(lcd,0,1);
				lcdPuts(lcd, "HELLO WORLD2");
    }
}

int main(){
	int lcd;
	wiringPiSetup();

	if(lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0)){
		return -1;
	}
	while ( 1 )
	{
			read_dht11_dat(lcd);
			delay( 1000 );
	}


	getchar();

}
