#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include "utilesWet2.h"

class HashTable {
    private:
    static const int FACTOR = 2;
    struct Customer {
        int id;
        int phone;
        bool member;
        Customer(int c_id, int phone);
    };
    struct Node {
        Customer customer;
        Node *left;
        Node *right;
        int height;
        Node(int c_id, int phone);
    };
    int hash(int c_id) const;
    void expand();
    Node *insert(Node *root, Node *node);
    Node *find(Node *node, int c_id) const;
    Node *rebalance(Node *node);
    Node *ll_roll(Node *node);
    Node *lr_roll(Node *node);
    Node *rr_roll(Node *node);
    Node *rl_roll(Node *node);
    void set_height(Node *node);
    int get_height(Node *node);
    int get_balance(Node *node);
    void clear(Node *node);
    void move_over(Node *node);

    int size;
    int customer_count;
    Node **customers;
    Node *latest_member;

    public:
    HashTable();
    ~HashTable();
    StatusType add_customer(int c_id, int phone);
    Output_t<int> get_phone(int c_id) const;
    StatusType make_member(int c_id);
    Output_t<bool> is_member(int c_id) const;
    void unmember_latest();
};

#endif //HASH_TABLE_H