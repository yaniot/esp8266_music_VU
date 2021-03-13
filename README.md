# esp8266_music_VU
声音模块采用MAX9814
频谱灯采用WS2812B(8位)//可在程序自定义你的灯珠数量
引脚
MAX9814   ESP8266
AR        GND
OUT       A0
VDD       5V/3.3V
GND       GND
引脚说明:
OUT       输出
GAIN      悬空增益为60db GND 增益为50dbVDD 增益为40db 
AR        悬空启动/释放比为1:500 
             GND启动/释放比为1:2000 
             vDD启动/释放比为1:4000
WS2812   ESP8266
VCC      5V
GND      GND
IN/DI    2（D4）此引脚为GPIO2

开发板型号选择wemos D1
