#include <catch2/catch_all.hpp>
#include "Graph.h"

TEST_CASE( "Graph", "[Graph]")
{

    SECTION("Creation")
    {
        Graph<int> graph0;
        REQUIRE(graph0.getNodes().size() == 0);

        Graph<std::string> graph1;
        REQUIRE(graph1.getNodes().size() == 0);
    }

    SECTION("Add Nodes")
    {
        Graph<int> graph;
        graph.addNode(12);
    }

}