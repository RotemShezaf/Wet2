#!/bin/bash
g++ -std=c++11 -Wall -Werror -pedantic-errors ./test/avltest2.cpp  -o ./test/avltest2.exe
chmod +x ./test/avltest2.exe
valgrind --leak-check=full --show-leak-kinds=all ./test/avltest2.exe 200 3
g++ -std=c++11 -Wall -Werror -pedantic-errors -DTEST -g ./test/hash_test.cpp ./hashTable.cpp -o ./test/hash_test.exe
chmod +x ./test/hash_test.exe
valgrind --leak-check=full --show-leak-kinds=all ./test/hash_test.exe 200
g++ -std=c++11 -DNDEBUG -Wall -o prog *.cpp  | grep -v /test*.cpp

TESTS_TO_RUN=4
EXECUTABLE=./prog

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

shopt -s nullglob
FAILED_TESTS=""

for i in ./test/inFiles/test*.in
do
	if [ ${i//[^0-9]/} -gt $TESTS_TO_RUN ]; then
		continue
	fi
	
	printf "test $i >>>  "
	$EXECUTABLE < $i > ./test/outFiles/test${i//[^0-9]/}.result
	diff ./test/outFiles/test${i//[^0-9]/}.out ./test/outFiles/test${i//[^0-9]/}.result

	if [ $? -eq 0 ]
	then
		printf "Test: ${GREEN}pass${NC},   "
	else
		printf "Test: ${RED}fail${NC},   "
		FAILED_TESTS+='-'
		FAILED_TESTS+='F'
	fi
	valgrind --log-file=$i.valgrind_log --leak-check=full $EXECUTABLE < $i 1>/dev/null 2>/dev/null
	if [ -f $i.valgrind_log ]
	then
		cat $i.valgrind_log | grep "ERROR SUMMARY: 0" > /dev/null
		if [ $? -eq 0 ]
		then
			printf "Leak: ${GREEN}pass${NC}\n"
		else
			printf "Leak: ${RED}fail${NC}\n"
			cat $i.valgrind_log
			FAILED_TESTS+="-"
		fi
	else
		printf "Leak: ${RED}couldnt get valgrind file${NC}\n"
		FAILED_TESTS+="-"
	fi
	rm $i.valgrind_log
done

if [ -z ${FAILED_TESTS} ]; then
	printf "\n${GREEN} All tests passed :)${NC}\n\n"
else
	printf "\n${RED} Failed ${FAILED_TESTS}${NC} tests.\n\n"
fi
printf "incress TESTS_TO_RUN in tester.sh in order to run more tests (up to 500)\n"
