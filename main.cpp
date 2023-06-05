#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <typeinfo>
#include <memory>
#include <list>
#include <functional>
#include <algorithm>

#include "PolymorphicSorterLibrary/polymorphic_sorter.hpp"

/* DESCRIPTION
usage: polysort [-i in] [-o out] [-s separator] { type colnum } 
    -i: input file (default: stdin)
    -o: output file (default: stdout)
    -s: separator (default: ' ')
    type: S - string, N - numeric, ...
    column: the number of logical column (starting from 1)

type: S - string, N - numeric, R - reversed string sort, I - ignore case string
      A - natural sort, F - floats, C - complex numbers
column: the number of logical column (starting from 1)
*/

int main(int argc, char **argv)
{
    Polymorphic_Sorter sorter = Polymorphic_Sorter();
    sorter.sort(argc, argv);

    return 0;
}