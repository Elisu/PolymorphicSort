#ifndef FLOAT_COLUMN_HPP
#define FLOAT_COLUMN_HPP

#include "../icolumn_sorter.hpp"
#include "../../Column/icolumn.hpp"

class Float_Column : public IColumn_Sorter
{
public:
	Float_Column();

	static float to_float(const std::string &s);

	void add_new(std::list<std::shared_ptr<IColumn>> &columns, std::vector<std::string> &values, int number) override;
};

#endif