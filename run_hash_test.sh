#!/bin/bash
g++ -std=c++11 -Wall -Werror -pedantic-errors -DTEST -g hash_test.cpp hashTable.cpp -o hash_test.exe
chmod +x hash_test.exe
valgrind --leak-check=full --show-leak-kinds=all ./hash_test.exe 200