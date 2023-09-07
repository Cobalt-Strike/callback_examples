#!/bin/bash

i686-w64-mingw32-gcc -c bof_callback.c -o bof_callback.x86.o
x86_64-w64-mingw32-gcc -c bof_callback.c -o bof_callback.x64.o
