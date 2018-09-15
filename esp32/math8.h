#ifndef __INC_LIB8TION_MATH_H
#define __INC_LIB8TION_MATH_H

#include "scale8.h"

#define QADD8_C 1
#define QADD7_C 1

///@ingroup lib8tion

///@defgroup Math Basic math operations
/// Fast, efficient 8-bit math functions specifically
/// designed for high-performance LED programming.
///
/// Because of the AVR(Arduino) and ARM assembly language
/// implementations provided, using these functions often
/// results in smaller and faster code than the equivalent
/// program using plain "C" arithmetic and logic.
///@{


/// add one byte to another, saturating at 0xFF
/// @param i - first byte to add
/// @param j - second byte to add
/// @returns the sum of i & j, capped at 0xFF
LIB8STATIC_ALWAYS_INLINE uint8_t qadd8( uint8_t i, uint8_t j)
{
#if QADD8_C == 1
    unsigned int t = i + j;
    if( t > 255) t = 255;
    return t;
#elif QADD8_AVRASM == 1
    asm volatile(
         /* First, add j to i, conditioning the C flag */
         "add %0, %1    \n\t"

         /* Now test the C flag.
           If C is clear, we branch around a load of 0xFF into i.
           If C is set, we go ahead and load 0xFF into i.
         */
         "brcc L_%=     \n\t"
         "ldi %0, 0xFF  \n\t"
         "L_%=: "
         : "+a" (i)
         : "a"  (j) );
    return i;
#elif QADD8_ARM_DSP_ASM == 1
    asm volatile( "uqadd8 %0, %0, %1" : "+r" (i) : "r" (j));
    return i;
#else
#error "No implementation for qadd8 available."
#endif
}

/// Add one byte to another, saturating at 0x7F
/// @param i - first byte to add
/// @param j - second byte to add
/// @returns the sum of i & j, capped at 0xFF
LIB8STATIC_ALWAYS_INLINE int8_t qadd7( int8_t i, int8_t j)
{
#if QADD7_C == 1
    int16_t t = i + j;
    if( t > 127) t = 127;
    return t;
#elif QADD7_AVRASM == 1
    asm volatile(
         /* First, add j to i, conditioning the V flag */
         "add %0, %1    \n\t"

         /* Now test the V flag.
          If V is clear, we branch around a load of 0x7F into i.
          If V is set, we go ahead and load 0x7F into i.
          */
         "brvc L_%=     \n\t"
         "ldi %0, 0x7F  \n\t"
         "L_%=: "
         : "+a" (i)
         : "a"  (j) );

    return i;
#elif QADD7_ARM_DSP_ASM == 1
    asm volatile( "qadd8 %0, %0, %1" : "+r" (i) : "r" (j));
    return i;
#else
#error "No implementation for qadd7 available."
#endif
}

/// subtract one byte from another, saturating at 0x00
/// @returns i - j with a floor of 0
LIB8STATIC_ALWAYS_INLINE uint8_t qsub8( uint8_t i, uint8_t j)
{
    int t = i - j;
    if( t < 0) t = 0;
    return t;
}

/// add one byte to another, with one byte result
LIB8STATIC_ALWAYS_INLINE uint8_t add8( uint8_t i, uint8_t j)
{
    int t = i + j;
    return t;
}

/// add one byte to another, with one byte result
LIB8STATIC_ALWAYS_INLINE uint16_t add8to16( uint8_t i, uint16_t j)
{
    uint16_t t = i + j;
    return t;
}


/// subtract one byte from another, 8-bit result
LIB8STATIC_ALWAYS_INLINE uint8_t sub8( uint8_t i, uint8_t j)
{
    int t = i - j;
    return t;
}

/// Calculate an integer average of two unsigned
///       8-bit integer values (uint8_t).
///       Fractional results are rounded down, e.g. avg8(20,41) = 30
LIB8STATIC_ALWAYS_INLINE uint8_t avg8( uint8_t i, uint8_t j)
{
    return (i + j) >> 1;
}

/// Calculate an integer average of two unsigned
///       16-bit integer values (uint16_t).
///       Fractional results are rounded down, e.g. avg16(20,41) = 30
LIB8STATIC_ALWAYS_INLINE uint16_t avg16( uint16_t i, uint16_t j)
{
    return (uint32_t)((uint32_t)(i) + (uint32_t)(j)) >> 1;
}


/// Calculate an integer average of two signed 7-bit
///       integers (int8_t)
///       If the first argument is even, result is rounded down.
///       If the first argument is odd, result is result up.
LIB8STATIC_ALWAYS_INLINE int8_t avg7( int8_t i, int8_t j)
{
    return ((i + j) >> 1) + (i & 0x1);
}

