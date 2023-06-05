#include <string>
#include <list>
#include <memory>
#include <vector>
#include <typeinfo>

#include "mixed_column.hpp"
#include "../../Column/add_column.hpp"

Mixed_Column::Mixed_Column() : IColumn_Sorter('A') {}

int Mixed_Column::natural_sort(std::string s1, std::string s2)
{
    if (s1.compare(s2) == 0)
        return 0;

    if (s1.empty())
        return 1;

    if (s2.empty())
        return -1;

    // if char same type - we have to parse the part - otherwise just compare
    while (isdigit(s1[0]) == isdigit(s2[0]))
    {
        if (isdigit(s1[0]))
        {
            int val1 = find_integer_part(s1);
            int val2 = find_integer_part(s2);

            if (val1 < val2)
                return 1;

            if (val1 > val2)
                return -1;
        }
        else
        {
            std::string str1 = find_character_part(s1);
            std::string str2 = find_character_part(s2);

            int res = str2.compare(str1);
            if (res != 0)
                return res;
        }

        if (s1.empty())
            return 1;

        if (s2.empty())
            return -1;
    }

    return s2.compare(s1);
}

// Find number part in string
int Mixed_Column::find_integer_part(std::string &str)
{
    size_t start = str.find_first_of("0123456789");
    size_t last = str.find_first_not_of("0123456789");
    int res = std::stoi(str.substr(start, last - start));

    // Modifies given string to cutoff already reviewed part
    if (last < str.size())
        str = str.substr(last);
    else
        str = "";
    return res;
}

// Finds part without numbers - just chars
std::string Mixed_Column::find_character_part(std::string &str)
{
    size_t start = str.find_first_not_of("0123456789");
    size_t last = str.find_first_of("0123456789");
    std::string res = str.substr(start, last - start);
    if (last < str.size())
        str = str.substr(last);
    else
        str = "";
    return res;
}

void Mixed_Column::add_new(std::list<std::shared_ptr<IColumn>> &columns, std::vector<std::string> &values, int number)
{
    add_column<std::string>(columns, values, number, natural_sort);
}
