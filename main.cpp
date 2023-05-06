#include <iostream>

#include "Libs/Graph/Graph.h"
#include "Libs/Tools/BFS.h"

int main()
{
    Graph<int> graph;

    Tools::Solution_t<int> solution;
    Tools::BFS(graph.begin(), 20, solution);
    return 0;
}