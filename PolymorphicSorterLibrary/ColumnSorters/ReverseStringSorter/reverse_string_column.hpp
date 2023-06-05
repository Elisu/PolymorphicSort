#ifndef REVERSE_STRING_COLUMN_HPP
#define REVERSE_STRING_COLUMN_HPP

#include "../icolumn_sorter.hpp"
#include "../../Column/icolumn.hpp"

class Reverse_String_Column : public IColumn_Sorter
{
public:
	Reverse_String_Column();

	static std::string reverse(std::string s);

	static int reversed_compare(const std::string &s1, const std::string &s2);

	void add_new(std::list<std::shared_ptr<IColumn>> &columns, std::vector<std::string> &values, int number) override;
};

#endif