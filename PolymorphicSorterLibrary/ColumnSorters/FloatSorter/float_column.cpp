#include <string>
#include <list>
#include <memory>
#include <vector>

#include "float_column.hpp"
#include "../../Column/add_column.hpp"

Float_Column::Float_Column() : IColumn_Sorter('F') {}

float Float_Column::to_float(const std::string &s)
{
    return std::stof(s);
}

void Float_Column::add_new(std::list<std::shared_ptr<IColumn>> &columns, std::vector<std::string> &values, int number)
{
    add_column<float>(columns, values, number, to_float);
}
