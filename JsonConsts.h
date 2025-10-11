#ifndef JSONCONSTS_H
#define JSONCONSTS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

enum {
    JSON_STRING,
    JSON_NUMBER,
    JSON_BOOLEAN,
    JSON_NULL,
    JSON_OBJECT,
    JSON_ARRAY
};

typedef struct {
    uint8_t type;
} JsonData;

typedef struct {
    JsonData base;
    size_t size;
    size_t capacity;
    char *value;
} JsonString;

typedef struct {
    JsonData base;
    double value;
} JsonNumber;

typedef struct {
    JsonData base;
    bool value;
} JsonBoolean;

// Null type has no additional fields.

typedef struct {
    JsonString *key;
    JsonData *value;
} JsonPair;

typedef struct {
    JsonData base;
    size_t size;
    size_t capacity;
    JsonPair **items;
} JsonObject;

typedef struct {
    JsonData base;
    size_t size;
    size_t capacity;
    JsonData **items;
} JsonArray;

#endif