#ifndef JSONFREE_C
#define JSONFREE_C

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "JsonConsts.h"

const char *json_escape_table[256];
bool json_escape_initialized = 0;

#if defined(_POSIX_VERSION) || defined(__APPLE__) || defined(__unix__)
static inline char* dupstr(const char* s) { return strdup(s); }
#elif defined(_WIN32)
static inline char* dupstr(const char* s) { return _strdup(s); }
#else
static inline char* dupstr(const char* s) {
    size_t len = strlen(s);
    char* r = malloc(len + 1);
    if (r) memcpy(r, s, len + 1);
    return r;
}
#endif

// Compact Print Functions
char* PrintJsonData(JsonData *data);

static void init_json_escape_table() {
    if (json_escape_initialized) return;
    for (int i = 0; i < 256; i++) json_escape_table[i] = NULL;

    // Control characters
    json_escape_table[0] = "\\u0000";
    json_escape_table[1] = "\\u0001";
    json_escape_table[2] = "\\u0002";
    json_escape_table[3] = "\\u0003";
    json_escape_table[4] = "\\u0004";
    json_escape_table[5] = "\\u0005";
    json_escape_table[6] = "\\u0006";
    json_escape_table[7] = "\\u0007";
    json_escape_table[8] = "\\b";
    json_escape_table[9] = "\\t";
    json_escape_table[10] = "\\n";
    json_escape_table[11] = "\\u000B";
    json_escape_table[12] = "\\f";
    json_escape_table[13] = "\\r";
    json_escape_table[14] = "\\u000E";
    json_escape_table[15] = "\\u000F";
    json_escape_table[16] = "\\u0010";
    json_escape_table[17] = "\\u0011";
    json_escape_table[18] = "\\u0012";
    json_escape_table[19] = "\\u0013";
    json_escape_table[20] = "\\u0014";
    json_escape_table[21] = "\\u0015";
    json_escape_table[22] = "\\u0016";
    json_escape_table[23] = "\\u0017";
    json_escape_table[24] = "\\u0018";
    json_escape_table[25] = "\\u0019";
    json_escape_table[26] = "\\u001A";
    json_escape_table[27] = "\\u001B";
    json_escape_table[28] = "\\u001C";
    json_escape_table[29] = "\\u001D";
    json_escape_table[30] = "\\u001E";
    json_escape_table[31] = "\\u001F";

    // Special JSON characters
    json_escape_table['"']  = "\\\"";
    json_escape_table['\\'] = "\\\\";

    json_escape_initialized = 1;
}

char* PrintJsonString(JsonString *str) {
    init_json_escape_table();
    size_t newLen = 2;
    for (size_t i = 0; i < str->size; i++) {
        if (json_escape_table[(unsigned char)str->value[i]]) {
            newLen += strlen(json_escape_table[(unsigned char)str->value[i]]);
        } else {
            newLen += 1;
        }
    }

    char *buf = malloc(newLen + 1);
    if (!buf) return NULL;

    char *ptr = buf;
    *ptr++ = '"';
    for (size_t i = 0; i < str->size; i++) {
        const char *escapeSeq = json_escape_table[(unsigned char)str->value[i]];
        if (escapeSeq) {
            size_t escLen = strlen(escapeSeq);
            memcpy(ptr, escapeSeq, escLen);
            ptr += escLen;
        } else {
            *ptr++ = str->value[i];
        }
    }
    *ptr++ = '"';
    *ptr = '\0';

    return buf;
}

char* PrintJsonNumber(JsonNumber *num) {
    size_t len = snprintf(NULL, 0, "%g", num->value);
    if (len < 0) return NULL;

    char *buf = malloc(len + 1);
    if (!buf) return NULL;

    snprintf(buf, len + 1, "%g", num->value);
    return buf;
}

char *PrintJsonBoolean(JsonBoolean *boolean) {
    return boolean->value ? dupstr("true") : dupstr("false");
}

char *PrintJsonNull() {
    return dupstr("null");
}

