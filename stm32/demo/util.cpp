#include <stm32f10x.h>

static uint32_t next = 1;

int rand()
{
    next = next * 1103515245 + 12345;
    return (uint32_t) (next >> 16) & RAND_MAX;
}

void srand(unsigned int seed)
{
    next = static_cast<uint32_t>(seed);
}
