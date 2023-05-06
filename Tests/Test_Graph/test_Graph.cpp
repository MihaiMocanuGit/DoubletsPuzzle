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

        //we add some nodes having as data prime numbers
        auto it2 = graph.addNode(2);
        auto it3 = graph.addNode(3);
        auto it5 = graph.addNode(5);
        auto it7 = graph.addNode(7);

        SECTION("Connect nodes at creation")
        {
            for (int i = 10; i <= 100; ++i)
            {
                MapNodesPtr_t<int> neighbours;

                //if the node with data i is divided by any of the prime nodes, we create a link
                if (i % 2 == 0)
                    neighbours.insert({2, &(it2->second)});
                if (i % 3 == 0)
                    neighbours.insert({3, &(it3->second)});
                if (i % 5 == 0)
                    neighbours.insert({5, &(it5->second)});
                if (i % 7 == 0)
                    neighbours.insert({7, &(it7->second)});

                graph.addNode(i, neighbours);
            }

            CHECK(it2->second.getNeighbours().size() == (100 / 2) - (9 / 2));
            CHECK(it3->second.getNeighbours().size() == (100 / 3) - (9 / 3));
            CHECK(it5->second.getNeighbours().size() == (100 / 5) - (9 / 5));
            CHECK(it7->second.getNeighbours().size() == (100 / 7) - (9 / 7));

            for (auto it = graph.begin(); it != graph.end(); it++)
            {
                if (it != it2 and it != it3 and it != it5 and it != it7)
                {
                    unsigned long divisors = (it->first % 2 == 0) + (it->first % 3 == 0)
                                    + (it->first % 5 == 0) + (it->first % 7 == 0);
                    CHECK(it->second.getNeighbours().size() == divisors);
                }
            }
        }

        SECTION("Connect/Disconnect nodes after creation")
        {
            for (int i = 10; i <= 100; ++i)
                graph.addNode(i);

            for (int i = 10; i <= 100; ++i)
            {

                //if the node with data i is divided by any of the prime nodes, we create a link
                if (i % 2 == 0)
                    graph.connectNodes(graph.findNode(i), it2);
                if (i % 3 == 0)
                    graph.connectNodes(graph.findNode(i), it3);
                if (i % 5 == 0)
                    graph.connectNodes(graph.findNode(i), it5);
                if (i % 7 == 0)
                    graph.connectNodes(graph.findNode(i), it7);

            }

            CHECK(it2->second.getNeighbours().size() == (100 / 2) - (9 / 2));
            CHECK(it3->second.getNeighbours().size() == (100 / 3) - (9 / 3));
            CHECK(it5->second.getNeighbours().size() == (100 / 5) - (9 / 5));
            CHECK(it7->second.getNeighbours().size() == (100 / 7) - (9 / 7));

            for (auto it = graph.begin(); it != graph.end(); it++)
            {
                if (it != it2 and it != it3 and it != it5 and it != it7)
                {
                    unsigned long divisors = (it->first % 2 == 0) + (it->first % 3 == 0)
                                    + (it->first % 5 == 0) + (it->first % 7 == 0);
                    CHECK(it->second.getNeighbours().size() == divisors);
                }
            }

            SECTION("Disconnect Nodes")
            {
                for (int i = 10; i <= 100; ++i)
                {
                    //if the node with data i is divided by any of the prime nodes, we delete the link
                    if (i % 2 == 0)
                        graph.disconnectNodes(graph.findNode(i), it2);
                    if (i % 3 == 0)
                        graph.disconnectNodes(graph.findNode(i), it3);
                    if (i % 5 == 0)
                        graph.disconnectNodes(graph.findNode(i), it5);
                    if (i % 7 == 0)
                        graph.disconnectNodes(graph.findNode(i), it7);

                }

                //disconnecting nodes that weren't connected should do nothing
                CHECK_NOTHROW(graph.disconnectNodes(it2, it3));
            }
        }



    }

}