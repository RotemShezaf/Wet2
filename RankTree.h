#ifndef AVL_RankTree_
#define AVL_RankTree_

#include "utilesWet2.h"

#define HEIGHT(nodeptr) ((nodeptr) ? ((nodeptr)->_height) : -1)

class KeyNotFound : public std::runtime_error {
public:
    KeyNotFound() :
        std::runtime_error("key not found")
    {}
};

class KeyAlreadyExists : public std::runtime_error {
public:
    KeyAlreadyExists() :
        std::runtime_error("key already exists")
    {}
};

template<class Key, class Value>
struct RankTree {
    struct Node {
        Key _key;
        Value _value;
        int _height;
        Node *_left;
        Node *_right;
        int _rank = 0;

        Node(const Key &key, const Value &value, int rank) :
            _key(key),
            _value(value),
            _height(0),
            _left(nullptr),
            _right(nullptr),
            _rank(rank)
        {}

        static void clear(Node *node)
        {
            if (node == nullptr) {
                return;
            }
            clear(node->_left);
            clear(node->_right);
            delete node;
        }

        static Node *find(Node *node, const Key &key)
        {
            if (!node) {
                throw KeyNotFound();
            }
            if (key < node->_key) {
                return find(node->_left, key);
            }
            if (node->_key < key) {
                return find(node->_right, key);
            }
            return node;
        }

        static int get_rank(Node* node, const Key& key)
        {
            if (!node) {
                throw KeyNotFound();
            }
            if (key < node->_key) {
                return get_rank(node->_left, key) + node->_rank;
            }
            if (node->_key < key) {
                return get_rank(node->_right, key) + node->_rank;
            }
            return node->_rank;
        }

        static void keys_inorder(const Node *node, Key **keys)
        {
            if (!node) {
                return;
            }
            keys_inorder(node->_right, keys);
            *((*keys)++) = node->_key;
            keys_inorder(node->_left, keys);
        }

        // returns the new root of the subRankTree.
        // may throw KeyAlreadyExists.
        static Node *insert(Node *node, const Key &key, const Value &value, int rank)
        {
            if (!node) {
                return new Node(key, value, rank);
            }

            if (key < node->_key) {
                node->_left = insert(node->_left, key, value, rank - node->_rank );
                return node ->rebalance();
            }
            if (node->_key < key) {
                node->_right = insert(node->_right, key, value, rank - node->_rank);
                return node ->rebalance();
            }
            throw KeyAlreadyExists();
        }

        // returns the new root of the subRankTree.
        // may throw KeyNotFound.
        static Node *remove(Node *node, const Key &key)
        {
            if (!node) {
                throw KeyNotFound();
            }
            if (key < node->_key) {
                node->_left = remove(node->_left, key);
                return node->rebalance();
            }
            if (node->_key < key) {
                node->_right = remove(node->_right, key);
                return node->rebalance();
            }
            // this is the node to delete.
            if (!node->_left && !node->_right) {
                delete node;
                return nullptr;
            }
            if (node->_left && !node->_right) {
                Node *root = node->_left;
                delete node;
                return root;
            }
            if (!node->_left && node->_right) {
                Node *root = node->_right;
                delete node;
                return root;
            }
            // both children exist
            Node * next;
            node->_right = node->_right->extract_next(&next);
            next->_left = node->_left;
            next->_right = node->_right;
            delete node;
            return next->rebalance();
        }

    private:

        void set_height()
        {
            int l_height = HEIGHT(_left), r_height = HEIGHT(_right);
            int max = l_height > r_height ? l_height : r_height;
            _height = max + 1;
        }

        // returns the new root of the subRankTree.
        Node *l_l_roll()
        {
            Node * root = _left;
            if (_left->_right != nullptr) {
                _left->_right += _left->_rank;
            }
            
            _left->_rank += this->_rank;
            this->_rank -= root->_rank;

            _left = _left->_right;
            root->_right = this;
            set_height();
            root->set_height();
            return root;
        }

