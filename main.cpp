#include <iostream>

#include "Libs/Graph/Graph.h"
#include "Libs/Tools/BFS.h"
#include "Libs/GraphGenerator/Generator.h"
int main()
{
    Generator generator("same", "test.txt");
    generator.generateGraph();

    Tools::Solution_t<std::string> solution = generator.findPath("came");

    for (const auto &element : solution)
        std::cout << element->first << '\t';
    return 0;
}