char *PrintJsonPair(JsonPair *pair) {
    char *keyStr = PrintJsonString(pair->key);
    if (!keyStr) return NULL;

    char *valueStr = PrintJsonData(pair->value);
    if (!valueStr) {
        free(keyStr);
        return NULL;
    }

    size_t len = snprintf(NULL, 0, "%s:%s", keyStr, valueStr);
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
        char *emptyObj = dupstr("{}");
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
        char *emptyArr = dupstr("[]");
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

// Pretty Print Functions
char *PPrintJsonData(JsonData *data, int indent);

static char *make_indent(int indent) {
    if (indent <= 0) return dupstr("");
    char *buf = malloc(indent + 1);
    memset(buf, ' ', indent);
    buf[indent] = '\0';
    return buf;
}

char* PPrintJsonPair(JsonPair *pair, int indent, int nextIndent) {
    char *keyStr = PrintJsonString(pair->key);
    char *valueStr = PPrintJsonData(pair->value, nextIndent);
    char *indentStr = make_indent(indent);

    size_t len = snprintf(NULL, 0, "%s%s: %s", indentStr, keyStr, valueStr);
    char *buf = malloc(len + 1);
    if (!buf) {
        free(keyStr);
        free(valueStr);
        free(indentStr);
        return NULL;
    }
    snprintf(buf, len + 1, "%s%s: %s", indentStr, keyStr, valueStr);

    free(keyStr);
    free(valueStr);
    free(indentStr);
    return buf;
}

char* PPrintJsonObject(JsonObject *obj, int indent) {
    if (obj->size == 0) return dupstr("{}");

    int nextIndent = indent + 2;
    char **pairStrs = malloc(obj->size * sizeof(char*));
    if (!pairStrs) return NULL;
    size_t totalLen = indent + 4;

    for (size_t i = 0; i < obj->size; i++) {
        pairStrs[i] = PPrintJsonPair(obj->items[i], nextIndent, nextIndent);
        totalLen += strlen(pairStrs[i]) + 2;
    }

    char *indentStr = make_indent(indent);
    totalLen += strlen(indentStr) + 3;

    char *buf = malloc(totalLen);
    if (!buf) {
        for (size_t i = 0; i < obj->size; i++) free(pairStrs[i]);
        free(pairStrs);
        free(indentStr);
        return NULL;
    }
    char *ptr = buf;

    ptr += sprintf(ptr, "{\n");
    for (size_t i = 0; i < obj->size; i++) {
        ptr += sprintf(ptr, "%s", pairStrs[i]);
        if (i < obj->size - 1)
            ptr += sprintf(ptr, ",\n");
        else
            ptr += sprintf(ptr, "\n");
        free(pairStrs[i]);
    }
    sprintf(ptr, "%s}", indentStr);

    free(pairStrs);
    free(indentStr);
    return buf;
}

char* PPrintJsonArray(JsonArray *array, int indent) {
    if (array->size == 0) return dupstr("[]");

    int nextIndent = indent + 2;
    char **itemStrs = malloc(array->size * sizeof(char*));
    if (!itemStrs) return NULL;
    size_t totalLen = indent + 4;

    for (size_t i = 0; i < array->size; i++) {
        char *valueStr = PPrintJsonData(array->items[i], nextIndent);
        char *lineIndent = make_indent(nextIndent);
        int lineLen = snprintf(NULL, 0, "%s%s", lineIndent, valueStr);
        itemStrs[i] = malloc(lineLen + 1);
        if (!itemStrs[i]) {
            for (size_t j = 0; j < i; j++) free(itemStrs[j]);
            free(itemStrs);
            free(lineIndent);
            free(valueStr);
            return NULL;
        }
        snprintf(itemStrs[i], lineLen + 1, "%s%s", lineIndent, valueStr);
        totalLen += strlen(itemStrs[i]) + 2;
        free(lineIndent);
        free(valueStr);
    }

    char *indentStr = make_indent(indent);
    totalLen += strlen(indentStr) + 3;

    char *buf = malloc(totalLen);
    if (!buf) {
        for (size_t i = 0; i < array->size; i++) free(itemStrs[i]);
        free(itemStrs);
        free(indentStr);
        return NULL;
    }
    char *ptr = buf;

    ptr += sprintf(ptr, "[\n");
    for (size_t i = 0; i < array->size; i++) {
        ptr += sprintf(ptr, "%s", itemStrs[i]);
        if (i < array->size - 1)
            ptr += sprintf(ptr, ",\n");
        else
            ptr += sprintf(ptr, "\n");
        free(itemStrs[i]);
    }
    sprintf(ptr, "%s]", indentStr);

    free(itemStrs);
    free(indentStr);
    return buf;
}

char* PPrintJsonData(JsonData *data, int indent) {
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
            return PPrintJsonObject((JsonObject*)data, indent);
        case JSON_ARRAY:
            return PPrintJsonArray((JsonArray*)data, indent);
        default:
            return NULL;
    }
}

#endif