#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <string>
#include <typeinfo>
#include <memory>
#include <list>
#include <functional>
#include <algorithm>

/* POPIS
Implementace rozparsuje vstupni text do jednotlivych radek a pote sloupcu podle delimineru. Vybrané sloupce z argumentu
jsou pote reprezentovany strukturou Column, ktera uchovava vektor hodnot daneho typu. Sloupce samotne uchovavame ve vektoru pointeru
jeji nadrazene abstraktni tridy IColumn. Do Columnu lze pri incializaci nastavit custom convert a compare funkce.
Pro kazdy typ sloupce je priravena novy potomek struktury IColumn_Sorter, ktera narizuje implementovat metodu add_new.
Metoda add_new je pote overridovana v potomcich, kde kazdy vola parametrickou funcki add_column, podle jeji potreby - lze do ni specifikovat 
custom compare a convert funkci. Add_column pote vytvori novy sloupce dle pozadavku. Jsou dostupne 2 overloady.
Pro pridani tedy staci implementovat potomka IColumn_Sorter a pridat jej do globalniho listu sorteru v metode set_sorters. 
Samotny sort se deje ve strukture Sorter, ktera vola knihovni sort se zadanou instanci metody comparer. Sort vlastne tridi pouze
indexy radek podle compare funci danych sloupcu. - viz oprator() v Comparer strukture. Podle poradi setrizenych indexu pak 
vypisujeme radky do vysledku. Hodnoty v samotnych sloupcich se nepremistuji.

Funkcni parametry - S, R, N, I, A, F, C
*/

struct IColumn
{
    virtual int row_count() = 0;
    virtual int compare(const int row1, const int row2) = 0;
};

template <typename T>
struct Column : public IColumn
{
    Column(std::function<int(T, T)> comp, std::function<T(const std::string &)> conv, int num) : compare_func(comp), convert(conv), custom_compare(true), number(num) {}

    // Neni custom compare funkce, použije se basic_compare viz nize
    Column(std::function<T(const std::string &)> conv, int num) : convert(conv), number(num)
    {
        custom_compare = false;
    }

    int compare(const int row1, const int row2) override
    {
        T value1 = values[row1];
        T value2 = values[row2];

        if (custom_compare)
            return compare_func(value1, value2);
        else
            return basic_compare(value1, value2);
    }

    int row_count() override
    {
        return values.size();
    }

    void add_value(const std::string &val)
    {
        try
        {
            values.push_back(convert(val));
        }
        catch (std::exception &e)
        {
            std::cerr << "error line: " << values.size() + 1 << " col: " << number << " bad" << std::endl;
            exit(0);
        }
    }

private:
    std::vector<T> values;
    std::function<int(T, T)> compare_func;
    std::function<T(const std::string &)> convert;
    bool custom_compare;
    int basic_compare(const T &value1, const T &value2)
    {
        if (value1 < value2)
            return 1;
        else if (value1 > value2)
            return -1;
        else
            return 0;
    }

public:
    int number;
};

// Nasleduji 2 overloady fce ke spravne inicialialitaci sloupce
template <typename T>
void add_column(
    std::list<std::shared_ptr<IColumn>> &columns, std::vector<std::string> values, int number, std::function<int(T, T)> compare, std::function<T(const std::string &)> convert = [](T val) { return val; })
{
    Column<T> col = Column<T>(compare, convert, number);

    for (auto &&value : values)
    {
        col.add_value(value);
    }

    columns.emplace_back(std::make_shared<Column<T>>(col));
}

template <typename T>
void add_column(
    std::list<std::shared_ptr<IColumn>> &columns, std::vector<std::string> values, int number, std::function<T(const std::string &)> convert = [](T val) { return val; })
{
    Column<T> col = Column<T>(convert, number);

    for (auto &&value : values)
    {
        col.add_value(value);
    }

    columns.emplace_back(std::make_shared<Column<T>>(col));
}

// Zakladni interface pro implementaci vlozeni spravnych parametru do sloupce - compare a convert funkci
struct IColumn_Sorter
{
    IColumn_Sorter(char op) : option(op) {}
    char option;
    bool match_option(char arg) { return option == arg; }
    virtual void add_new(std::list<std::shared_ptr<IColumn>> &columns, std::vector<std::string> &values, int number) = 0;
};

struct Reverse_String_Column : public IColumn_Sorter
{
    Reverse_String_Column() : IColumn_Sorter('R') {}

    // Nejsem si jisti jestli je myšleno reversed string sort - jako porovnávání otočených stringů ("odzadu")-> reverse
    // nebo jako porovnávaní v opačném pořadí z - a - "ozadu abecedy"
    // první varianta je zakomentovaná pro případ, že druhá je špatně :)

    static std::string reverse(std::string s)
    {
        std::reverse(s.begin(), s.end());
        return s;
    }

