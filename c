#!/bin/bash

arduino-cli compile -b esp32:esp32:ttgo-t7-v14-mini32 examples/VGA/$1 --board-options PartitionScheme=min_spiffs --verbose
#UploadSpeed=230400 --upload -p /dev/ttyACM0 --verbose
