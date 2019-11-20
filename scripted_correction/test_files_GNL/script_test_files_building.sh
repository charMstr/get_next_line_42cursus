#!/bin/sh

for i in `seq 0 5`
do
	touch test_file${i}
	for j in `seq 0 ${i}`
	do
		if [ ${j} != 0 ]
		then
			echo "" >>test_file${i}	
		fi
	done
	echo "." >>test_file${i}
done