    // void add_new(std::list<std::shared_ptr<IColumn>> &columns, std::vector<std::string> &values, int number) override
    // {
    //     add_column<std::string>(columns, values, number, reverse);
    // }

    static int reversed_compare(const std::string &s1, const std::string &s2)
    {
        return s1.compare(s2);
    }

    void add_new(std::list<std::shared_ptr<IColumn>> &columns, std::vector<std::string> &values, int number) override
    {
        add_column<std::string>(columns, values, number, reversed_compare);
    }
};

struct String_Column : public IColumn_Sorter
{
    String_Column() : IColumn_Sorter('S') {}

    void add_new(std::list<std::shared_ptr<IColumn>> &columns, std::vector<std::string> &values, int number) override
    {
        add_column<std::string>(columns, values, number);
    }
};

struct Int_Column : public IColumn_Sorter
{
    Int_Column() : IColumn_Sorter('N') {}

    static int to_int(const std::string &s)
    {
        return std::stoi(s);
    }

    void add_new(std::list<std::shared_ptr<IColumn>> &columns, std::vector<std::string> &values, int number) override
    {
        add_column<int>(columns, values, number, to_int);
    }
};

struct Ignore_Case_Column : public IColumn_Sorter
{
    Ignore_Case_Column() : IColumn_Sorter('I') {}

    static std::string to_lowercase(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

    void add_new(std::list<std::shared_ptr<IColumn>> &columns, std::vector<std::string> &values, int number) override
    {
        add_column<std::string>(columns, values, number, to_lowercase);
    }
};

struct Mixed_Column : public IColumn_Sorter
{
    Mixed_Column() : IColumn_Sorter('A') {}

    static int natural_sort(std::string s1, std::string s2)
    {
        if (s1.compare(s2) == 0)
            return 0;

        if (s1.empty())
            return 1;

        if (s2.empty())
            return -1;

        //if char same type - we have to parse the part - otherwise just compare
        while (isdigit(s1[0]) == isdigit(s2[0]))
        {
            if (isdigit(s1[0]))
            {
                int val1 = find_integer_part(s1);
                int val2 = find_integer_part(s2);

                if (val1 < val2)
                    return 1;

                if (val1 > val2)
                    return -1;
            }
            else
            {
                std::string str1 = find_character_part(s1);
                std::string str2 = find_character_part(s2);

                int res = str2.compare(str1);
                if (res != 0)
                    return res;
            }

            if (s1.empty())
                return 1;

            if (s2.empty())
                return -1;
        }

        return s2.compare(s1);
    }

    //Find number part in string
    static int find_integer_part(std::string &str)
    {
        size_t start = str.find_first_of("0123456789");
        size_t last = str.find_first_not_of("0123456789");
        int res = std::stoi(str.substr(start, last - start));

        //Modifies given string to cutoff already reviewed part
        if (last < str.size())
            str = str.substr(last);
        else
            str = "";
        return res;
    }

    //Finds part without numbers - just chars
    static std::string find_character_part(std::string &str)
    {
        size_t start = str.find_first_not_of("0123456789");
        size_t last = str.find_first_of("0123456789");
        std::string res = str.substr(start, last - start);
        if (last < str.size())
            str = str.substr(last);
        else
            str = "";
        return res;
    }

    void add_new(std::list<std::shared_ptr<IColumn>> &columns, std::vector<std::string> &values, int number) override
    {
        add_column<std::string>(columns, values, number, natural_sort);
    }
};

struct Float_Column : public IColumn_Sorter
{
    Float_Column() : IColumn_Sorter('F') {}

    static float to_float(const std::string &s)
    {
        return std::stof(s);
    }

    void add_new(std::list<std::shared_ptr<IColumn>> &columns, std::vector<std::string> &values, int number) override
    {
        add_column<float>(columns, values, number, to_float);
    }
};

struct Complex_Column : public IColumn_Sorter
{
    Complex_Column() : IColumn_Sorter('C') {}

    static int absolute_value_sort(const int &s1, const int &s2)
    {
        if (abs(s1) < abs(s2))
            return 1;

        if (abs(s1) > abs(s2))
            return -1;

        return 0;
    }

    void add_new(std::list<std::shared_ptr<IColumn>> &columns, std::vector<std::string> &values, int number) override
    {
        add_column<int>(columns, values, number, absolute_value_sort, Int_Column::to_int);
    }
};

struct IComparer
{
    std::list<std::shared_ptr<IColumn>> columns;

    virtual bool operator()(const int row1, const int row2) = 0;
};

struct Comparer : public IComparer
{

    explicit Comparer(std::list<std::shared_ptr<IColumn>> &col)
    {
        columns = std::move(col);
    }

