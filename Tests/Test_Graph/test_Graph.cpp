#include <catch2/catch_all.hpp>
#include "Graph/Graph.h"

TEST_CASE( "Graph", "[Graph]")
{

    SECTION("Creation")
    {
        Graph<int> graph0;
        REQUIRE(graph0.getNodes().size() == 0);

        Graph<std::string> graph1;
        REQUIRE(graph1.getNodes().size() == 0);
    }

    SECTION("Add/Remove Nodes")
    {
        Graph<int> graph;
        auto it = graph.addNode(12);

        REQUIRE(graph.getNodes().size() == 1);
        REQUIRE(it->first == 12);

        REQUIRE_THROWS_AS(graph.addNode(12), std::logic_error);
        REQUIRE(graph.getNodes().size() == 1);

        it = graph.addNode(13);
        REQUIRE(graph.getNodes().size() == 2);
        REQUIRE(it->first == 13);

        graph.removeNode(it);
        REQUIRE(graph.getNodes().size() == 1);
        REQUIRE(it == graph.getNodes().end());
    }

}