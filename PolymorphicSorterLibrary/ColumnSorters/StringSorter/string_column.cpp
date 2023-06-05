#include <list>
#include <typeinfo>

#include "string_column.hpp"
#include "../../Column/add_column.hpp"

String_Column::String_Column() : IColumn_Sorter('S') {}

void String_Column::add_new(std::list<std::shared_ptr<IColumn>> &columns, std::vector<std::string> &values, int number)
{
    add_column<std::string>(columns, values, number);
}
