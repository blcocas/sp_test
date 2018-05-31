#include <gpio.h>

#define GPIO_NUMBER 11

int gpio_nreset = GPIO_NUMBER;
static int cs4271_probe(struct snd_soc_codec *codec)
{
    struct cs4271_private *cs4271 = snd_soc_codec_get_drvdata(codec);
    struct cs4271_platform_data *cs4271plat = codec->dev->platform_data;
    int ret;
    int gpio_nreset = -EINVAL;

//GPIO 핀을 초기화
        if (gpio_request(gpio_nreset, "CS4271 Reset"))
            gpio_nreset = -EINVAL;
    if (gpio_nreset >= 0) {
        /* Reset codec */

// GPIO를 입력(input)모드로 변경
        gpio_direction_input(gpio_nreset);

// 나노 초만큼 딜레이를 줌
        udelay(1);

// GPIO 핀을 읽음
        if(gpio_get_value(gpio_nreset) == 1)
              printk(" Oh! GPIO PIN is one");
        /* Give the codec time to wake up */
        udelay(1);
    }
