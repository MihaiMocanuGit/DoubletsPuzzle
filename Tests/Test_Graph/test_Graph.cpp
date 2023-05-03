#include <catch2/catch_all.hpp>
#include "Graph.h"

TEST_CASE( "Graph", "[Graph]")
{

    SECTION("Creation")
    {
        Graph<int> graph0;
        REQUIRE(graph0.getNodes().size() == 0);

        Graph<int> graph1(5);
        REQUIRE(graph1.getNodes().size() == 5);

        Graph<int> graph2(500);
        REQUIRE(graph2.getNodes().size() == 500);
    }

}