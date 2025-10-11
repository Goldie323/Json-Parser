#ifndef JSONPARSER_C
#define JSONPARSER_C

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "JsonConsts.h"

JsonData *ParseJsonData(const char *arrStr);

JsonData *ParseJsonString(const char *str) {
    // this has to handle the escape characters
}

JsonData *ParseJsonNumber(const char *num) {
    char *expPos = strchr(num, 'e');
    if (!expPos) expPos = strchr(num, 'E');
    char *part1;
    char *part2;

    if (expPos) {
        size_t len_part1 = expPos - num;

        part1 = malloc(len_part1 + 1);
        if (!part1) return NULL;

        strncpy(part1, num, len_part1);
        part1[len_part1] = '\0';
        part2 = expPos + 1;
    } else {
        part1 = (char *)num;
    }

    JsonNumber *ParsedNumber = malloc(sizeof(JsonNumber));
    ParsedNumber->base.type = JSON_NUMBER;
    if (!ParsedNumber) {
        if (expPos) free(part1);
        return NULL;
    }

    ParsedNumber->value = strtod(part1, NULL);

    if (expPos) {
        long expValue = strtol(part2, NULL, 10);
        ParsedNumber->value *= pow(10.0, expValue);
        free(part1);
    }

    return (JsonData *)ParsedNumber;

}

JsonData *ParseJsonBoolean(const char *boolStr) {
    JsonBoolean *ParsedBool = malloc(sizeof(JsonBoolean));
    ParsedBool->base.type = JSON_BOOLEAN;
    if (strcmp("false", boolStr)) {
        ParsedBool->value = false;
        return (JsonData *)ParsedBool;
    }
    ParsedBool->value = true;
    return (JsonData *)ParsedBool;
}

JsonData *ParseJsonNull(JsonData *null) {
    JsonData *ParsedNull = malloc(sizeof(JsonData));
    ParsedNull->type = JSON_NULL;
    return ParsedNull;
}

JsonData *ParseJsonPair(const char *pairStr) {
    //this needs to handle a lot and so I don't want to do it yet.
}

JsonData *ParseJsonObject(const char *objStr) {
    //this needs to handle a lot and so I don't want to do it yet.
}

JsonData *ParseJsonArray(const char *arrStr) {
    //this needs to handle a lot and so I don't want to do it yet.
}

JsonData *ParseJsonData(const char *arrStr) {
    //this really needs to handle A LOT to work, wanna do this later
}

JsonData *ParseJson(const char *jsonStr) {
    //this needs to handle a lot and so I don't want to do it yet.
}

#endif