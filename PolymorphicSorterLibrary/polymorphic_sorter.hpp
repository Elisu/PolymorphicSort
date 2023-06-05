#ifndef POLYMORPHIC_SORTER
#define POLYMORPHIC_SORTER

#include <memory>
#include "Column/icolumn.hpp"
#include "ColumnSorters/icolumn_sorter.hpp"

class Polymorphic_Sorter
{

    std::list<std::unique_ptr<IColumn_Sorter>> sorters;

    void set_sorters();

    void print_sorted(
        bool to_file,
        std::vector<std::vector<std::string>> &text,
        char del,
        std::list<int> &indexes,
        std::string out_file);

    bool print_to(
        std::ostream &output,
        std::vector<std::vector<std::string>> &text,
        char del,
        std::list<int> &indexes);

    std::list<std::shared_ptr<IColumn>> get_cols(
        const std::vector<std::vector<std::string>> &text,
        const std::vector<unsigned long> &cols,
        const std::vector<char> &types);

    std::vector<std::vector<std::string>> read_rows(bool from_file, char del, std::string in_file);
    std::vector<std::vector<std::string>> read_from(std::istream &input, char del);
    std::vector<std::string> parse_row(std::string line, char deliminer);

public:
    Polymorphic_Sorter();
    void print_help();
    void sort(int argc, char **argv);
};

#endif