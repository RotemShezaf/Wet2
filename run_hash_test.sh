#!/bin/bash
g++ -std=c++11 -Wall -Werror -pedantic-errors -DTEST -g test/hash_test.cpp hashTable.cpp -o test/hash_test.exe
chmod +x test/hash_test.exe
valgrind --leak-check=full --show-leak-kinds=all test/hash_test.exe 200