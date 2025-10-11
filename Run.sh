#!/bin/bash

gcc ./Test.c ./JsonTools.c ./JsonPrint.c ./JsonParser.c ./JsonFree.c ./JsonCreate.c -o ./Test -lm
./Test
rm ./Test