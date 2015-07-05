#include <stm32f10x.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "delay.h"
#include "systick.h"

#define CYCLES_PER_MICROSECOND    72
#define SYSTICK_RELOAD_VAL     71999 /* takes a cycle to reload */

static inline uint32_t millis()
{
    return systick_uptime();
}

static inline uint32_t micros()
{
    uint32_t ms;
    uint32_t cycle_cnt;

    do {
        ms = millis();
        cycle_cnt = systick_get_count();
        asm volatile("nop"); //allow interrupt to fire
        asm volatile("nop");
    } while (ms != millis());

#define US_PER_MS               1000
    /* SYSTICK_RELOAD_VAL is 1 less than the number of cycles it
     * actually takes to complete a SysTick reload */
    return ((ms * US_PER_MS) +
            (SYSTICK_RELOAD_VAL + 1 - cycle_cnt) / CYCLES_PER_MICROSECOND);
#undef US_PER_MS
}

static double abs(double val)
{
    return val > 0 ? val : -val;
}

void setPixelColor(unsigned char* buf, uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t *p = &buf[n * 3];
    *p++ = g;
    *p++ = r;
    *p = b;
}

// Set pixel color from 'packed' 32-bit RGB color:
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

const int SIDE = 8;

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

// Sets a pixel at position (x,y,z) to the col parameter's color
void setPixel(unsigned char* buf, int x, int y, int z, const color& c)
{
    // TODO: handle flipped strips
    int index = (z*SIDE*SIDE) + (x*SIDE) + y;
    setPixelColor(buf, index, c.red, c.green, c.blue);
}

void updateFireworks(unsigned char* buf);
float distance(float x, float y, float z, float x1, float y1, float z1);
void prepRocket();
void initFireworks();

void show(unsigned char* buf, uint16_t n, GPIO_TypeDef* gpio, int pin)
{
  __disable_irq(); // Need 100% focus on instruction timing

  volatile uint32_t 
    c,    // 24-bit pixel color
    mask; // 8-bit mask
  volatile uint8_t
    j,              // 8-bit inner loop counter
   *ptr = buf,   // Pointer to next byte
    g,              // Current green byte value
    r,              // Current red byte value
    b;              // Current blue byte value
  
  while (n--)
  {
      mask = 0x800000; // reset the mask
      g = *ptr++;   // Next green byte value
      r = *ptr++;   // Next red byte value
      b = *ptr++;   // Next blue byte value
      c = ((uint32_t)g << 16) | ((uint32_t)r <<  8) | b; // Pack the next 3 bytes to keep timing tight
      j = 0;        // reset the 24-bit counter
      do {
          gpio->ODR |= pin;	// HIGH
          if (c & mask) { // if masked bit is high
              // WS2812 spec             700ns HIGH
              // Adafruit on Arduino    (meas. 812ns)
              // This lib on Spark Core (meas. 792ns)
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
              // WS2812 spec             600ns LOW
              // Adafruit on Arduino    (meas. 436ns)
              // This lib on Spark Core (meas. 472ns)
              gpio->ODR &= ~pin;	// LOW
              asm volatile(
                           "mov r0, r0" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
                           "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
                           ::: "r0", "cc", "memory");
          }
          else { // else masked bit is low
              // WS2812 spec             350ns HIGH
              // Adafruit on Arduino    (meas. 312ns)
              // This lib on Spark Core (meas. 306ns)
              asm volatile(
                           "mov r0, r0" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
                           ::: "r0", "cc", "memory");
              // WS2812 spec             800ns LOW
              // Adafruit on Arduino    (meas. 938ns)
              // This lib on Spark Core (meas. 932ns)
              gpio->ODR &= ~pin;	// LOW
              asm volatile(
                           "mov r0, r0" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t" "nop" "\n\t"
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
      } while ( ++j < 24 ); // ... pixel done
  } // end while(i) ... no more pixels

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
    for (int p = 0; p < 8; ++p)
    {
        show(buf+SIDE*SIDE*p*3, SIDE*SIDE, GPIOA, 1 << p);
    }
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
        for (int idx = 0; idx < SIDE*SIDE*SIDE*3; ++idx)
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

    unsigned char buf[8*8*8*3];
    memset(buf, 0, sizeof(buf));
    ShowBuffer(buf);
    delay_ms(1);

#if 0
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
            for (int j = 0; j < SIDE*SIDE*SIDE; ++j)
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
            for (int j = 0; j < SIDE*SIDE*SIDE; ++j)
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
            for (int j = 0; j < SIDE*SIDE*SIDE; ++j)
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

#if 1
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

#if 0
    // Chase single pixel
    int index = 0;
    for (int n = 0; n < numShows; ++n)
    {
        memset(buf, 0, sizeof(buf));
        setPixelColor(buf, index, 255, 255, 255);
        ShowBuffer(buf);
        ++index;
        if (index > SIDE*SIDE*SIDE)
            index = 0;
        if (iterDelay)
            delay_us(iterDelay);
        delay_us(100);
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
    for(int x=0;x<SIDE;x++)
        for(int y=0;y<SIDE;y++) 
            for(int z=0;z<SIDE;z++)
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
    for(int x=0;x<SIDE;x++)
        for(int y=0;y<SIDE;y++) 
            for(int z=0;z<SIDE;z++)
            {
                //setPixel(buf, x,y,z, rocketColor);
                setPixelColor(buf, x*8+y*8*8+z, 16, 32, 64);
            }
}

float distance(float x, float y, float z, float x1, float y1, float z1)
{
    return 0.0;//!!(sqrt(pow(x-x1,2)+pow(y-y1,2)+pow(z-z1,2)));
}

void prepRocket()
{
    radius=0;
    centerX=rand()%8;
    centerY=rand()%8;
    centerZ=rand()%8;
    fireworkColor.red=rand()%brightness;
    fireworkColor.green=rand()%brightness;
    fireworkColor.blue=rand()%brightness;
    launchX=rand()%8;
    launchZ=rand()%8;
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
    //speed=rand()%5;
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

