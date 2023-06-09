#include "hashTable.h"
#include <new>
#ifdef TEST
#include <algorithm>
#include <iostream>
#endif

HashTable::Customer::Customer(int c_id, int phone) :
    id(c_id),
    phone(phone),
    member(false)
{}

HashTable::Node::Node(int c_id, int phone) :
    customer(Customer(c_id, phone)),
    left(nullptr),
    right(nullptr),
    height(0)
{}

HashTable::Node::~Node()
{
    delete left;
    delete right;
}

HashTable::HashTable() :
    size(0),
    customer_count(0),
    customers(nullptr),
    latest_member(nullptr)
{}

HashTable::~HashTable()
{
    for(int i = 0; i < size; ++i) {
        delete customers[i];
    }
    delete [] customers;
}

int HashTable::get_height(Node *node) const
{
    return (node == nullptr) ? -1 : node->height;
}

int HashTable::get_balance(Node *node) const
{
    return get_height(node->left) - get_height(node->right);
}

void HashTable::set_height(Node *node)
{
    int l_height = get_height(node->left), r_height = get_height(node->right);
    int max = l_height > r_height ? l_height : r_height;
    node->height = max + 1;
}

HashTable::Node *HashTable::ll_roll(Node *node)
{
    Node *root = node->left;
    node->left = node->left->right;
    root->right = node;
    set_height(node);
    set_height(root);
    return root;
}

HashTable::Node *HashTable::lr_roll(Node *node)
{
    Node *temp = node->left, *root = node->left->right;
    node->left = root->right;
    temp->right = root->left;
    root->left = temp;
    root->right = node;
    set_height(node);
    set_height(temp);
    set_height(root);
    return root;
}

HashTable::Node *HashTable::rr_roll(Node *node)
{
    Node *root = node->right;
    node->right = node->right->left;
    root->left = node;
    set_height(node);
    set_height(root);
    return root;
}

HashTable::Node *HashTable::rl_roll(Node *node)
{
    Node *temp = node->right, *root = node->right->left;
    node->right = root->left;
    temp->left = root->right;
    root->right = temp;
    root->left = node;
    set_height(node);
    set_height(temp);
    set_height(root);
    return root;
}

HashTable::Node *HashTable::rebalance(Node *node)
{
    int balance = get_balance(node);
    if (balance == 2) {
        if (get_balance(node->left) == -1) {
            return lr_roll(node);
        }
        return ll_roll(node);
    }
    if (balance == -2) {
        if (get_balance(node->right) == 1) {
            return rl_roll(node);
        }
        return rr_roll(node);
    }
    set_height(node);
    return node;
}

HashTable::Node *HashTable::find(Node *node, int c_id) const
{
    if(node == nullptr) {
        return nullptr;
    }
    if (node->customer.id < c_id) {
        return find(node->left, c_id);
    }
    if (c_id < node->customer.id) {
        return find(node->right, c_id);
    }
    return node;
}

HashTable::Node *HashTable::insert(Node *root, Node *node)
{
    if (root == nullptr) {
        return node;
    }
    if (node->customer.id < root->customer.id) {
        root->left = insert(root->left, node);
    } else {
        root->right = insert(root->right, node);
    }
    return rebalance(root);
}

void HashTable::move_over(Node *node)
{
    if (node == nullptr) {
        return;
    }
    move_over(node->left);
    move_over(node->right);
    node->left = nullptr;
    node->right = nullptr;
    node->height = 0;
    int h = hash(node->customer.id);
    customers[h] = insert(customers[h], node);
}

// may throw std::bad_alloc
void HashTable::expand()
{
    if(size == 0) {
        customers = new Node*[1];
        customers[0] = nullptr;
        size = 1;
        return;
    }
    int old_size = size;
    Node **old_customers = customers;
    customers = new Node*[size * FACTOR];
    size = size * FACTOR;
    for(int i = 0; i < size; ++i) {
        customers[i] = nullptr;
    }
    for(int i = 0; i < old_size; ++i) {
        move_over(old_customers[i]);
    }
    delete [] old_customers;
}

