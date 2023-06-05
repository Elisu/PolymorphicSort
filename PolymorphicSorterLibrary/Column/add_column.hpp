#ifndef ADD_COLUMN_HPP
#define ADD_COLUMN_HPP

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <list>

#include "icolumn.hpp"
#include "column.hpp"

template <typename T>
inline void add_column(
    std::list<std::shared_ptr<IColumn>> &columns,
    std::vector<std::string> values,
    int number,
    std::function<int(T, T)> compare,
    std::function<T(const std::string &)> convert = [](T val)
    { return val; })
{
    Column<T> col = Column<T>(compare, convert, number);

    for (auto &&value : values)
    {
        col.add_value(value);
    }

    columns.emplace_back(std::make_shared<Column<T>>(col));
};

template <typename T>
inline void add_column(
    std::list<std::shared_ptr<IColumn>> &columns,
    std::vector<std::string> values,
    int number,
    std::function<T(const std::string &)> convert = [](T val)
    { return val; })
{
    Column<T> col = Column<T>(convert, number);

    for (auto &&value : values)
    {
        col.add_value(value);
    }

    columns.emplace_back(std::make_shared<Column<T>>(col));
};

#endif