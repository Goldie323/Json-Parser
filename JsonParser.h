#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "JsonConsts.h"

JsonData *ParseJson(const char *jsonStr);

#endif