#!/bin/sh

gcc tranlib/internalFunc.c -c -o tranlib/internal.o -g
gcc tranlib/tranIO.c -c -o tranlib/tranIO.o -g
make
./main $1 > compileInfo.txt
as tryRun.s -o tryRun.o -g
gcc tryRun.o tranlib/tranIO.o tranlib/internal.o -o exe -g
rm tryRun.s
rm tryRun.o