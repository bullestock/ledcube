#include <stm32f10x.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "util.h"

#include "delay.h"

const int PIX_PER_SIDE = 8;
const int TOTAL_PIX = PIX_PER_SIDE*PIX_PER_SIDE*PIX_PER_SIDE;

static double abs(double val)
{
    return val > 0 ? val : -val;
}

// Set pixel color from RGB components
void setPixelColor(unsigned char* buf, uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t *p = &buf[n * 3];
    *p++ = g;
    *p++ = r;
    *p = b;
}

// Set pixel color from 'packed' 32-bit RGB color
void setPixelColor(unsigned char* buf, uint16_t n, uint32_t c)
{
    uint8_t
       r = (uint8_t)(c >> 16),
       g = (uint8_t)(c >>  8),
       b = (uint8_t)c;
    uint8_t *p = &buf[n * 3];
    *p++ = g;
    *p++ = r;
    *p = b;
}

struct color
{
    color(unsigned char r = 0,
          unsigned char g = 0,
          unsigned char b = 0)
        : red(r),
          green(g),
          blue(b)
    {
    }
    
    unsigned char red, green, blue;
};

// Set (x, y, z) pixel color from colour struct
void setPixel(unsigned char* buf, int x, int y, int z, const color& c)
{
    // TODO: handle flipped strips
    int index = (z*PIX_PER_SIDE*PIX_PER_SIDE) + (x*PIX_PER_SIDE) + y;
    setPixelColor(buf, index, c.red, c.green, c.blue);
}

void updateFireworks(unsigned char* buf);
float distance(float x, float y, float z, float x1, float y1, float z1);
void prepRocket();
void initFireworks();

// Simple version
void show(unsigned char* buf, uint16_t n, GPIO_TypeDef* gpio, int pin)
{
    __disable_irq(); // Need 100% focus on instruction timing

    volatile uint8_t* ptr = buf;   // Pointer to next byte
  
    while (n--)
    {
        uint32_t mask = 0x800000; // reset the mask
        const uint32_t g = *ptr++;   // Next green byte value
        const uint32_t r = *ptr++;   // Next red byte value
        const uint32_t b = *ptr++;   // Next blue byte value
        const uint32_t c = (g << 16) | (r <<  8) | b; // Pack the next 3 bytes to keep timing tight
        int j = 0;        // reset the 24-bit counter
        do {
            gpio->ODR |= pin;	// HIGH
            if (c & mask)
            {
                // Masked bit is high
                // WS2812 spec             700ns HIGH  !! 0.8 us
                asm volatile(
                             "mov r0, r0" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
                             "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
                             "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
                             "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
                             "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
                             "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
                             "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
                             "nop" "\n\t" 
                             ::: "r0", "cc", "memory");
                // WS2812 spec             600ns LOW  !! 0.45 us
                gpio->ODR &= ~pin;	// LOW
                asm volatile("mov r0, r0" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
                             "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
                             ::: "r0", "cc", "memory");
            }
            else
            {
                // Masked bit is low
                // WS2812 spec             350ns HIGH  !! 0.4 us
                asm volatile(
                             "mov r0, r0" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
                             ::: "r0", "cc", "memory");
                // WS2812 spec             800ns LOW   !! 0.85 us
                gpio->ODR &= ~pin;	// LOW
                asm volatile("mov r0, r0" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
                             "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
                             "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
                             "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
                             "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
                             "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
                             "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
                             "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
                             ::: "r0", "cc", "memory");
            }
            mask >>= 1;
        } while (++j < 24); // ... pixel done
    }

    __enable_irq();
}

// Convert separate R,G,B into packed 32-bit RGB color.
// Packed format is always RGB, regardless of LED strand color order.
uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}

