#ifndef COMPARER_HPP
#define COMPARER_HPP

#include "icomparer.hpp"

class Comparer : public IComparer
{
public:
    explicit Comparer(std::list<std::shared_ptr<IColumn>> &col);
    bool operator()(const int row1, const int row2) override;
};

#endif