int HashTable::hash(int c_id) const
{
    return c_id % size;
}

#ifdef TEST

bool HashTable::is_valid(Node *node) const
{
    if(!node) {
        return true;
    }
    int expected = 1 + std::max(get_height(node->left), get_height(node->right));
    if (node->height != expected) {
        std::cout << "id: " << node->customer.id
                  << "\nheight: expected: " << expected
                  << " actual: " << node->height << '\n';
        return false;
    }
    if (node->left && node->left->customer.id >= node->customer.id) {
        std::cout << "id: " << node->customer.id
                  << " left id: " << node->left->customer.id << '\n';
        return false;
    }
    if (node->right && node->right->customer.id <= node->customer.id) {
        std::cout << "id: " << node->customer.id
                  << " right id: " << node->right->customer.id << '\n';
        return false;
    }
    if (std::abs(get_balance(node)) > 1) {
        std::cout << "id: " << node->customer.id
                  << "\nleft hight: " << get_height(node->left)
                  << " right hight: " << get_height(node->right) << '\n';
        return false;
    }
    return is_valid(node->left) && is_valid(node->right);
}

void HashTable::print_node(Node *node, int space) const
{
    if (node == nullptr) {
        return;
    }
    print_node(node->left, space + 1);
    for(int i = 0; i < space; ++i) std::cout << ' ';
    std::cout << "id: " << node->customer.id << " phone: " << node->customer.phone << '\n';
    print_node(node->right, space + 1);
}

#endif //TEST

//==================== public member functions =======================

StatusType HashTable::add_customer(int c_id, int phone)
{
    if(c_id < 0 || phone < 0) {
        return INVALID_INPUT;
    }
    if(size != 0 && find(customers[hash(c_id)], c_id) != nullptr) {
        return ALREADY_EXISTS;
    }
    Node *node;
    try {
        if (size == customer_count) {
            expand();
        }
        node = new Node(c_id, phone);
    } catch (const std::bad_alloc &) {
        return ALLOCATION_ERROR;
    }
    int h = hash(c_id);
    customers[h] = insert(customers[h], node);
    customer_count++;
    return SUCCESS;
}

Output_t<int> HashTable::get_phone(int c_id) const
{
    if(c_id < 0) {
        return INVALID_INPUT;
    }
    Node *node = find(customers[hash(c_id)], c_id);
    if (node == nullptr) {
        return DOESNT_EXISTS;
    }
    return node->customer.phone;
}

Output_t<bool> HashTable::is_member(int c_id) const
{
    if(c_id < 0) {
        return INVALID_INPUT;
    }
    Node *node = find(customers[hash(c_id)], c_id);
    if (node == nullptr) {
        return DOESNT_EXISTS;
    }
    return node->customer.member;
}

StatusType HashTable::make_member(int c_id)
{
    if(c_id < 0) {
        return INVALID_INPUT;
    }
    Node *node = find(customers[hash(c_id)], c_id);
    if (node == nullptr) {
        return DOESNT_EXISTS;
    }
    if (node->customer.member) {
        return ALREADY_EXISTS;
    }
    node->customer.member = true;
    latest_member = node;
    return SUCCESS;
}

void HashTable::unmember_latest()
{
    latest_member->customer.member = false;
    latest_member = nullptr;
}

#ifdef TEST

bool HashTable::is_valid() const
{
    for(int i = 0; i < size; ++i) {
        if (!is_valid(customers[i])) {
            std::cout << "at customers[" << i << "]\n";
            return false;
        }
    }
    return true;
}

void HashTable::print() const
{
    std::cout << "size: " << size
              << "\nnumber of customers: " << customer_count << '\n';
    for(int i = 0; i < size; ++i) {
        std::cout << "custormers[" << i << "]\n";
        print_node(customers[i], 0);
    }
}

#endif //TEST