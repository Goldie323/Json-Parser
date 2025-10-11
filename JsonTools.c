#ifndef JSONTOOLS_C
#define JSONTOOLS_C

#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "JsonConsts.h"

size_t RoundUpToPowerOfTwo(size_t x) {
    if (x == 0) return 1;

    x--;
    for (size_t i = 1; i < sizeof(size_t)*8; i *= 2)
        x |= x >> i;
    x++;
    return x;
}

bool JsonStringCmp(const void *a, const void *b) {
    const JsonString *strA = (const JsonString *)a;
    const JsonString *strB = (const JsonString *)b;
    if (strA->size != strB->size) return false;
    return (memcmp(strA->value, strB->value, strA->size) == 0);
}

char *JsonMinimizeString(const char *str) {
    size_t len = strlen(str);
    char *minimized = (char *)malloc(len + 1);
    if (!minimized) return NULL;

    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (str[i] != ' ' && str[i] != '\n' && str[i] != '\t' && str[i] != '\r') {
            minimized[j++] = str[i];
        }
    }
    minimized[j] = '\0';
    return minimized;
}

bool isString(void *data) {
    if (!data) return false;
    JsonData *dataPtr = (JsonData *)data;
    return (dataPtr->type == JSON_STRING);
}

bool isNumber(void *data) {
    if (!data) return false;
    JsonData *dataPtr = (JsonData *)data;
    return (dataPtr->type == JSON_NUMBER);
}

bool isBoolean(void *data) {
    if (!data) return false;
    JsonData *dataPtr = (JsonData *)data;
    return (dataPtr->type == JSON_BOOLEAN);
}

bool isNull(void *data) {
    if (!data) return false;
    JsonData *dataPtr = (JsonData *)data;
    return (dataPtr->type == JSON_NULL);
}

bool isObject(void *data) {
    if (!data) return false;
    JsonData *dataPtr = (JsonData *)data;
    return (dataPtr->type == JSON_OBJECT);
}

bool isArray(void *data) {
    if (!data) return false;
    JsonData *dataPtr = (JsonData *)data;
    return (dataPtr->type == JSON_ARRAY);
}

#endif