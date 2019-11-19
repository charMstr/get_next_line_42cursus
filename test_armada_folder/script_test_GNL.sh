#!/bin/sh

SIZE=3
TEST=3
TEST_FILE=./test_files_GNL/test_file${TEST}

#make re MAIN=mains/${MAIN_NAME} BUF_SIZE=${SIZE}
make re BUF_SIZE=${SIZE}
./get_next_line ${TEST_FILE}
diff user_output ${TEST_FILE}
echo "$?"

for MAIN_NAME in  mains/*
do
	if [ $MAIN_NAME != "mains/includes" ]
	then
		for TEST in `seq 1 3`
		do
			TEST_FILE=./test_files_GNL/test_file${TEST}
			echo "\ttest_file is test_file${TEST}"
			for SIZE in `seq 1 37`
			do
				make re MAIN=${MAIN_NAME} BUF_SIZE=${SIZE}
				./get_next_line ${TEST_FILE}
				diff user_output ${TEST_FILE}
				echo "test done: diff was: $?"
			done
		done
	fi
done
