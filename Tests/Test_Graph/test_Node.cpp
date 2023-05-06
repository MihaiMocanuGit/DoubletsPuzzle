#include <catch2/catch_all.hpp>
#include "Graph/Graph.h"

TEST_CASE( "Node", "[Node]")
{

    SECTION("Iterators")
    {
        Graph<int> graph;
        auto itParent = graph.addNode(0);

        for (int i = 1; i <= 50; ++i)
            graph.addNode(i, {{0,&(itParent->second)}});

        int count = 0;
        for (auto it = itParent->second.beginNeighbours(); it != itParent->second.endNeighbours(); it++)
            count++;

        CHECK(count == 50);

    }

}