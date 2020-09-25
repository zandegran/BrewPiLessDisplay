# BrewPiLessDisplay
Secondary Display for BrewPiLess with ESP8266 with OLED

[![N|Image](https://github.com/zandegran/BrewPiLessDisplay/raw/master/blob/IMG-2001.gif)](https://github.com/zandegran/BrewPiLessDisplay/raw/master/blob/IMG-2001.gif)

## Parts

A wemos with oled like this one is needed.
[ESP8266 + 0.96 Inch OLED Module Development Board](https://www.banggood.com/Nodemcu-Wifi-And-NodeMCU-ESP8266-0_96-Inch-OLED-Module-Development-Board-p-1154759.html?gmcCountry=SE&currency=SEK&createTmp=1&utm_source=googleshopping&utm_medium=cpc_bgs&utm_content=xibei&utm_campaign=xibei-ssc-se-en-ecs-0724&gclid=Cj0KCQjwqrb7BRDlARIsACwGad5uszR3xhayIzRJiRdPt8raMLTFDafqT2JiHUJ_HgyBtHdqKfBVsCoaArw4EALw_wcB&cur_warehouse=CN)

## How to run
* Open in Arduino IDE
* Change the wifi credentials
* Change the ip of the BrewPiLess
* Upload to the device

## How to Add your own logo
- Resize the image size to fit inside the display
- Use [https://www.online-utility.org/image/convert/to/XBM](https://www.online-utility.org/image/convert/to/XBM) to convert image to XBMmformat
- Open the converted XBM file in any text editor
- Copy contents to [https://xbm.jazzychad.net](https://xbm.jazzychad.net) to retouch and smoothout edges, if needed
- Then copy the contents to `images.h`