uint32_t Wheel(uint8_t WheelPos)
{
    if (WheelPos < 85)
    {
        return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
    else if(WheelPos < 170)
    {
        WheelPos -= 85;
        return Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    else
    {
        WheelPos -= 170;
        return Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
}

color wheel(uint8_t pos)
{
    if (pos < 85)
    {
        return color(pos * 3, 255 - pos * 3, 0);
    }
    else if(pos < 170)
    {
        pos -= 85;
        return color(255 - pos * 3, 0, pos * 3);
    }
    else
    {
        pos -= 170;
        return color(0, pos * 3, 255 - pos * 3);
    }
}

void ShowBuffer(unsigned char* buf)
{
#if 1
    for (int p = 0; p < 8; ++p)
    {
        show(buf+PIX_PER_SIDE*PIX_PER_SIDE*p*3, PIX_PER_SIDE*PIX_PER_SIDE, GPIOA, 1 << p);
    }
#else
    show(buf, PIX_PER_SIDE, PIX_PER_SIDE*PIX_PER_SIDE, GPIOA);
#endif
    delay_us(50);
}

void delay_ms(int ms)
{
    for (int i = 0; i < ms; ++i)
        delay_us(1000);
}

void Fade(unsigned char* buf)
{
    for (int i = 0; i < 50; ++i)
    {
        for (int idx = 0; idx < PIX_PER_SIDE*PIX_PER_SIDE*PIX_PER_SIDE*3; ++idx)
        {
            char v = buf[idx];
            if (v > 32)
                v *= 0.8;
            else
                v /= 2;
            buf[idx] = v;
        }
        ShowBuffer(buf);
        delay_ms(100);
    }
}

int main()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
    
	GPIO_InitStruct.GPIO_Pin = 255;		  // we want to configure PA0-PA7
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 	// we want the pins to be a push-pull output
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  //this sets the GPIO modules clock speed
	GPIO_Init(GPIOA, &GPIO_InitStruct);			    // this passes the configuration to the Init function which takes care of the low level stuff

    // Blank all LEDs
    unsigned char buf[TOTAL_PIX*3];
    memset(buf, 0, sizeof(buf));
    ShowBuffer(buf);

    Srand();

#if 0
    // Timing test
    GPIO_TypeDef* gpio = GPIOA;
    while (1)
    {
        gpio->ODR = 1;
        delay_us(5000);
        gpio->ODR = 0;
        delay_us(5000);
    }
#endif

#if 0
    // Test
    while (1)
    {
        for (int c = 8; c < 32; ++c)
        {
            for (int j = 0; j < PIX_PER_SIDE*PIX_PER_SIDE*PIX_PER_SIDE; ++j)
            {
                color C = wheel(c);
                buf[j*3] = C.red;
                buf[j*3+1] = C.green;
                buf[j*3+2] = C.blue;
            }
            ShowBuffer(buf);
            delay_us(5000);
        }
    }
#endif

    const int numShows = 3;
    const int iterDelay = 1;
    
#if 0
    // Rainbow
    for (int n = 0; n < numShows; ++n)
    {
        for (int i = 0; i < 255; ++i)
        {
            for (int j = 0; j < PIX_PER_SIDE*PIX_PER_SIDE*PIX_PER_SIDE; ++j)
            {
                setPixelColor(buf, j, Wheel((i+j) & 255));
            }
            ShowBuffer(buf);
            if (iterDelay)
                delay_us(iterDelay);
        }
    }
#endif

    Fade(buf);

#if 0
    // Rainbow wipe
    for (int n = 0; n < numShows; ++n)
    {
        for (int c = 0; c < 255; ++c)
        {
            for (int j = 0; j < PIX_PER_SIDE*PIX_PER_SIDE*PIX_PER_SIDE; ++j)
            {
                setPixelColor(buf, j, Wheel(c));
            }
            ShowBuffer(buf);
            if (iterDelay)
                delay_us(iterDelay);
        }
    }
#endif

    Fade(buf);

#if 0
    // Epilectic fit
    for (int n = 0; n < numShows*10; ++n)
    {
        memset(buf, 16, sizeof(buf));
        ShowBuffer(buf);
        delay_ms(1);
        memset(buf, 0, sizeof(buf));
        ShowBuffer(buf);
        delay_ms(10);
    }
#endif

    Fade(buf);

#if 1
    // Chase single pixel
    int index = 0;
    for (int n = 0; n < numShows; ++n)
    {
        for (int index = 0; index < PIX_PER_SIDE*PIX_PER_SIDE*PIX_PER_SIDE; ++index)
        {
            memset(buf, 0, sizeof(buf));
            setPixelColor(buf, index, 50, 0, 0);
            ShowBuffer(buf);
            if (iterDelay)
                delay_us(iterDelay);
            delay_ms(100);
        }
    }

    Fade(buf);
#endif

    initFireworks();
    while (1)
    {
        setPixelColor(buf, 0, 16, 32, 64);
        setPixelColor(buf, 42, 16, 32, 64);
        setPixelColor(buf, 117, 16, 32, 64);
        setPixelColor(buf, 245, 16, 32, 64);
        setPixelColor(buf, 333, 16, 32, 64);
        //updateFireworks(buf);
        ShowBuffer(buf);
        //if (iterDelay)
            delay_ms(100);
    }
}

/******************************
 * fireworks variables *
 * ****************************/
color black;
int centerX, centerY, centerZ;
int launchX, launchZ;
int red, green, blue;
int brightness=35;
float radius=0;
float speed;
bool showRocket;
bool exploded;
float xInc, yInc, zInc;
float rocketX, rocketY, rocketZ;
float launchTime;
int maxSize;
color rocketColor, fireworkColor;

void updateFireworks(unsigned char* buf)
{
#if 0
    //loop through all the pixels, calculate the distance to the center point, and turn the pixel on if it's at the right radius
    for(int x=0;x<PIX_PER_SIDE;x++)
        for(int y=0;y<PIX_PER_SIDE;y++) 
            for(int z=0;z<PIX_PER_SIDE;z++)
            {
                if(showRocket)
                    if(abs(distance(x,y,z,rocketX, rocketY, rocketZ)-radius)<0.05)
                        setPixel(buf, x,y,z, rocketColor);                
                if(exploded)
                    if(abs(distance(x,y,z,centerX, centerY, centerZ)-radius)<0.1)
                        setPixel(buf, x,y,z, fireworkColor);
            }

    if(exploded)
        radius+=speed;  //the sphere gets bigger
    if(showRocket)
    {
        rocketX+=xInc;
        rocketY+=yInc;
        rocketZ+=zInc;
    }
    //if our sphere gets too large, restart the animation in another random spot
    if(radius>maxSize)
        prepRocket();
    if(abs(distance(centerX,centerY,centerZ,rocketX, rocketY, rocketZ)-radius)<2)
    {
        showRocket=false;
        exploded=true;
    }
#endif
    for(int x=0;x<PIX_PER_SIDE;x++)
        for(int y=0;y<PIX_PER_SIDE;y++) 
            for(int z=0;z<PIX_PER_SIDE;z++)
            {
                //setPixel(buf, x,y,z, rocketColor);
                setPixelColor(buf, x*PIX_PER_SIDE+y*PIX_PER_SIDE*PIX_PER_SIDE+z, 16, 32, 64);
            }
}

float distance(float x, float y, float z, float x1, float y1, float z1)
{
    return 0.0;//!!(sqrt(pow(x-x1,2)+pow(y-y1,2)+pow(z-z1,2)));
}

void prepRocket()
{
    radius=0;
    centerX = Rand() % PIX_PER_SIDE;
    centerY = Rand() % PIX_PER_SIDE;
    centerZ = Rand() % PIX_PER_SIDE;
    fireworkColor.red = Rand()%brightness;
    fireworkColor.green = Rand()%brightness;
    fireworkColor.blue = Rand()%brightness;
    launchX = Rand() % PIX_PER_SIDE;
    launchZ = Rand() % PIX_PER_SIDE;
    rocketX=launchX;
    rocketY=0;
    rocketZ=launchZ;
    launchTime=15+rand()%25;
    xInc=(centerX-rocketX)/launchTime;
    yInc=(centerY-rocketY)/launchTime;
    zInc=(centerZ-rocketZ)/launchTime;
    showRocket=true;
    exploded=false;
    speed=0.15;
    maxSize=2+rand()%6;
    //speed = Rand()%5;
    //speed*=0.1;
}

void initFireworks()
{
    rocketColor.red=255;
    rocketColor.green=150;
    rocketColor.blue=100;
    prepRocket();
}

extern "C" void _exit()
{
}

extern "C" void _kill()
{
}

extern "C" void _getpid()
{
}

extern "C" void _sbrk()
{
}

