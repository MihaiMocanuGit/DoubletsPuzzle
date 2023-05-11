#include <iostream>

#include "Libs/Application/Application.h"

int main()
{
    Application app;

    app.startApp();
    /*
    Generator generator("words_alpha.txt");
    generator.generateGraph(5);

    //https://lewiscarrollresources.net/doublets/puzzles.html
    Tools::Solution_t<std::string> solution = generator.findPath("wheat", "bread");

    for (const auto &element : solution)
        std::cout << element->first << '\t';
    */


    return 0;
}