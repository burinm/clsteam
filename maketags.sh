#!/bin/bash

SDK_ROOT=~/SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.4

EM_LIB=${SDK_ROOT}/platform/emlib
PLATFORM=${SDK_ROOT}/platform/Device/SiliconLabs/EFR32BG1B
BLE_LIB=${SDK_ROOT}/protocol/bluetooth/lib/EFR32BG1B

find -iname "*.h" -or -iname "*.c" -or -iname "*.cpp" -or -iname "*.s" -or -name Makefile  -or -name *.xml > cscope.files

find ${EM_LIB} -iname "*.h" -or -iname "*.c" -or -iname "*.cpp" -or -iname "*.s" -or -name Makefile >> cscope.files
find ${PLATFORM} -iname "*.h" -or -iname "*.c" -or -iname "*.cpp" -or -iname "*.s" -or -name Makefile >> cscope.files
find ${BLE_LIB} -iname "*.h" -or -iname "*.c" -or -iname "*.cpp" -or -iname "*.s" -or -name Makefile >> cscope.files

cscope -b
ctags -L ./cscope.files
cscope
