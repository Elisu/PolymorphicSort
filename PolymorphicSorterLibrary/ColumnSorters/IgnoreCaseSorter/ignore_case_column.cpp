#include <string>
#include <algorithm>
#include <list>
#include <memory>
#include <vector>

#include "ignore_case_column.hpp"
#include "../../Column/add_column.hpp"

Ignore_Case_Column::Ignore_Case_Column() : IColumn_Sorter('I') {}

std::string Ignore_Case_Column::to_lowercase(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

void Ignore_Case_Column::add_new(std::list<std::shared_ptr<IColumn>> &columns, std::vector<std::string> &values, int number)
{
    add_column<std::string>(columns, values, number, to_lowercase);
}
