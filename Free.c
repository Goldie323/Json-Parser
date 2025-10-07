#ifndef FREE_C
#define FREE_C

#include <stdlib.h>
#include "Consts.h"

void FreeJsonData(JsonData *data);

void FreeJsonString(JsonString *str) {
    if (!str) return;
    if (str->value) free(str->value);
    free(str);
}

void FreeJsonNumber(JsonNumber *num) {
    if (!num) return;
    free(num);
}

void FreeJsonBoolean(JsonBoolean *boolean) {
    if (!boolean) return;
    free(boolean);
}

void FreeJsonNull(JsonData *null) {
    if (!null) return;
    free(null);
}

void FreeJsonPair(JsonPair *pair) {
    if (!pair) return;
    if (pair->key) FreeJsonString(pair->key);
    if (pair->value) FreeJsonData(pair->value);
    free(pair);
}

void FreeJsonObject(JsonObject *obj) {
    if (!obj) return;
    if (!obj->items) {
        free(obj);
        return;
    }
    for (size_t i = 0; i < obj->size; i++) {
        FreeJsonPair(obj->items[i]);
    }
    free(obj->items);
    free(obj);
}

void FreeJsonArray(JsonArray *arr) {
    if (!arr) return;
    if (!arr->items) {
        free(arr);
        return;
    }
    for (size_t i = 0; i < arr->size; i++) {
        FreeJsonData(arr->items[i]);
    }
    free(arr->items);
    free(arr);
}

void FreeJsonData(JsonData *data) {
    if (!data) return;
    switch (data->type) {
        case JSON_STRING:
            FreeJsonString((JsonString *)data);
            break;
        case JSON_NUMBER:
            FreeJsonNumber((JsonNumber *)data);
            break;
        case JSON_BOOLEAN:
            FreeJsonBoolean((JsonBoolean *)data);
            break;
        case JSON_NULL:
            FreeJsonNull(data);
            break;
        case JSON_OBJECT:
            FreeJsonObject((JsonObject *)data);
            break;
        case JSON_ARRAY:
            FreeJsonArray((JsonArray *)data);
            break;
        default:
            free(data);
            break;
    }
}

#endif