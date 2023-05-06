#include <catch2/catch_all.hpp>
#include "Tools/BFS.h"


TEST_CASE("BFS", "[BFS]")
{
    Graph<int> graph;

    //we add some nodes having as data prime numbers
    auto it2 = graph.addNode(2);
    auto it3 = graph.addNode(3);
    auto it5 = graph.addNode(5);
    auto it7 = graph.addNode(7);


    SECTION("Straight line near")
    {
        graph.connectNodes(it2, it3);
        graph.connectNodes(it3, it5);
        graph.connectNodes(it5, it7);


        Tools::Solution_t<int> solution;
        Tools::BFS(graph, it2, 7, solution);

        CHECK(solution.size() == 4);
        CHECK(solution[0]->first == 7);
        CHECK(solution[1]->first == 5);
        CHECK(solution[2]->first == 3);
        CHECK(solution[3]->first == 2);


    }

    SECTION("Straight line with branches near")
    {
        graph.connectNodes(it2, it3);
        graph.connectNodes(it3, it5);
        graph.connectNodes(it5, it7);

        auto it4 = graph.addNode(4);
        graph.connectNodes(it2, it4);

        auto it8 = graph.addNode(8);
        graph.connectNodes(it2, it8);

        auto it6 = graph.addNode(6);
        graph.connectNodes(it3, it6);

        auto it9 = graph.addNode(9);
        graph.connectNodes(it3, it9);

        graph.connectNodes(it9, it8);
        graph.connectNodes(it9, it7);


        Tools::Solution_t<int> solution;
        Tools::BFS(graph, it2, 7, solution);

        CHECK(solution.size() == 4);
        CHECK(solution[0]->first == 7);
        CHECK(solution[1]->first == 5);
        CHECK(solution[2]->first == 3);
        CHECK(solution[3]->first == 2);

    }

    SECTION("Bigger Graph")
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

        Tools::Solution_t<int> solution;
        Tools::BFS(graph, it2, 2, solution);
        CHECK(solution.size() == 1);
        CHECK(solution[0]->first == 2);

        Tools::BFS(graph, it2, 16, solution);
        CHECK(solution.size() == 2);
        CHECK(solution[0]->first == 2);
        CHECK(solution[1]->first == 16);

        Tools::BFS(graph, it2, 64, solution);
        CHECK(solution.size() == 2);
        CHECK(solution[0]->first == 2);
        CHECK(solution[1]->first == 64);
    }
}