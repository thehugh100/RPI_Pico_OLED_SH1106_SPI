#include "SSH1106_SPI_Lite.h"

SSH1106::SSH1106(
    spi_inst_t *spiChan,
    uint8_t sclk_pin_, 
    uint8_t mosi_pin_, 
    uint8_t cs_pin_, 
    uint8_t rst_pin_, 
    uint8_t dc_pin_
    )
    :spiChan(spiChan), sclk_pin_(sclk_pin_), mosi_pin_(mosi_pin_), cs_pin_(cs_pin_), rst_pin_(rst_pin_), dc_pin_(dc_pin_)
{
    bufSize = SSH1106_LCDHEIGHT * SSH1106_LCDWIDTH / 8;
    buffer = new uint8_t[bufSize];
}

void SSH1106::command(uint8_t c)
{
    gpio_put(dc_pin_, 0);
    spi_write_blocking(spiChan, &c, 1);
    gpio_put(dc_pin_, 1);
}

void SSH1106::init()
{
    //initialise GPIO

    gpio_init_mask(1 << sclk_pin_ | 1 << mosi_pin_ | 1 << cs_pin_ | 1 << dc_pin_ | 1 << rst_pin_);
    gpio_set_dir(rst_pin_, GPIO_OUT);
    gpio_put(rst_pin_, 0);
    spi_init(spiChan, 8000 * 1000); //spi bus speed
    gpio_set_function(mosi_pin_, GPIO_FUNC_SPI);
    gpio_set_function(sclk_pin_, GPIO_FUNC_SPI);
    gpio_set_dir(cs_pin_, GPIO_OUT);
    gpio_put(cs_pin_, 0);
    gpio_set_dir(dc_pin_, GPIO_OUT);
    gpio_put(dc_pin_, 0);

    uint8_t vccstate = 0;

    //reset

    gpio_put(rst_pin_, 0);
    sleep_ms(10);
    gpio_put(rst_pin_, 1);

    //send init commands

    command(SSH1106_DISPLAYOFF);
    command(SSH1106_SETDISPLAYCLOCKDIV);
    command(0x80);
    command(SSH1106_SETMULTIPLEX);
    command(0x3F);
    command(SSH1106_SETDISPLAYOFFSET);
    command(0x0);
    command(SSH1106_SETSTARTLINE | 0x0);
    command(SSH1106_CHARGEPUMP);
    if (vccstate == SSH1106_EXTERNALVCC)
    {
        command(0x10);
    }
    else 
    {
        command(0x14);
    }
    command(SSH1106_MEMORYMODE);
    command(0x00);
    command(SSH1106_SEGREMAP | 0x1);
    command(SSH1106_COMSCANDEC);
    command(SSH1106_SETCOMPINS);
    command(0x12);
    command(SSH1106_SETCONTRAST);
    if (vccstate == SSH1106_EXTERNALVCC)
    {
        command(0x9F);
    }
    else 
    {
        command(0xCF);
    }
    command(SSH1106_SETPRECHARGE);
    if (vccstate == SSH1106_EXTERNALVCC)
    {
        command(0x22);
    }
    else
    {
        command(0xF1);
    
    }
    command(SSH1106_SETVCOMDETECT);
    command(0x40);
    command(SSH1106_DISPLAYALLON_RESUME);
    command(SSH1106_NORMALDISPLAY);

    command(SSH1106_DISPLAYON);

    //init buffer
    clear();
}

void SSH1106::drawPixel(uint8_t x, uint8_t y, uint8_t color)
{
    switch (color) 
    {
      case 0:   buffer[x+ (y/8)*SSH1106_LCDWIDTH] &= ~(1 << (y&7)); break; //black
      case 1:   buffer[x+ (y/8)*SSH1106_LCDWIDTH] |=  (1 << (y&7)); break; //white
      case 2: buffer[x+ (y/8)*SSH1106_LCDWIDTH] ^=  (1 << (y&7)); break; //invert
    }
}

void SSH1106::display()
{
    command(SSH1106_SETLOWCOLUMN | 0x0); 
    command(SSH1106_SETHIGHCOLUMN | 0x0);
    command(SSH1106_SETSTARTLINE | 0x0);
	
	uint8_t height=64;
	uint8_t width=132; 
	uint8_t m_row = 0;
	uint8_t m_col = 2;
	
	height >>= 3;
	width >>= 3;

	int p = 0;
	
	uint8_t i, j, k =0;
	
    for (i = 0; i < height; i++)
    {
        command(0xB0 + i + m_row);
        command(m_col & 0xf);
        command(0x10 | (m_col >> 4));
        
        for( j = 0; j < 8; j++)
        {        
            for ( k = 0; k < width; k++, p++) {
                spi_write_blocking(spiChan, &buffer[p], 1);
            }
        }
    }
}

void SSH1106::clear()
{
    for(int i = 0; i < bufSize; ++i)
        buffer[i] = 0;
}

SSH1106::~SSH1106()
{
    delete[] buffer;
}