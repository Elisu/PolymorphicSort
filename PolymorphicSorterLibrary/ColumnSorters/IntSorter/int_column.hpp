#ifndef INT_COLUMN_HPP
#define INT_COLUMN_HPP

#include "../icolumn_sorter.hpp"
#include "../../Column/icolumn.hpp"

class Int_Column : public IColumn_Sorter
{
public:
	Int_Column();

	static int to_int(const std::string &s);

	void add_new(std::list<std::shared_ptr<IColumn>> &columns, std::vector<std::string> &values, int number) override;
};

#endif