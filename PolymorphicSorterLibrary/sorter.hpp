#ifndef SORTER_HPP
#define SORTER_HPP

#include "comparer.hpp"

class Sorter
{
public:
    std::list<int> indexes;
    Comparer cmp;

    explicit Sorter(const Comparer &comparer);
    void sort();
};

#endif