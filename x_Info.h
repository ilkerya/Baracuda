//C Standarts
http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf

git add .
git commit -m "Comment"
git push https://github.com/ilkerya/Baracuda.git master

First Time
git init
git add README.md

GitHub Bash Commands
Adding new file
https://help.github.com/articles/adding-a-file-to-a-repository-using-the-command-line/#platform-windows

git add .
git commit -m "Add existing file"
// For Common git
git push https://github.com/ilkerya/Baracuda master


In case in the first push gives error use below command
git remote add origin remote repository URL
git push origin master
git push --force https://github.com/ilkerya/Baracuda master --force

DataLogger Arduino Atmega/Arduino Due // Arduino IDE / Microchip Studio 7.0
CPU Modules : Both Can Used as Selectable Due Needs to be debugged for I2C issue!!
https://content.arduino.cc/assets/Pinout-Mega2560rev3_latest.pdf https://content.arduino.cc/assets/Pinout-Due_latest.pdf
RTC and SD CArd Module
https://www.adafruit.com/product/1141
Display 128x64 SSD1306 OLED Display
https://esphome.io/components/display/ssd1306.html
Sensors That Has been Used:
https://cdn-learn.adafruit.com/downloads/pdf/adafruit-tsl2591.pdf
https://cdn-learn.adafruit.com/downloads/pdf/adafruit-bmp388.pdf?timestamp=1556108471
https://cdn-learn.adafruit.com/downloads/pdf/adafruit-si7021-temperature-plus-humidity-sensor.pdf
https://learn.adafruit.com/adafruit-lsm9ds1-accelerometer-plus-gyro-plus-magnetometer-9-dof-breakout
https://moderndevice.com/product/wind-sensor/
Energy Metering IC from Analog Devices
https://www.analog.com/media/en/technical-documentation/data-sheets/ade9153a.pdf https://www.analog.com/en/products/ade9153a.html#product-overview
Debugger & Protocol Analyzer
https://reference.digilentinc.com/reference/instrumentation/analog-discovery-2/start

// C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries
// Location of Main Librarires

// Set Origin
//   git remote add origin git@github.com:ilkerya/Baracuda.git
//  https://github.com/ilkerya/Baracuda
//  https://blog.plover.com/prog/git-ff-error.html
// C:\Users\Yagciilk\Documents\Arduino\libraries
// C:\Program Files (x86)\Arduino\libraries   
// C:\Projects\Pangolin\Baracuda\ArduinoCore\include  default AtmelStudio Project lib locations

#include  "C:\Users\ilker\OneDrive\Belgeler\Arduino\Baracuda\Defs.h"
#include  "C:\Users\ilker\OneDrive\Belgeler\Arduino\Baracuda\Variables.h"
#include  "C:\Users\ilker\OneDrive\Belgeler\Arduino\Baracuda\RTC.h"
#include  "C:\Users\ilker\OneDrive\Belgeler\Arduino\Baracuda\SDCard.h"
#include  "C:\Users\ilker\OneDrive\Belgeler\Arduino\Baracuda\UserInt.h"
#include  "C:\Users\ilker\OneDrive\Belgeler\Arduino\Baracuda\Display.h"
#include  "C:\Users\ilker\OneDrive\Belgeler\Arduino\Baracuda\Sensors.h"
#include  "C:\Users\ilker\OneDrive\Belgeler\Arduino\Baracuda\ADE9153.h"
#include  "C:\Users\ilker\OneDrive\Belgeler\Arduino\Baracuda\Menu.h"
#include  "C:\Users\ilker\OneDrive\Belgeler\Arduino\Baracuda\Functions.h"
