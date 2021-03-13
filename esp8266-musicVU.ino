/*------------------------------------------------------------------------------
2021年3月13日
  Author: Cisco • A C R O B O T I C 
  Platforms: ESP32
  Language: C++/Arduino
  File: vu_meter.ino
  ------------------------------------------------------------------------------
  Description:
  Code for YouTube video tutorial demonstrating how to build a Volume Unit (VU) 
  meter using an ESP32, a pair of NeoPixel/WS2812B LED sticks, and a MAX9814 mic. 
  The audio signal is read from the mic, and a set of LEDs proportional to the 
  amplitude lights up using a wide range of colors:
  https://youtu.be/xvG_kvhBECc
  ------------------------------------------------------------------------------
  Do you like my work? You can support me:
  https://patreon.com/acrobotic
  https://paypal.me/acrobotic
  https://buymeacoff.ee/acrobotic
  ------------------------------------------------------------------------------
  Please consider buying products and kits to help fund future Open-Source 
  projects like this! We'll always put our best effort in every project, and 
  release all our design files and code for you to use. 
  https://acrobotic.com/
  https://amazon.com/shops/acrobotic
  ------------------------------------------------------------------------------
  License:
  Please see attached LICENSE.txt file for details.
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
基于esp8266的音乐vu（音乐频谱）
声音采用MAX9814
频谱灯采用WS2812B(8位)//可在程序自定义你的灯珠数量


引脚
MAX9814   ESP8266

AR        GND
OUT       A0
VDD       5V/3.3V
GND       GND
引脚说明:
          OUT输出
GAIN      悬空增益为60db GND 增益为50dbVDD 增益为40db 
AR        悬空启动/释放比为1:500 
             GND启动/释放比为1:2000 
             vDD启动/释放比为1:4000
WS2812   ESP8266
VCC      5V
GND      GND
IN/DI    D4


开发板型号选择wemos D1
------------------------------------------------------------------------------*/
#include<FastLED.h>
#include<MegunoLink.h>
#include<Filter.h>  

#define N_PIXELS  16//灯珠数量
#define MIC_PIN   A0//A0(看丝印)
#define LED_PIN   2// D4(看丝印)

//以下参数可以根据您的音频级别进行调整
#define NOISE 580  //声音
#define TOP   (N_PIXELS+2)
#define LED_TYPE  WS2812B//灯板型号
#define BRIGHTNESS  100     
#define COLOR_ORDER GRB


CRGB leds[N_PIXELS];

int lvl = 0, minLvl = 0, maxLvl = 136; //根据需要调整麦克风接收“最小值”和“最大值”

//音频过滤实现平滑原始音频信号
ExponentialFilter<long> ADCFilter(5,0);

void music_vu()
    {
  
          int n, height;
          n = analogRead(MIC_PIN);
          n = abs(1023 - n);
          n = (n <= NOISE) ? 0 : abs(n - NOISE);
          ADCFilter.Filter(n);
          lvl = ADCFilter.Current();
          height = TOP * (lvl - minLvl) / (long)(maxLvl - minLvl);
          if(height < 0L) height = 0;
          else if(height > TOP) height = TOP;
          for(uint8_t i = 0; i < N_PIXELS; i++) 
          {
          if(i >= height) leds[i] = CRGB(0,0,0);
          else leds[i] = Wheel( map( i, 0, N_PIXELS-1, 30, 150 ) );
              }
  FastLED.show();
     }


void setup() {

  Serial.begin(115200);

  FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds,N_PIXELS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {

music_vu();

}

//调整颜色设置
CRGB Wheel(byte WheelPos) {

  if(WheelPos < 85)//初始颜色
    return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;//终止颜色
    return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
