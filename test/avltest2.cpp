#include <iostream>
#include <vector>
#include <ostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <memory>
#include <assert.h>
#include "../rankTree.h"

using Avl = RankTree<int, int>;
using Node = Avl::Node;

int BalanceFactor(Node *node)
{
    return HEIGHT(node->_left) - HEIGHT(node->_right);
}

bool is_node_valid(Node *node)
{
    if(!node) {
        return true;
    }
    // Checks the height of every node is valid
    if (node->_height != 1 + std::max(HEIGHT(node->_left), HEIGHT(node->_right))) {
        std::cout << "Height is not valid\n";
        return false;
    }
    if (!node->_left && !node->_right && node->_height != 0) {
        std::cout << "Leaf Height is not valid\n";
        return false;
    }
    //checks the Tree is a Binary Search Tree
    if (node->_left && node->_left->_key >= node->_key) {
        std::cout << "Left son is not smaller than root\n";
        return false;
    }
    if (node->_right && node->_right->_key <= node->_key) {
        std::cout << "Right son is not bigger than root\n";
        return false;
    }
    //maybe TODO: check rank
    //checks that the Balance Factor of every node is valid
    if (std::abs(BalanceFactor(node)) > 1) {
        std::cout << "Balance Factor is not valid\n";
        return false;
    }
    //maybe TODO: validate parant
    return is_node_valid(node->_left) && is_node_valid(node->_right);
}

bool is_tree_valid(const Avl &tree)
{
    return is_node_valid(tree.root());
}

void print_node(const Node *node, int deapth)
{
    if (!node) {
        return;
    }
    print_node(node->_left, deapth + 1);
    for (int i = 0; i < deapth; ++i) std::cout << '\t';
    std::cout << node->_value << '\n';
    print_node(node->_right, deapth + 1);
}

void print_tree(const Avl &tree)
{
    print_node(tree.root(), 0);
    std::cout << '\n';
}

void test_tree(std::vector<int> &vector)
{
    Avl tree;
    std::vector<int> vector2;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle (vector.begin(), vector.end(), std::default_random_engine(seed));
    for (unsigned i = 0; i < vector.size(); i++) {
        vector2.push_back(0);
    }
    std::cout<<"\033[34mCreating The Tree\033[39m\n\n";
    std::cout << "\033[34mfinush\033[39m\n\n";
    for (unsigned i = 0; i < vector.size(); i++) {
        int num = vector[i];
        //std::cout << "\033[93minserting " << num << "\033[39m\n";
        std::srand(std::time(nullptr));
        tree.insert(num, num);
        int amount = std::rand();
        int id = (std::rand()) % (num + 1);
        tree.add_rank(  id, amount);
        for (unsigned j = 0; j<= i; j++) {
            if (vector[j] < id  ) {
                vector2[j] += amount;
            }
        }
        assert(is_tree_valid(tree));
        //print_tree(tree);
    }
    std::cout << "\033[34mfinush\033[39m\n\n";
    for (unsigned i = 0; i < vector.size(); i++) {
        int rank = tree.get_rank(vector[i]);
        assert(rank == vector2[i]);
    }
    std::cout << "\033[34mfinush\033[39m\n\n";
}

int main(int argc, char **argv)
{
    int size = (argc >= 2) ? std::atoi(argv[1]) : 200;
    int repeat = (argc >= 3) ? std::atoi(argv[2]) : 3;

    Avl tree;
    std::vector<int> vector;
    for(int i = 0; i < size; ++i) {
        vector.push_back(i);
    }
    for (int k = 1; k <= repeat; ++k) {
        if(k != 0){
            std::cout << "\033[34mStaring tree #" << k << "\033[39m\n\n";
        }
        test_tree(vector);
    }
    std::cout << "\033[32mGreat!\033[39m\n";
}