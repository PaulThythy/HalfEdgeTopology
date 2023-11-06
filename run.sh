#!/bin/bash

make mrproper
make

if [ $? -eq 0 ]; then
	echo "Compilation successful. Running program..."
	./bin/main
else
	echo "Compilation failed. Please check the error messages."
fi