        // returns the new root of the subRankTree.
        Node *l_r_roll()
        {
            _left = _left->r_r_roll();
            return l_l_roll();

            //Node *temp = _left, *root = _left->_right;
            //_left = root->_right;
            //temp->_right = root->_left;
            //root->_left = temp;
            //root->_right = this;
            //set_height();
            //temp->set_height();
            //root->set_height();
            //return root;
        }

        // returns the new root of the subRankTree.
        Node *r_r_roll()
        {
            Node *root = _right;
            

            if (_right->_left != nullptr) {
                _right->_left->_rank += _right->_rank;
            }
            _right->_rank += this->_rank;
            this->_rank -= root->_rank;   
            _right = _right->_left;
            root->_left = this;
            set_height();
            root->set_height();
            return root;
        }

        // returns the new root of the subRankTree.
        Node *r_l_roll()
        {
            _right = _right->l_l_roll();
            return r_r_roll();
           // Node *temp = _right, *root = _right->_left;
           // _right = root->_left;
           // temp->_left = root->_right;
           // root->_right = temp;
           // root->_left = this;
            //set_height();
           // temp->set_height();
           // root->set_height();
           // return root;
        }

        int get_balance() const
        {
            return HEIGHT(_left) - HEIGHT(_right);
        }

        // returns the new root of the subRankTree.
        Node *rebalance()
        {
            int balance = get_balance();
            if (balance == 2) {
                if (_left->get_balance() == -1) {
                    return l_r_roll();
                }
                return l_l_roll();
            }
            if (balance == -2) {
                if (_right->get_balance() == 1) {
                    return r_l_roll();
                }
                return r_r_roll();
            }
            set_height();
            return this;
        }

        // next (output) is set by the function to point to the first node lexicoraphically in the subRankTree.
        // that node is extracted from the subRankTree.
        // returns the new root of the subRankTree.
        Node *extract_next(Node **next)
        {
            if (!_left) {
                *next = this;
                return _right;
            }
            _left = _left->extract_next(next);
            return rebalance();
        }
    };


    RankTree() :
        _root(nullptr),
        _first(nullptr),
        _size(0)
    {}

    // may throw KeyNotFound.
    Value& find(const Key& key)
    {
        return Node::find(_root, key)->_value;
    }

    int get_rank(const Key & key)
    {
        return Node::get_rank(_root, key);
    }

    void add_rank(const Key& key, int amount)
    {
        Node * node = _root;
        bool is_right_sequence = false;
        while (node != nullptr) {
            if (key < node->_key) {
                if (is_right_sequence) {
                    node->_rank -= amount;
                    is_right_sequence = false;
                }
                node = node->_left;
            }
            else if (node->_key < key) {
                if (!is_right_sequence) {
                    node->_rank += amount;
                    is_right_sequence = true;
                }
                node = node->_right;
            }
            else if (node->_key == key) {
                if (is_right_sequence) {
                    node->_rank -= amount;
                }
                if (node->_left)
                    node->_left += amount;
                return;
            }
        }

        return ;

    }

    bool is_empty() const
    {
        return !_root;
    }

    int size() const
    {
        return _size;
    }

    const Value& get_first() const
    {
        if(!_first) {
            throw KeyNotFound();
        }
        return _first->_value;
    }

    // may throw KeyAlreadyExists.
    void insert(const Key &key, const Value &value)
    {
        _root = Node::insert(_root, key, value, 0);
        _size++;
        set_first();
    }

    void get_keys_array( Key *const keys_out) const
    {
        Key *keys = keys_out;
        Node::keys_inorder(_root, &keys);
    }

    // may throw KeyNotFound.
    void remove(const Key &key)
    {
        _root = Node::remove(_root, key);
        _size--;
        set_first();
    }

    ~RankTree()
    {
        Node::clear(_root);
    }

    Node *root() const
    {
        return _root;
    }

    private:
        Node* _root;
        Node* _first;
        int _size;

        void set_first()
        {
            if (!_root) {
                _first = nullptr;
                return;
            }
            for (_first = _root; _first->_right; _first = _first->_right);
        }
};

#endif // AVL_RankTree_