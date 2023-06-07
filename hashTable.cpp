#include "hashTable.h"
#include <new>

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

HashTable::HashTable() :
    size(0),
    customer_count(0),
    customers(nullptr)
{}

HashTable::~HashTable()
{
    for(int i = 0; i < size; ++i) {
        clear(customers[i]);
    }
}

void HashTable::clear(Node *node)
{
    clear(node->left);
    clear(node->right);
    delete node;
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

int HashTable::hash(int c_id) const
{
    return c_id % size;
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
    return SUCCESS;
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
    size *= FACTOR;
    Node **old_customers = customers;
    customers = new Node*[size];
    for(int i = 0; i < size; ++i) {
        customers[i] = nullptr;
    }
    for(int i = 0; i < old_size; ++i) {
        move_over(old_customers[i]);
    }
    delete [] old_customers;
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
    node->right = 0;
    int h = hash(node->customer.id);
    customers[h] = insert(customers[h], node);
}

StatusType HashTable::add_customer(int c_id, int phone)
{
    if(c_id < 0 || phone < 0) {
        return INVALID_INPUT;
    }
    Node *node = find(customers[hash(c_id)], c_id);
    if (node != nullptr) {
        return ALREADY_EXISTS;
    }
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
    return SUCCESS;
}

HashTable::Node *HashTable::insert(Node *root, Node *node)
{
    if (root == nullptr) {
        return node;
    }
    if (node->customer.id < root->customer.id) {
        node->left = insert(node->left, node);
    } else {
        node->right = insert(root->right, node);
    }
    return rebalance(root);
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

int HashTable::get_height(Node *node)
{
    return (node == nullptr) ? -1 : node->height;
}

int HashTable::get_balance(Node *node)
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