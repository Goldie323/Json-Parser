#ifndef JSONFREE_C
#define JSONFREE_C

#include <stdlib.h>

static const char *json_escape_table[256] = {
    "\\u0000","\\u0001","\\u0002","\\u0003","\\u0004","\\u0005","\\u0006","\\u0007",
    "\\b","\\t","\\n","\\u000B","\\f","\\r","\\u000E","\\u000F",
    "\\u0010","\\u0011","\\u0012","\\u0013","\\u0014","\\u0015","\\u0016","\\u0017",
    "\\u0018","\\u0019","\\u001A","\\u001B","\\u001C","\\u001D","\\u001E","\\u001F",
    NULL,NULL,"\\\"",NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL
};


#include "JsonConsts.h"

char* PrintJsonData(JsonData *data);

char* PrintJsonString(JsonString *str) {
    return "NOT DONE YET, STRING IS NOT OUTPUTED";
}

char* PrintJsonNumber(JsonNumber *num) {
    int len = snprintf(NULL, 0, "%g", num->value);
    if (len < 0) return NULL;

    char *buf = malloc(len + 1);
    if (!buf) return NULL;

    snprintf(buf, len + 1, "%g", num->value);
    return buf;
}

char *PrintJsonBoolean(JsonBoolean *boolean) {
    return boolean->value ? strdup("true") : strdup("false");
}

char *PrintJsonNull() {
    return strdup("null");
}

char *PrintJsonPair(JsonPair *pair) {
    char *keyStr = PrintJsonString(pair->key);
    if (!keyStr) return NULL;

    char *valueStr = PrintJsonData(pair->value);
    if (!valueStr) {
        free(keyStr);
        return NULL;
    }

    int len = snprintf(NULL, 0, "%s:%s", keyStr, valueStr);
    if (len < 0) {
        free(keyStr);
        free(valueStr);
        return NULL;
    }

    char *buf = malloc(len + 1);
    if (!buf) {
        free(keyStr);
        free(valueStr);
        return NULL;
    }

    snprintf(buf, len + 1, "%s:%s", keyStr, valueStr);
    free(keyStr);
    free(valueStr);
    return buf;
}

char *PrintJsonObject(JsonObject *obj) {
    if (obj->size == 0) {
        char *emptyObj = strdup("{}");
        if (!emptyObj) return NULL;
        return emptyObj;
    }

    size_t totalLen = 2;
    char **pairStrs = malloc(obj->size * sizeof(char*));
    if (!pairStrs) return NULL;

    for (size_t i = 0; i < obj->size; i++) {
        pairStrs[i] = PrintJsonPair(obj->items[i]);
        if (!pairStrs[i]) {
            for (size_t j = 0; j < i; j++) free(pairStrs[j]);
            free(pairStrs);
            return NULL;
        }
        totalLen += strlen(pairStrs[i]) + 1;
    }
    totalLen--;

    char *buf = malloc(totalLen + 1);
    if (!buf) {
        for (size_t i = 0; i < obj->size; i++) free(pairStrs[i]);
        free(pairStrs);
        return NULL;
    }

    char *ptr = buf;
    *ptr++ = '{';
    for (size_t i = 0; i < obj->size; i++) {
        size_t len = strlen(pairStrs[i]);
        memcpy(ptr, pairStrs[i], len);
        ptr += len;
        if (i < obj->size - 1) {
            *ptr++ = ',';
        }
        free(pairStrs[i]);
    }
    *ptr++ = '}';
    *ptr = '\0';

    free(pairStrs);
    return buf;
}

char *PrintJsonArray(JsonArray *array) {
    if (array->size == 0) {
        char *emptyArr = strdup("[]");
        if (!emptyArr) return NULL;
        return emptyArr;
    }

    size_t totalLen = 2;
    char **itemStrs = malloc(array->size * sizeof(char*));
    if (!itemStrs) return NULL;

    for (size_t i = 0; i < array->size; i++) {
        itemStrs[i] = PrintJsonData(array->items[i]);
        if (!itemStrs[i]) {
            for (size_t j = 0; j < i; j++) free(itemStrs[j]);
            free(itemStrs);
            return NULL;
        }
        totalLen += strlen(itemStrs[i]) + 1;
    }
    totalLen--;

    char *buf = malloc(totalLen + 1);
    if (!buf) {
        for (size_t i = 0; i < array->size; i++) free(itemStrs[i]);
        free(itemStrs);
        return NULL;
    }

    char *ptr = buf;
    *ptr++ = '[';
    for (size_t i = 0; i < array->size; i++) {
        size_t len = strlen(itemStrs[i]);
        memcpy(ptr, itemStrs[i], len);
        ptr += len;
        if (i < array->size - 1) {
            *ptr++ = ',';
        }
        free(itemStrs[i]);
    }
    *ptr++ = ']';
    *ptr = '\0';

    free(itemStrs);
    return buf;
}

char *PrintJsonData(JsonData *data) {
    switch (data->type) {
        case JSON_STRING:
            return PrintJsonString((JsonString*)data);
        case JSON_NUMBER:
            return PrintJsonNumber((JsonNumber*)data);
        case JSON_BOOLEAN:
            return PrintJsonBoolean((JsonBoolean*)data);
        case JSON_NULL:
            return PrintJsonNull();
        case JSON_OBJECT:
            return PrintJsonObject((JsonObject*)data);
        case JSON_ARRAY:
            return PrintJsonArray((JsonArray*)data);
        default:
            return NULL;
    }
}

#endif