#ifndef COMPLEX_COLUMN_HPP
#define COMPLEX_COLUMN_HPP

#include "../icolumn_sorter.hpp"
#include "../../Column/icolumn.hpp"

class Complex_Column : public IColumn_Sorter
{
public:
	Complex_Column();

	static int absolute_value_sort(const int &s1, const int &s2);

	void add_new(std::list<std::shared_ptr<IColumn>> &columns, std::vector<std::string> &values, int number) override;
};

#endif