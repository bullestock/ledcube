#include <stm32f10x.h>

#define znew (z=36969*(z&65535)+(z>>16))
#define wnew (w=18000*(w&65535)+(w>>16))
#define MWC ((znew<<16)+wnew )
#define UC (unsigned char) /*a cast operation*/
typedef unsigned long UL;

/* Global static variables: */
static UL z=362436069, w=521288629, jsr=123456789, jcong=380116160;
static UL a=224466889, b=7584631, t[256];

/* Use random seeds to reset z,w,jsr,jcong,a,b, and the table t[256]*/
static UL x=0,y=0,bro; static unsigned char c=0;

static void settable(UL i1,UL i2,UL i3,UL i4,UL i5, UL i6)
{
    int i;
    z=i1;w=i2,jsr=i3; jcong=i4; a=i5; b=i6;
    for(i=0;i<256;i=i+1)
        t[i]=MWC;
}

void Srand()
{
    settable(12345, 65435, 34221, 12345, 9983651, 95746118);
}

unsigned long Rand()
{
    return MWC;
}
