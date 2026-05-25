#pragma once
#include<stack>
using namespace std;

template<typename K, typename V>
class AVLTree {
private:
    struct Node {
        K key;
        V value;
        Node* left;
        Node* right;
        Node* parent;
        int height;
        Node(const K& k, const V& v, Node* p = nullptr)
            : key(k), value(v), left(nullptr), right(nullptr), parent(p), height(1) {
        }
    };
    Node* root;

    int height(Node* n) const { return n ? n->height : 0; }
    void updateHeight(Node* n) { if (n) n->height = 1 + max(height(n->left), height(n->right)); }

    Node* balance(Node* n) {
        if (!n) return nullptr;
        updateHeight(n);
        int bf = height(n->left) - height(n->right);
        if (bf > 1) {
            if (height(n->left->left) < height(n->left->right))
                n->left = rotateLeft(n->left);
            return rotateRight(n);
        }
        if (bf < -1) {
            if (height(n->right->right) < height(n->right->left))
                n->right = rotateRight(n->right);
            return rotateLeft(n);
        }
        return n;
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        x->parent = y->parent;
        y->parent = x;
        if (T2) T2->parent = y;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        y->parent = x->parent;
        x->parent = y;
        if (T2) T2->parent = x;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

public:
    AVLTree() : root(nullptr) {}

    ~AVLTree() {
        if (!root) return;
        stack<Node*> st;
        Node* last = nullptr;
        st.push(root);
        while (!st.empty()) {
            Node* cur = st.top();
            if (cur->left && cur->left != last && (!cur->right || cur->right != last)) {
                st.push(cur->left);
            }
            else if (cur->right && cur->right != last) {
                st.push(cur->right);
            }
            else {
                st.pop();
                delete cur;
                last = cur;
            }
        }
    }

    void insert(const K& key, const V& value) {
        if (!root) {
            root = new Node(key, value);
            return;
        }
        Node* cur = root;
        Node* parent = nullptr;
        while (cur) {
            parent = cur;
            if (key < cur->key)
                cur = cur->left;
            else if (key > cur->key)
                cur = cur->right;
            else {
                cur->value = value;
                return;
            }
        }
        Node* newNode = new Node(key, value, parent);
        if (key < parent->key)
            parent->left = newNode;
        else
            parent->right = newNode;

        cur = newNode;
        while (cur) {
            Node* newSubRoot = balance(cur);
            if (newSubRoot != cur) {
                if (newSubRoot->parent) {
                    if (newSubRoot->parent->left == cur)
                        newSubRoot->parent->left = newSubRoot;
                    else
                        newSubRoot->parent->right = newSubRoot;
                }
                else {
                    root = newSubRoot;
                }
                cur = newSubRoot;
            }
            cur = cur->parent;
        }
    }

    V* find(const K& key) {
        Node* cur = root;
        while (cur) {
            if (key < cur->key)
                cur = cur->left;
            else if (key > cur->key)
                cur = cur->right;
            else
                return &cur->value;
        }
        return nullptr;
    }

    void print(std::ostream& out = std::cout) const {
        if (!root) {
            out << "empty\n";
            return;
        }
        stack<const Node*> st;
        const Node* cur = root;
        out << "Content:\n";
        while (!st.empty() || cur) {
            while (cur) {
                st.push(cur);
                cur = cur->left;
            }
            cur = st.top();
            st.pop();
            out << cur->key << " = " << cur->value << "\n";
            cur = cur->right;
        }
    }
};