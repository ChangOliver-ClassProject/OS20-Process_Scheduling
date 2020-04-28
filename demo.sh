#!/bin/bash

FILE=('OS_PJ1_Test/TIME_MEASUREMENT.txt' 'OS_PJ1_Test/FIFO_1.txt' 'OS_PJ1_Test/PSJF_2.txt' 'OS_PJ1_Test/RR_3.txt' 'OS_PJ1_Test/SJF_4.txt')
make
for file in ${FILE[@]}
do
	echo "Processing " "$file" >&2
	name=$(basename ${file} .txt)
	./scheduler < "$file" | tee demo/${name}_stdout.txt;
	dmesg -t | grep Project1 |tee demo/${name}_dmesg.txt;
	sudo dmesg -c;
done
make clean