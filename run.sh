#!/bin/bash
make
for file in OS_PJ1_Test/*
do
	echo "Processing " "$file" >&2
	name=$(basename ${file} .txt)
	./scheduler < "$file" > output/${name}_stdout.txt;
	dmesg -t | grep Project1 > output/${name}_dmesg.txt;
	sudo dmesg -c;
done
make clean
