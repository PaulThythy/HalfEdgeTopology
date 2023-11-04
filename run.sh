#!/bin/bash

make

if [ $? -eq 0 ]; then
	echo "Compilation successful. Runnning program..."
	./bin/main
else
	echo "Compilation failed. Please check the error messages."
fi
