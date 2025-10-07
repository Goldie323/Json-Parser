#ifndef JSONTOOLS_C
#define JSONTOOLS_C

#include <stddef.h>
#include <stdbool.h>
#include "JsonConsts.h"

size_t RoundUpToPowerOfTwo(size_t x) {
    if (x == 0) return 1;

    x--;
    for (size_t i = 1; i < sizeof(size_t)*8; i *= 2)
        x |= x >> i;
    x++;
    return x;
}

bool JsonStringCmp(const JsonString *a, const JsonString *b) {
    if (a->size != b->size) return false;
    return (memcmp(a->value, b->value, a->size) == 0);
}

#endif