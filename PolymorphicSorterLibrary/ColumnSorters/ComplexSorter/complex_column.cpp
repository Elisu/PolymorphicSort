#include <list>
#include <memory>
#include <vector>
#include <string>

#include "complex_column.hpp"
#include "../../Column/add_column.hpp"
#include "../IntSorter/int_column.hpp"

Complex_Column::Complex_Column() : IColumn_Sorter('C') {}

int Complex_Column::absolute_value_sort(const int &s1, const int &s2)
{
    if (abs(s1) < abs(s2))
        return 1;

    if (abs(s1) > abs(s2))
        return -1;

    return 0;
}

void Complex_Column::add_new(
    std::list<std::shared_ptr<IColumn>> &columns,
    std::vector<std::string> &values,
    int number)
{
    add_column<int>(columns, values, number, absolute_value_sort, Int_Column::to_int);
}
