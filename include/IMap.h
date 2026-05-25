#pragma once


template<typename K, typename V>
class IMap {
public:
    virtual ~IMap() {}
    virtual V& operator[](const K& key) = 0;
    virtual V* find(const K& key) = 0;
    virtual void print(std::ostream& out = std::cout) const = 0;
};