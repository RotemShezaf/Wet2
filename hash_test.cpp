#include <iostream>
#include <vector>
#include <ostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <memory>
#include <assert.h>
#include "hashTable.h"


void test_table(std::vector<int> &vector)
{
    HashTable table;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle (vector.begin(), vector.end(), std::default_random_engine(seed));
    for (int num : vector) {
        table.add_customer(num, num);
        if (!table.is_valid()) {
            table.print();
            assert(false);
        }
    }
}

int main(int argc, char **argv)
{
    int size = (argc >= 2) ? std::atoi(argv[1]) : 200;

    std::vector<int> vector;
    for(int i = 0; i < size; ++i) {
        vector.push_back(i);
    }
    test_table(vector);
}