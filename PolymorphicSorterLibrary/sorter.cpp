#include <list>

#include "sorter.hpp"

Sorter::Sorter(const Comparer &comparer) : cmp(comparer)
{
    for (int i = 0; i < cmp.columns.front()->row_count(); i++)
    {
        indexes.push_back(i);
    }
}

void Sorter::sort()
{
    indexes.sort(cmp);
}
