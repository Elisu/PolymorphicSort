# PolymorphicSort
Polymorphic sort in written C++</br>

Implementation of a polymorphic sorter of a column-containing text input.

The program behaves kindof like the unix sort, i.e. reads either from a file specified on the command line, or from the standard input; similarly for the output.

The input lines are separated into logical columns with some (specifiable) single-character separator. The program allows to sort the lines based on various chosen priorities and types of the columns.
 
 usage: polysort [-i in] [-o out] [-s separator] { type colnum } </br>
    -i: input file (default: stdin)</br>
    -o: output file (default: stdout)</br>
    -s: separator (default: ' ')</br>
    type: S - string, N - numeric, R (reversed ("backwards") string sort), I (ignore-case string sort), A (nAtural sort of numbers mixed with text, as in sort -V), F (floating-point number), C (complex number, ordered by absolute value)</br> 
    column: the number of logical column (starting from 1)</br>

# Details
Order the input rows by the specified columns, with decreasing priority: The second comparison rule gets evaluated only if the first comparison rule compared the rows as equal.
   - The columns are numbered from 1
   - Different columns may contain different data "types" 
   - If some columns or rows do not match the specification, print out an error and terminate.
   
Internally, represent the data in the specified format (i.e., with numeric ordering, the program should not convert the strings to integers everytime it needs to compare them)