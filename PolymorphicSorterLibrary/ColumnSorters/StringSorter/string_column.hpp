#ifndef STRING_COLUMN_HPP
#define STRING_COLUMN_HPP

#include <memory>
#include <vector>
#include <string>

#include "../icolumn_sorter.hpp"
#include "../../Column/icolumn.hpp"

class String_Column : public IColumn_Sorter
{
public:
	String_Column();

	void add_new(
        std::list<std::shared_ptr<IColumn>> &columns, 
        std::vector<std::string> &values, 
        int number) override;
};

#endif