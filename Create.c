#ifndef CREATE_C
#define CREATE_C

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "Consts.h"
#include "Tools.c"

JsonData *CreateJsonString(const char *value, size_t length, bool isKey) {
    JsonString *JsonString = malloc(sizeof(JsonString));
    JsonString->base.type = JSON_STRING;
    JsonString->size = length;
    if (isKey) {
        JsonString->capacity = length;
    } else {
        JsonString->capacity = RoundUpToPowerOfTwo(length);
    }
    JsonString->value = malloc(JsonString->capacity);
    memcpy(JsonString->value, value, JsonString->size);
    return (JsonData *)JsonString;
}

JsonData *CreateJsonStringNullTerminated(const char *value, bool isKey) {
    return CreateJsonString(value, strlen(value), isKey);
}

JsonData *CreateJsonNumber(double value) {
    JsonNumber *JsonNumber = malloc(sizeof(JsonNumber));
    JsonNumber->base.type = JSON_NUMBER;
    JsonNumber->value = value;
    return (JsonData *)JsonNumber;
}

JsonData *CreateJsonBoolean(bool value) {
    JsonBoolean *JsonBoolean = malloc(sizeof(JsonBoolean));
    JsonBoolean->base.type = JSON_BOOLEAN;
    JsonBoolean->value = value;
    return (JsonData *)JsonBoolean;
}

JsonData *CreateJsonNull() {
    JsonData *JsonNull = malloc(sizeof(JsonData));
    JsonNull->type = JSON_NULL;
    return JsonNull;
}

JsonData *CreateJsonObject() {
    JsonObject *JsonObject = malloc(sizeof(JsonObject));
    JsonObject->base.type = JSON_OBJECT;
    JsonObject->size = 0;
    JsonObject->capacity = 4;
    JsonObject->items = malloc(JsonObject->capacity * sizeof(JsonPair *));
    return (JsonData *)JsonObject;
}

bool DoesKeyExistInJsonObject(JsonObject *obj, JsonString *key) {
    for (size_t i = 0; i < obj->size; i++) {
        if (JsonStringCmp(obj->items[i]->key, key)) {
            return true;
        }
    }
    return false;
}

bool AddToJsonObject(JsonObject *obj, JsonString *key, JsonData *value) {
    if (DoesKeyExistInJsonObject(obj, key)) {
        return false;
    }

    if (obj->size >= obj->capacity) {
        obj->capacity *= 2;
        JsonPair **newItems = realloc(obj->items, obj->capacity * sizeof(JsonPair *));
        if (!newItems) return false;
        obj->items = newItems;
    }

    JsonPair *pair = malloc(sizeof(JsonPair));
    pair->key = key;
    pair->value = value;
    obj->items[obj->size++] = pair;
    return true;
}

bool GetValueFromJsonObject(JsonObject *obj, JsonString *key, JsonData **outValue) {
    for (size_t i = 0; i < obj->size; i++) {
        if (JsonStringCmp(obj->items[i]->key, key)) {
            *outValue = obj->items[i]->value;
            return true;
        }
    }
    return false;
}

JsonData *CreateJsonArray() {
    JsonArray *JsonArray = malloc(sizeof(JsonArray));
    JsonArray->base.type = JSON_ARRAY;
    JsonArray->size = 0;
    JsonArray->capacity = 4;
    JsonArray->items = malloc(JsonArray->capacity * sizeof(JsonData *));
    return (JsonData *)JsonArray;
}

bool AddToJsonArray(JsonArray *arr, JsonData *value) {
    if (arr->size >= arr->capacity) {
        arr->capacity *= 2;
        JsonData **newItems = realloc(arr->items, arr->capacity * sizeof(JsonData *));
        if (!newItems) return false;
        arr->items = newItems;
    }

    arr->items[arr->size++] = value;
    return true;
}

bool GetValueFromJsonArray(JsonArray *arr, size_t index, JsonData **outValue) {
    if (index >= arr->size) return false;
    *outValue = arr->items[index];
    return true;
}

#endif