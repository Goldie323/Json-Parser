#ifndef JSONTOOLS_H
#define JSONTOOLS_H

#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "JsonConsts.h"

size_t RoundUpToPowerOfTwo(size_t x);
bool JsonStringCmp(const void *a, const void *b);
char *JsonMinimizeString(const char *str);
bool isString(void *data);
bool isNumber(void *data);
bool isBoolean(void *data);
bool isNull(void *data);
bool isObject(void *data);
bool isArray(void *data);

#endif