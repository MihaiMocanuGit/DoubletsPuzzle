#include <catch2/catch_all.hpp>
#include "Graph/Graph.h"

TEST_CASE( "Graph", "[Graph]")
{

    SECTION("Creation")
    {
        Graph<int> graph0;
        REQUIRE(graph0.getNodes().empty());

        Graph<std::string> graph1;
        REQUIRE(graph1.getNodes().empty());
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

        graph.removeNode(graph.begin());
        REQUIRE(graph.getNodes().empty());

        REQUIRE_THROWS_AS(graph.removeNode(graph.begin()), std::logic_error);
        REQUIRE(graph.getNodes().empty());

        Graph<int> graph2;

        for (int i = 0; i < 100; ++i)
            graph2.addNode(i);

        REQUIRE(graph2.size() == 100);
    }

    SECTION("ModifyData")
    {
        Graph<int> graph;
        REQUIRE_THROWS_AS(graph.modifyNodeData(graph.begin(), 21), std::logic_error);
        auto it = graph.addNode(12);
        graph.modifyNodeData(it, 21);

        Node<int> firstNode = graph.begin()->second;
        REQUIRE(firstNode.getData() == 21);
        REQUIRE(it->second.getData() == 21);

        auto it100 = graph.addNode(100);
        auto it101 = graph.addNode(101);
        auto it102 = graph.addNode(102);

        graph.modifyNodeData(it101, 201);
        REQUIRE(it101->second.getData() == 201);

        graph.modifyNodeData(it100, 300);
        REQUIRE(it100->second.getData() == 300);

        REQUIRE(it102->second.getData() == 102);
    }

    SECTION("Connect/Disconnect nodes")
    {
        Graph<int> graph;

        for (int i = 0; i < 100; ++i)
            graph.addNode(i);
    }

}