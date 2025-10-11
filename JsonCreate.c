#ifndef JSONCREATE_C
#define JSONCREATE_C

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "JsonConsts.h"
#include "JsonTools.h"

JsonData *CreateJsonString(const char *value, size_t length, bool isKey) {
    JsonString *str = malloc(sizeof(JsonString));
    if (!str) return NULL;
    str->base.type = JSON_STRING;
    str->size = length;
    if (isKey) {
        str->capacity = length;
    } else {
        str->capacity = RoundUpToPowerOfTwo(length);
    }
    str->value = malloc(str->capacity);
    if (!str->value) {
        free(str);
        return NULL;
    }
    memcpy(str->value, value, str->size);
    return (JsonData *)str;
}

JsonData *CreateJsonStringNullTerminated(const char *value, bool isKey) {
    return CreateJsonString(value, strlen(value), isKey);
}

JsonData *CreateJsonNumber(double value) {
    JsonNumber *num = malloc(sizeof(JsonNumber));
    if (!num) return NULL;
    num->base.type = JSON_NUMBER;
    num->value = value;
    return (JsonData *)num;
}

JsonData *CreateJsonBoolean(bool value) {
    JsonBoolean *b = malloc(sizeof(JsonBoolean));
    if (!b) return NULL;
    b->base.type = JSON_BOOLEAN;
    b->value = value;
    return (JsonData *)b;
}

JsonData *CreateJsonNull() {
    JsonData *n = malloc(sizeof(JsonData));
    if (!n) return NULL;
    n->type = JSON_NULL;
    return n;
}

JsonData *CreateJsonObject() {
    JsonObject *obj = malloc(sizeof(JsonObject));
    if (!obj) return NULL;
    obj->base.type = JSON_OBJECT;
    obj->size = 0;
    obj->capacity = 4;
    obj->items = malloc(obj->capacity * sizeof(JsonPair *));
    if (!obj->items) {
        free(obj);
        return NULL;
    }
    return (JsonData *)obj;
}

bool DoesKeyExistInJsonObject(void *obj, void *key) {
    JsonObject *jsonObj = (JsonObject *)obj;
    JsonString *jsonKey = (JsonString *)key;
    if (!jsonObj || jsonObj->base.type != JSON_OBJECT) return false;
    if (!jsonKey || jsonKey->base.type != JSON_STRING) return false;
    for (size_t i = 0; i < jsonObj->size; i++) {
        if (JsonStringCmp(jsonObj->items[i]->key, jsonKey)) {
            return true;
        }
    }
    return false;
}

bool AddToJsonObject(void *obj, void *key, void *value) {
    JsonObject *jsonObj = (JsonObject *)obj;
    JsonString *jsonKey = (JsonString *)key;
    JsonData *jsonValue = (JsonData *)value;
    if (!jsonObj || jsonObj->base.type != JSON_OBJECT) return false;
    if (!jsonKey || jsonKey->base.type != JSON_STRING) return false;
    if (!jsonValue) return false;

    if (DoesKeyExistInJsonObject(jsonObj, jsonKey)) {
        return false;
    }

    if (jsonObj->size >= jsonObj->capacity) {
        jsonObj->capacity *= 2;
        JsonPair **newItems = realloc(jsonObj->items, jsonObj->capacity * sizeof(JsonPair *));
        if (!newItems) return false;
        jsonObj->items = newItems;
    }

    JsonPair *pair = malloc(sizeof(JsonPair));
    pair->key = jsonKey;
    pair->value = jsonValue;
    jsonObj->items[jsonObj->size++] = pair;
    return true;
}

JsonData *GetValueFromJsonObject(void *obj, void *key) {
    JsonObject *jsonObj = (JsonObject *)obj;
    JsonString *jsonKey = (JsonString *)key;
    if (!jsonObj || jsonObj->base.type != JSON_OBJECT) return NULL;
    if (!jsonKey || jsonKey->base.type != JSON_STRING) return NULL;
    for (size_t i = 0; i < jsonObj->size; i++) {
        if (JsonStringCmp(jsonObj->items[i]->key, jsonKey)) {
            return jsonObj->items[i]->value;
        }
    }
    return NULL;
}

JsonData *CreateJsonArray() {
    JsonArray *arr = malloc(sizeof(JsonArray));
    if (!arr) return NULL;
    arr->base.type = JSON_ARRAY;
    arr->size = 0;
    arr->capacity = 4;
    arr->items = malloc(arr->capacity * sizeof(JsonData *));
    if (!arr->items) {
        free(arr);
        return NULL;
    }
    return (JsonData *)arr;
}

bool AddToJsonArray(void *arr, void *value) {
    JsonArray *jsonArr = (JsonArray *)arr;
    JsonData *jsonValue = (JsonData *)value;
    if (!jsonArr || jsonArr->base.type != JSON_ARRAY) return false;
    if (!jsonValue) return false;
    if (jsonArr->size >= jsonArr->capacity) {
        jsonArr->capacity *= 2;
        JsonData **newItems = realloc(jsonArr->items, jsonArr->capacity * sizeof(JsonData *));
        if (!newItems) return false;
        jsonArr->items = newItems;
    }

    jsonArr->items[jsonArr->size++] = jsonValue;
    return true;
}

JsonData *GetValueFromJsonArray(void *arr, size_t index) {
    JsonArray *jsonArr = (JsonArray *)arr;
    if (!jsonArr || jsonArr->base.type != JSON_ARRAY) return NULL;
    if (index >= jsonArr->size) return NULL;
    return jsonArr->items[index];
}

size_t GetJsonArraySize(void *arr) {
    JsonArray *jsonArr = (JsonArray *)arr;
    if (!jsonArr || jsonArr->base.type != JSON_ARRAY) return 0;
    return jsonArr->size;
}

char *GetStringFromJson(void *data) {
    if (!data) return NULL;
    JsonString *str = (JsonString *)data;
    if (str->base.type != JSON_STRING) return NULL;
    char *result = malloc(str->size + 1);
    if (!result) return NULL;
    memcpy(result, str->value, str->size);
    result[str->size] = '\0';
    return result;
}

double GetNumberFromJson(void *data, bool *success) {
    if (!data) {
        if (success) *success = false;
        return 0.0;
    }
    JsonNumber *num = (JsonNumber *)data;
    if (num->base.type != JSON_NUMBER) {
        if (success) *success = false;
        return 0.0;
    }
    if (success) *success = true;
    return num->value;
}

bool GetBooleanFromJson(void *data, bool *success) {
    if (!data) {
        if (success) *success = false;
        return false;
    }
    JsonBoolean *b = (JsonBoolean *)data;
    if (b->base.type != JSON_BOOLEAN) {
        if (success) *success = false;
        return false;
    }
    if (success) *success = true;
    return b->value;
}

#endif