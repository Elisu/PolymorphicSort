#include <string>
#include <list>
#include <memory>
#include <vector>
#include <typeinfo>

#include "int_column.hpp"
#include "../../Column/add_column.hpp"

Int_Column::Int_Column() : IColumn_Sorter('N') {}

int Int_Column::to_int(const std::string &s)
{
    return std::stoi(s);
}

void Int_Column::add_new(std::list<std::shared_ptr<IColumn>> &columns, std::vector<std::string> &values, int number)
{
    add_column<int>(columns, values, number, to_int);
}
