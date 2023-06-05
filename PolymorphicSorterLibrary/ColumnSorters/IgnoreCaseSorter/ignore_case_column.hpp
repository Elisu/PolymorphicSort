#ifndef IGNORE_CASE_COLUMN_HPP
#define IGNORE_CASE_COLUMN_HPP

#include "../icolumn_sorter.hpp"
#include "../../Column/icolumn.hpp"

class Ignore_Case_Column : public IColumn_Sorter
{
public:
	Ignore_Case_Column();

	static std::string to_lowercase(std::string s);

	void add_new(std::list<std::shared_ptr<IColumn>> &columns, std::vector<std::string> &values, int number) override;
};

#endif