/// Calculate an integer average of two signed 15-bit
///       integers (int16_t)
///       If the first argument is even, result is rounded down.
///       If the first argument is odd, result is result up.
LIB8STATIC_ALWAYS_INLINE int16_t avg15( int16_t i, int16_t j)
{
    return ((int32_t)((int32_t)(i) + (int32_t)(j)) >> 1) + (i & 0x1);
}


///       Calculate the remainder of one unsigned 8-bit
///       value divided by anoter, aka A % M.
///       Implemented by repeated subtraction, which is
///       very compact, and very fast if A is 'probably'
///       less than M.  If A is a large multiple of M,
///       the loop has to execute multiple times.  However,
///       even in that case, the loop is only two
///       instructions long on AVR, i.e., quick.
LIB8STATIC_ALWAYS_INLINE uint8_t mod8( uint8_t a, uint8_t m)
{
    while( a >= m) a -= m;
    return a;
}

///          Add two numbers, and calculate the modulo
///          of the sum and a third number, M.
///          In other words, it returns (A+B) % M.
///          It is designed as a compact mechanism for
///          incrementing a 'mode' switch and wrapping
///          around back to 'mode 0' when the switch
///          goes past the end of the available range.
///          e.g. if you have seven modes, this switches
///          to the next one and wraps around if needed:
///            mode = addmod8( mode, 1, 7);
///LIB8STATIC_ALWAYS_INLINESee 'mod8' for notes on performance.
LIB8STATIC uint8_t addmod8( uint8_t a, uint8_t b, uint8_t m)
{
    a += b;
    while( a >= m) a -= m;
    return a;
}

/// 8x8 bit multiplication, with 8 bit result
LIB8STATIC_ALWAYS_INLINE uint8_t mul8( uint8_t i, uint8_t j)
{
    return ((int)i * (int)(j) ) & 0xFF;
}


/// saturating 8x8 bit multiplication, with 8 bit result
/// @returns the product of i * j, capping at 0xFF
LIB8STATIC_ALWAYS_INLINE uint8_t qmul8( uint8_t i, uint8_t j)
{
    int p = ((int)i * (int)(j) );
    if( p > 255) p = 255;
    return p;
}


/// take abs() of a signed 8-bit uint8_t
LIB8STATIC_ALWAYS_INLINE int8_t abs8( int8_t i)
{
    if( i < 0) i = -i;
    return i;
}

///         square root for 16-bit integers
///         About three times faster and five times smaller
///         than Arduino's general sqrt on AVR.
LIB8STATIC uint8_t sqrt16(uint16_t x)
{
    if( x <= 1) {
        return x;
    }

    uint8_t low = 1; // lower bound
    uint8_t hi, mid;

    if( x > 7904) {
        hi = 255;
    } else {
        hi = (x >> 5) + 8; // initial estimate for upper bound
    }

    do {
        mid = (low + hi) >> 1;
        if ((uint16_t)(mid * mid) > x) {
            hi = mid - 1;
        } else {
            if( mid == 255) {
                return 255;
            }
            low = mid + 1;
        }
    } while (hi >= low);

    return low - 1;
}

/// blend a variable proproportion(0-255) of one byte to another
/// @param a - the starting byte value
/// @param b - the byte value to blend toward
/// @param amountOfB - the proportion (0-255) of b to blend
/// @returns a byte value between a and b, inclusive
#if (FASTLED_BLEND_FIXED == 1)
LIB8STATIC uint8_t blend8( uint8_t a, uint8_t b, uint8_t amountOfB)
{
    uint16_t partial;
    uint8_t result;
    
    uint8_t amountOfA = 255 - amountOfB;
    
    partial = (a * amountOfA);
#if (FASTLED_SCALE8_FIXED == 1)
    partial += a;
    //partial = add8to16( a, partial);
#endif
    
    partial += (b * amountOfB);
#if (FASTLED_SCALE8_FIXED == 1)
    partial += b;
    //partial = add8to16( b, partial);
#endif
    
    result = partial >> 8;
    
    return result;
}

#else
LIB8STATIC uint8_t blend8( uint8_t a, uint8_t b, uint8_t amountOfB)
{
    // This version loses precision in the integer math
    // and can actually return results outside of the range
    // from a to b.  Its use is not recommended.
    uint8_t result;
    uint8_t amountOfA = 255 - amountOfB;
    result = scale8_LEAVING_R1_DIRTY( a, amountOfA)
           + scale8_LEAVING_R1_DIRTY( b, amountOfB);
    cleanup_R1();
    return result;
}
#endif


///@}
#endif
