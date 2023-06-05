#include <unistd.h>
#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "polymorphic_sorter.hpp"
#include "sorter.hpp"
#include "comparer.hpp"
#include "ColumnSorters/IgnoreCaseSorter/ignore_case_column.hpp"
#include "ColumnSorters/ComplexSorter/complex_column.hpp"
#include "ColumnSorters/FloatSorter/float_column.hpp"
#include "ColumnSorters/IntSorter/int_column.hpp"
#include "ColumnSorters/MixedSorter/mixed_column.hpp"
#include "ColumnSorters/ReverseStringSorter/reverse_string_column.hpp"
#include "ColumnSorters/StringSorter/string_column.hpp"

// Initializes a list of available sorters
void Polymorphic_Sorter::set_sorters()
{
    sorters.emplace_back(std::make_unique<String_Column>(String_Column()));
    sorters.emplace_back(std::make_unique<Reverse_String_Column>(Reverse_String_Column()));
    sorters.emplace_back(std::make_unique<Int_Column>(Int_Column()));
    sorters.emplace_back(std::make_unique<Ignore_Case_Column>(Ignore_Case_Column()));
    sorters.emplace_back(std::make_unique<Mixed_Column>(Mixed_Column()));
    sorters.emplace_back(std::make_unique<Float_Column>(Float_Column()));
    sorters.emplace_back(std::make_unique<Complex_Column>(Complex_Column()));
}

void Polymorphic_Sorter::print_sorted(bool to_file, std::vector<std::vector<std::string>> &text, char del, 
    std::list<int> &indexes, std::string out_file)
{
    // Write to file, else cout
    if (to_file)
    {
        std::ofstream ofs;
        ofs.open(out_file, std::ofstream::out);
        if (ofs.is_open())
        {
            print_to(ofs, text, del, indexes);
            ofs.close();
        }
        else
        {
            std::cerr << "Output File Error" << std::endl;
            exit(0);
        }
    }
    else
    {
        print_to(std::cout, text, del, indexes);
    }
}

bool Polymorphic_Sorter::print_to(std::ostream &output, std::vector<std::vector<std::string>> &text, char del, std::list<int> &indexes)
{
    for (size_t i = 0; i < text.size(); i++)
    {
        for (size_t j = 0; j < text[i].size(); j++)
        {
            output << text[indexes.front()][j];

            if (j != text[i].size() - 1)
                output << del;
        }

        output << std::endl;
        indexes.pop_front();
    }
    return true;
}

// Returns requested columns based on function arguments
std::list<std::shared_ptr<IColumn>> Polymorphic_Sorter::get_cols(const std::vector<std::vector<std::string>> &text, const std::vector<unsigned long> &cols, const std::vector<char> &types)
{
    std::list<std::shared_ptr<IColumn>> columns;
    for (size_t i = 0; i < cols.size(); i++)
    {
        if (cols[i] > text[0].size())
        {
            std::cerr << "Column does not exist" << std::endl;
            exit(0);
        }

        int number = cols[i] - 1;

        std::vector<std::string> str_values;

        for (size_t j = 0; j < text.size(); j++)
        {
            str_values.push_back(text[j][number]);
        }

        bool matched = false;

        for (auto &&sorter : sorters)
        {
            if (sorter->match_option(types[i]))
            {
                sorter->add_new(columns, str_values, number);
                matched = true;
                break;
            }
        }

        if (!matched)
        {
            std::cerr << "Not supported sorting type argument " << types[i] << std::endl;
            exit(0);
        }
    }
    return columns;
}

std::vector<std::vector<std::string>> Polymorphic_Sorter::read_rows(bool from_file, char del, std::string in_file)
{
    std::vector<std::vector<std::string>> text;

    // Read from file, else cin
    if (from_file)
    {
        std::ifstream ifs;
        ifs.open(in_file, std::ifstream::in);
        if (ifs.is_open())
        {
            text = read_from(ifs, del);
            ifs.close();
        }
        else
        {
            std::cerr << "Input File Error" << std::endl;
            exit(0);
        }
    }
    else
    {
        text = read_from(std::cin, del);
    }

    return text;
}


std::vector<std::vector<std::string>> Polymorphic_Sorter::read_from(std::istream &input, char del)
{
    std::vector<std::vector<std::string>> text;
    std::string line;

    while (std::getline(input, line, '\n'))
    {
        std::vector<std::string> row = parse_row(line, del);

        // If only one empty column and it is end of input -> endl at the end, which is allowed, but skipped
        if (!(row.size() == 1 && row[0].compare("") == 0) || !input.eof())
        {
            // Otherwise we check if number of columns is equal to number of columns in first row
            if (text.size() > 0 && row.size() != text.front().size())
            {
                std::cerr << "error: row " << text.size() + 1 << " has missing columns" << std::endl;
                exit(0);
            }

            text.push_back(row);
        }
    }

    return text;
}

std::vector<std::string> Polymorphic_Sorter::parse_row(std::string line, char deliminer)
{
    std::string word;
    std::stringstream input(line);
    std::vector<std::string> row;

    while (!input.eof())
    {
        std::getline(input, word, deliminer);
        row.push_back(word);
    }

    return row;
}

Polymorphic_Sorter::Polymorphic_Sorter()
{
    set_sorters();
}

void Polymorphic_Sorter::print_help()
{
    std::cout << "usage: polysort [-i in] [-o out] [-s separator] { type colnum }" << std::endl;
    std::cout << "-i: input file (default: stdin)" << std::endl;
    std::cout << "-o: output file (default: stdout)" << std::endl;
    std::cout << "-s: separator (default: ' ')" << std::endl;
    std::cout << "type: S - string, N - numeric, R - reversed string sort, I - ignore case string" << std::endl;
    std::cout << "      A - natural sort, F - floats, C - complex numbers" << std::endl;
    std::cout << "column: the number of logical column (starting from 1)" << std::endl;
}

void Polymorphic_Sorter::sort(int argc, char **argv)
{
    int opt;
    char deliminer = ' ';
    std::string in_file;
    std::string out_file;
    bool from_file = false;
    bool to_file = false;

    std::vector<unsigned long> col_numbers;
    std::vector<char> types;

    set_sorters();

    while ((opt = getopt(argc, argv, "i:o:s:h")) != -1)
    {
        switch (opt)
        {
        case 'i':
            in_file = optarg;
            from_file = true;
            break;
        case 'o':
            out_file = optarg;
            to_file = true;
            break;
        case 's':
            deliminer = optarg[0];
            break;
        case 'h':
            print_help();
            return;
        }
    }

    // Parsing parameters for sorted columns
    for (int i = optind; i < argc; i++)
    {
        std::string s = argv[i];
        types.push_back(s[0]);
        try
        {
            col_numbers.push_back(stoul(s.substr(1)));
        }
        catch (std::exception &e)
        {
            std::cerr << "Wrong column number in arg" << std::endl;
            exit(0);
        }
    }

    if (types.size() == 0)
    {
        std::cerr << "Sorting column not specified" << std::endl;
        exit(0);
    }

    std::vector<std::vector<std::string>> text = read_rows(from_file, deliminer, in_file);

    // Getting columns and sorting
    std::list<std::shared_ptr<IColumn>> columns = get_cols(text, col_numbers, types);
    Comparer cmp = Comparer(columns);
    Sorter sorter = Sorter(cmp);
    sorter.sort();

    print_sorted(to_file, text, deliminer, sorter.indexes, out_file);
}
