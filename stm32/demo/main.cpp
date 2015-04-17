#include <stm32f10x.h>

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

uint32_t endTime = 0;

void show(unsigned char* buf, uint16_t n, GPIO_TypeDef* gpio, int pin)
{
  // Data latch = 24 or 50 microsecond pause in the output stream.  Rather than
  // put a delay at the end of the function, the ending time is noted and
  // the function will simply hold off (if needed) on issuing the
  // subsequent round of data until the latch time has elapsed.  This
  // allows the mainline code to start generating the next frame of data
  // rather than stalling for the latch.
  uint32_t waitime = 50; // wait time in microseconds.

  //!!  while ((micros() - endTime) < waitime);

  __disable_irq(); // Need 100% focus on instruction timing

  int numBytes = n*3;

  volatile uint32_t 
    c,    // 24-bit pixel color
    mask; // 8-bit mask
  volatile uint16_t i = numBytes; // Output loop counter
  volatile uint8_t
    j,              // 8-bit inner loop counter
   *ptr = buf,   // Pointer to next byte
    g,              // Current green byte value
    r,              // Current red byte value
    b;              // Current blue byte value
  
  while(i) { // While bytes left... (3 bytes = 1 pixel)
      mask = 0x800000; // reset the mask
      i = i-3;      // decrement bytes remaining
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
  endTime = micros(); // Save EOD time for latch on next call
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

int main()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
    
	GPIO_InitStruct.GPIO_Pin = 255;		  // we want to configure PA0-PA7
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 	// we want the pins to be a push-pull output
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  //this sets the GPIO modules clock speed
	GPIO_Init(GPIOA, &GPIO_InitStruct);			    // this passes the configuration to the Init function which takes care of the low level stuff

    unsigned char buf[8*8*3];
#if 0
    GPIO_TypeDef* gpio = GPIOA;
    while (1)
    {
        gpio->ODR = 1;
        delay_us(5000);
        gpio->ODR = 0;
        delay_us(5000);
    }
#else
    while (1)
    {
        for (int i = 0; i < 255; ++i)
        {
            for (int j = 0; j < 8*8; ++j)
            {
                setPixelColor(buf, j, Wheel((i+j) & 255));
            }
            for (int p = 0; p < 8; ++p)
            {
                show(buf, 8*8, GPIOA, 1 << p);
            }
            delay_us(5000);
        }
    }
#endif
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

