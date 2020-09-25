# BrewPiLessDisplay
Secondary Display for BrewPiLess with ESP8266 with OLED

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
