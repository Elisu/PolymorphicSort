#include <list>

#include "comparer.hpp"

Comparer::Comparer(std::list<std::shared_ptr<IColumn>> &col)
{
    columns = std::move(col);
}

bool Comparer::operator()(const int row1, const int row2)
{
    // We compare column values on given indexes, if they are equeal we use
    // the next column and so on, if not we return result
    for (auto &column : columns)
    {
        int res = column->compare(row1, row2);
        if (res < 0)
            return false;
        if (res > 0)
            return true;
    }

    // To make it stable
    if (row1 < row2)
        return true;
    else
        return false;
}
