#ifndef MIXED_COLUMN_HPP
#define MIXED_COLUMN_HPP

#include "../icolumn_sorter.hpp"
#include "../../Column/icolumn.hpp"

class Mixed_Column : public IColumn_Sorter
{
public:
	Mixed_Column();

	static int natural_sort(std::string s1, std::string s2);

	// Find number part in string
	static int find_integer_part(std::string &str);

	// Finds part without numbers - just chars
	static std::string find_character_part(std::string &str);

	void add_new(std::list<std::shared_ptr<IColumn>> &columns, std::vector<std::string> &values, int number) override;
};

#endif