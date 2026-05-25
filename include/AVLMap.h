#pragma once
#include "AVLTree.h"
#include "IMap.h"

template<typename K, typename V>
class AVLMap : public IMap<K, V> {
private:
    AVLTree<K, V> tree;
public:
    V& operator[](const K& key) override {
        V* p = tree.find(key);
        if (!p) {
            tree.insert(key, V());
            p = tree.find(key);
        }
        return *p;
    }

    V* find(const K& key) override {
        return tree.find(key);
    }


    void print(std::ostream& out = std::cout) const override {
        tree.print(out);
    }
};