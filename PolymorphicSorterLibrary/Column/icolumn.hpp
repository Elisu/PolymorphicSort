#ifndef ICOLUMN_HPP
#define ICOLUMN_HPP

class IColumn
{
public:
    virtual int row_count() = 0;
    virtual int compare(const int row1, const int row2) = 0;
};

#endif