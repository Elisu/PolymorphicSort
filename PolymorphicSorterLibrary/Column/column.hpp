#ifndef COLUMN_HPP
#define COLUMN_HPP

#include <vector>
#include <string>
#include <functional>
#include <iostream>

#include "icolumn.hpp"

template <typename T>
class Column : public IColumn
{
    std::vector<T> values;
    std::function<int(T, T)> compare_func;
    std::function<T(const std::string &)> convert;

    bool custom_compare;

    int basic_compare(const T &value1, const T &value2)
    {
        if (value1 < value2)
            return 1;
        else if (value1 > value2)
            return -1;
        else
            return 0;
    }

public:
    Column(std::function<int(T, T)> comp, std::function<T(const std::string &)> conv, int num) : 
        compare_func(comp), convert(conv), custom_compare(true), number(num) {}

    Column(std::function<T(const std::string &)> conv, int num) : convert(conv), number(num)
    {
        custom_compare = false;
    }

    int number;

    int compare(const int row1, const int row2) override
    {
        T value1 = values[row1];
        T value2 = values[row2];

        if (custom_compare)
            return compare_func(value1, value2);
        else
            return basic_compare(value1, value2);
    }

    int row_count() override
    {
        return values.size();
    }

    void add_value(const std::string &val)
    {
        try
        {
            values.push_back(convert(val));
        }
        catch (std::exception &e)
        {
            std::cerr << "error line: " << values.size() + 1 << " col: " << number << " bad" << std::endl;
            exit(0);
        }
    }
};

#endif