#ifndef ICOLUMN_SORTER_HPP
#define ICOLUMN_SORTER_HPP

#include <memory>
#include <string>
#include "../Column/icolumn.hpp"

class IColumn_Sorter
{
public:
	IColumn_Sorter(char op) : option(op) {}
	char option;
	bool match_option(char arg) { return option == arg; }
	virtual void add_new(std::list<std::shared_ptr<IColumn>> &columns, std::vector<std::string> &values, int number) = 0;
};

#endif