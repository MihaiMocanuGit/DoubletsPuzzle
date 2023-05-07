#include <iostream>

#include "Libs/Graph/Graph.h"
#include "Libs/Tools/BFS.h"
#include "Libs/GraphGenerator/Generator.h"
int main()
{
    Generator generator("same", "words_alpha.txt");
    generator.generateGraph();

    Tools::Solution_t<std::string> solution = generator.findPath("case");

    for (const auto &element : solution)
        std::cout << element->first << '\t';
    return 0;
}