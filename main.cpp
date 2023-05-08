#include <iostream>

#include "Libs/Graph/Graph.h"
#include "Libs/Tools/BFS.h"
#include "Libs/GraphGenerator/Generator.h"
int main()
{
    Generator generator("words_alpha.txt");
    generator.generateGraph(5);

    //https://lewiscarrollresources.net/doublets/puzzles.html
    Tools::Solution_t<std::string> solution = generator.findPath("wheat", "bread");

    for (const auto &element : solution)
        std::cout << element->first << '\t';
    return 0;
}