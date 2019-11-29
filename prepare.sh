#!/bin/bash

MM="buddy.c"

if [[ "$1" == "buddy" ]]; then
	MM="memory.c"
fi

cd Kernel
mv *.c src 2>/dev/null
cd src
mv $MM ../

