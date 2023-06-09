#include "../rankTree.h"
#include <iostream>
#include <cassert>

using Avl = RankTree<int, int>;
using Node = Avl::Node;

void print_node(const Node *node, int deapth)
{
    if (!node) {
        return;
    }
    print_node(node->_left, deapth + 1);
    for (int i = 0; i < deapth; ++i) std::cout << '\t';
    std::cout << node->_key << ' ' << node->_rank << ' ' << node->_height << std::endl;
    print_node(node->_right, deapth + 1);
}

void print_tree(const Avl &tree)
{
    std::cout << "size: " << tree.size() << " first: ";
    if (!tree.is_empty()) {
        std::cout << tree.get_first();
    } else {
        std::cout << "none";
    }
    std::cout << std::endl;
    print_node(tree.root(), 0);
    std::cout << std::endl;
}

void avl_test()
{
    Avl tr;
    print_tree(tr);
    for(int i = 1; i < 16; ++i) {
        tr.insert(i, i);
        print_tree(tr);
    }
    for(int i = 2; i < 16; i += 2) {
        tr.add_rank( i, 5);
        print_tree(tr);
    }
    for(int i = 16; i < 32; ++i) {
        tr.insert(i, i);
        print_tree(tr);
    }
}