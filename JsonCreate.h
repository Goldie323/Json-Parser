#ifndef JSONCREATE_H
#define JSONCREATE_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "JsonConsts.h"
#include "JsonTools.h"

JsonData *CreateJsonString(const char *value, size_t length, bool isKey);
JsonData *CreateJsonStringNullTerminated(const char *value, bool isKey);
JsonData *CreateJsonNumber(double value);
JsonData *CreateJsonBoolean(bool value);
JsonData *CreateJsonNull();
JsonData *CreateJsonObject();
JsonData *CreateJsonArray();

bool DoesKeyExistInJsonObject(void *obj, void *key);
bool AddToJsonObject(void *obj, void *key, void *value);
JsonData *GetValueFromJsonObject(void *obj, void *key);

bool AddToJsonArray(void *arr, void *value);
JsonData *GetFromJsonArray(void *arr, size_t index);
size_t GetJsonArraySize(void *arr);

#endif