    bool operator()(const int row1, const int row2) override
    {
        //We compare column values on given indexes, if they are equeal we use the next column and so on, if not we return result
        for (auto &column : columns)
        {
            int res = column->compare(row1, row2);
            if (res < 0)
                return false;
            if (res > 0)
                return true;
        }

        // To make it stable
        if (row1 < row2)
            return true;
        else
            return false;
    }
};

struct Sorter
{
    std::list<int> indexes;
    Comparer cmp;

    explicit Sorter(const Comparer &comparer) : cmp(comparer)
    {
        // Jen cisla od 0 do n-1 radek jako indexy ve sloupci, ktere pak tridime
        for (int i = 0; i < cmp.columns.front()->row_count(); i++)
        {
            indexes.push_back(i);
        }
    }

    void sort()
    {
        indexes.sort(cmp);
    }
};

std::vector<std::string> parse_row(std::string line, char deliminer)
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

std::vector<std::vector<std::string>> read_rows(std::istream &input, char del)
{
    std::vector<std::vector<std::string>> text;
    std::string line;

    while (std::getline(input, line, '\n'))
    {
        std::vector<std::string> row = parse_row(line, del);

        // If only one empty column and it is end of input -> endl at the end, which is allowed, but skipped
        if (!(row.size() == 1 && row[0].compare("") == 0) || !input.eof())
        {
            // Otherwise we check if number of columns equal number of columns in first row
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

bool print_sorted(std::ostream &output, std::vector<std::vector<std::string>> &text, char del, std::list<int> &indexes)
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

std::list<std::unique_ptr<IColumn_Sorter>> sorters;

// Inicializuje globalni list dostupnych sorteru
void set_sorters()
{
    sorters.emplace_back(std::make_unique<String_Column>(String_Column()));
    sorters.emplace_back(std::make_unique<Reverse_String_Column>(Reverse_String_Column()));
    sorters.emplace_back(std::make_unique<Int_Column>(Int_Column()));
    sorters.emplace_back(std::make_unique<Ignore_Case_Column>(Ignore_Case_Column()));
    sorters.emplace_back(std::make_unique<Mixed_Column>(Mixed_Column()));
    sorters.emplace_back(std::make_unique<Float_Column>(Float_Column()));
    sorters.emplace_back(std::make_unique<Complex_Column>(Complex_Column()));
}

// Funkce vrati potrebne sloupce podle argumentu
std::list<std::shared_ptr<IColumn>> get_cols(const std::vector<std::vector<std::string>> &text, const std::vector<unsigned long> &cols, const std::vector<char> &types)
{
    std::list<std::shared_ptr<IColumn>> columns;
    for (size_t i = 0; i < cols.size(); i++)
    {
        // Cislo sloupce vyssi nez pocet sloupcu
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

        // Kdyz yadane pismeno nepatri zadnemu sorteru
        if (!matched)
        {
            std::cerr << "Not suppoerted sorting type argument " << types[i] << std::endl;
            exit(0);
        }
    }
    return columns;
}

void print_help()
{
    std::cout << "usage: polysort [-i in] [-o out] [-s separator] { type colnum }" << std::endl;
    std::cout << "-i: input file (default: stdin)" << std::endl;
    std::cout << "-o: output file (default: stdout)" << std::endl;
    std::cout << "-s: separator (default: ' ')" << std::endl;
    std::cout << "type: S - string, N - numeric, R - reversed string sort, I - ignore case string" << std::endl;
    std::cout << "      A - natural sort, F - floats, C - complex numbers" << std::endl;
    std::cout << "column: the number of logical column (starting from 1)" << std::endl;
}

int main(int argc, char **argv)
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
            return 0;
        }
    }

    //Parsovani parametru sortovacich sloupcu
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

    // Kdyz zadny parametr sloupce
    if (types.size() == 0)
    {
        std::cerr << "Sorting column not specified" << std::endl;
        exit(0);
    }

    std::vector<std::vector<std::string>> text;

    // cteni ze souboru/cinu
    if (from_file)
    {
        std::ifstream ifs;
        ifs.open(in_file, std::ifstream::in);
        if (ifs.is_open())
        {
            text = read_rows(ifs, deliminer);
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
        text = read_rows(std::cin, deliminer);
    }

    // Ziskani sloupcu a samotne sorteni
    std::list<std::shared_ptr<IColumn>> columns = get_cols(text, col_numbers, types);
    Comparer cmp = Comparer(columns);
    Sorter sorter = Sorter(cmp);
    sorter.sort();

    // psani do souboru/coutu
    if (to_file)
    {
        std::ofstream ofs;
        ofs.open(out_file, std::ofstream::out);
        if (ofs.is_open())
        {
            print_sorted(ofs, text, deliminer, sorter.indexes);
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
        print_sorted(std::cout, text, deliminer, sorter.indexes);
    }

    return 0;
}