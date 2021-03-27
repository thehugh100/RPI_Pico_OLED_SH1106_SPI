#pragma once

#include <stdio.h>
#include <cstdio>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"

#define SSH1106_LCDWIDTH 128
#define SSH1106_LCDHEIGHT 64

#define SSH1106_SETCONTRAST 0x81
#define SSH1106_DISPLAYALLON_RESUME 0xA4
#define SSH1106_DISPLAYALLON 0xA5
#define SSH1106_NORMALDISPLAY 0xA6
#define SSH1106_INVERTDISPLAY 0xA7
#define SSH1106_DISPLAYOFF 0xAE
#define SSH1106_DISPLAYON 0xAF
#define SSH1106_SETDISPLAYOFFSET 0xD3
#define SSH1106_SETCOMPINS 0xDA
#define SSH1106_SETVCOMDETECT 0xDB
#define SSH1106_SETDISPLAYCLOCKDIV 0xD5
#define SSH1106_SETPRECHARGE 0xD9
#define SSH1106_SETMULTIPLEX 0xA8
#define SSH1106_SETLOWCOLUMN 0x00
#define SSH1106_SETHIGHCOLUMN 0x10
#define SSH1106_SETSTARTLINE 0x40
#define SSH1106_MEMORYMODE 0x20
#define SSH1106_COLUMNADDR 0x21
#define SSH1106_PAGEADDR   0x22
#define SSH1106_COMSCANINC 0xC0
#define SSH1106_COMSCANDEC 0xC8
#define SSH1106_SEGREMAP 0xA0
#define SSH1106_CHARGEPUMP 0x8D
#define SSH1106_EXTERNALVCC 0x1
#define SSH1106_SWITCHCAPVCC 0x2

class SSH1106
{
public:
    SSH1106(
        spi_inst_t *spiChan = spi0,
        uint8_t sclk_pin_ = 2, 
        uint8_t mosi_pin_ = 3, 
        uint8_t cs_pin_ = 5, 
        uint8_t rst_pin_ = 6, 
        uint8_t dc_pin_ = 7
    );

    ~SSH1106();

    void init();
    void command(uint8_t c);
    void drawPixel(uint8_t x, uint8_t y, uint8_t color);
    void display();
    
private:
    uint8_t sclk_pin_;
    uint8_t mosi_pin_;
    uint8_t cs_pin_;
    uint8_t rst_pin_;
    uint8_t dc_pin_;
    spi_inst_t *spiChan;

    uint16_t bufSize;

    uint8_t *buffer;
};

