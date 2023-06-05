#include <string>
#include <list>
#include <memory>
#include <vector>
#include <typeinfo>
#include <algorithm>

#include "reverse_string_column.hpp"
#include "../../Column/add_column.hpp"

Reverse_String_Column::Reverse_String_Column() : IColumn_Sorter('R') {}

std::string Reverse_String_Column::reverse(std::string s)
{
    std::reverse(s.begin(), s.end());
    return s;
}

int Reverse_String_Column::reversed_compare(const std::string &s1, const std::string &s2)
{
    return s1.compare(s2);
}

void Reverse_String_Column::add_new(
    std::list<std::shared_ptr<IColumn>> &columns,
    std::vector<std::string> &values,
    int number)
{
    add_column<std::string>(columns, values, number, reversed_compare);
}
