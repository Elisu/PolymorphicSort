#ifndef ICOMPARER_HPP
#define ICOMPARER_HPP

#include <list>
#include <memory>

#include "Column/icolumn.hpp"

class IComparer
{
public:
    std::list<std::shared_ptr<IColumn>> columns;

    virtual bool operator()(const int row1, const int row2) = 0;
};

#endif
