# RPI_Pico_OLED_SSH1106_SPI
A quick and dirty driver for the SSH1106 SPI OLED modules for the Raspberry Pi Pico

Any fancy advanced graphics is beyond the scope of this repo, this is just the bare minimum i.e. a screenbuffer and a way to plot pixels.

# Basic Usage:

```C++
#include "pico/stdlib.h"
#include "SSH1106_SPI_Lite.h"
...
//define and init the OLED object
SSH1106 oled;
oled.init();
...
// draw some nice pixels
oled.drawPixel(10, 10, 1);
oled.drawPixel(11, 11, 1);
oled.drawPixel(12, 12, 1);
...
// Send our screenbuffer to the OLED
oled.display();
```
