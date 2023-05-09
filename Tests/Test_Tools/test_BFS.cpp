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


    SECTION("Small Straight line")
    {
        graph.connectNodes(it2, it3);
        graph.connectNodes(it3, it5);
        graph.connectNodes(it5, it7);


        Tools::Solution_t<int> solution;
        Tools::BFS(it2, 7, solution);

        CHECK(solution.size() == 4);
        CHECK(solution[0]->first == 7);
        CHECK(solution[1]->first == 5);
        CHECK(solution[2]->first == 3);
        CHECK(solution[3]->first == 2);


    }

    SECTION("small Straight line with some branches")
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
        Tools::BFS(it2, 7, solution);

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
        Tools::BFS(it2, 2, solution);
        CHECK(solution.size() == 1);
        CHECK(solution[0]->first == 2);

        Tools::BFS(it2, 16, solution);
        CHECK(solution.size() == 2);
        CHECK(solution[0]->first == 16);
        CHECK(solution[1]->first == 2);

        Tools::BFS(it2, 64, solution);
        CHECK(solution.size() == 2);
        CHECK(solution[0]->first == 64);
        CHECK(solution[1]->first == 2);

        Tools::BFS(graph.findNode(64), graph.findNode(16)->first, solution);
        CHECK(solution.size() == 3);
        CHECK(solution[0]->first == 16);
        CHECK(solution[1]->first == 2);
        CHECK(solution[2]->first == 64);

        Tools::BFS(graph.findNode(7 * 7), 5 * 5, solution);
        CHECK(solution.size() == 5);
        CHECK(solution[0]->first == 5*5);
        CHECK(solution[1]->first == 5);
        CHECK(solution[2]->first == 5*7);   //this might be implementation specific of std::set on ints, not sure.
                                            // It could as well be: k * 5 * 7, for any K natural non zero s.t k <= 100 / (5*7)
        CHECK(solution[3]->first == 7);
        CHECK(solution[4]->first == 7*7);

        Tools::BFS(it2, -1, solution);
        CHECK(solution.empty());

    }

    SECTION("Graph with words")
    {
        Graph<std::string> graphStr;

        auto itAnna = graphStr.addNode(std::string("Anna"));
        auto itAre = graphStr.addNode(std::string("are"));
        auto itMere = graphStr.addNode(std::string("mere"));
        auto itPaul = graphStr.addNode(std::string("Paul"));
        auto itNu = graphStr.addNode(std::string("nu"));

        graphStr.connectNodes(itAnna, itAre);
        graphStr.connectNodes(itAre, itMere);

        graphStr.connectNodes(itPaul, itNu);

        Tools::Solution_t<std::string> solution;
        Tools::BFS(itAnna, std::string("mere"), solution);
        CHECK(solution.size() == 3);
        CHECK(solution[0]->first == std::string("mere"));
        CHECK(solution[1]->first == std::string("are"));
        CHECK(solution[2]->first == std::string("Anna"));
    }
}

TEST_CASE("BFS Find nodes at distance", "[BFS]")
{
    Graph<int> graph;

    //                      5
    //                      3
    //                      1
    //                      |
    //          -6 -4 -2 -- 0 -- 2 4 6
    //                      |
    //                     -1
    //                     -3
    //                     -5


    auto itParent = graph.addNode(0);

    auto itPrevious = itParent;
    for (int i = 1; i <= 5; ++i)
    {
        auto itCurrent = graph.addNode(2 * i);
        graph.connectNodes(itCurrent, itPrevious);

        itPrevious = itCurrent;
    }

    itPrevious = itParent;
    for (int i = 1; i <= 5; ++i)
    {
        auto itCurrent = graph.addNode(2 * i - 1);
        graph.connectNodes(itCurrent, itPrevious);

        itPrevious = itCurrent;
    }

    itPrevious = itParent;
    for (int i = 1; i <= 5; ++i)
    {
        auto itCurrent = graph.addNode(-2 * i);
        graph.connectNodes(itCurrent, itPrevious);

        itPrevious = itCurrent;
    }

    itPrevious = itParent;
    for (int i = 1; i <= 5; ++i)
    {
        auto itCurrent = graph.addNode(-2 * i + 1);
        graph.connectNodes(itCurrent, itPrevious);

        itPrevious = itCurrent;
    }

    SECTION("Distance 0")
    {
        Tools::Solution_t<int> solution;
        Tools::searchNodesAtDistance<int>(itParent, 0, solution);

        CHECK(solution.size() == 1);
        CHECK(solution[0]->first == 0);
    }

    SECTION("Distance 1-5")
    {
        Tools::Solution_t<int> solution;
        Tools::searchNodesAtDistance<int>(itParent, 1, solution);

        CHECK(solution.size() == 4);

        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(1)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(-1)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(-2)) != solution.end());

        Tools::searchNodesAtDistance<int>(itParent, 2, solution);

        CHECK(solution.size() == 4);

        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(1 + 2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(-1 - 2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(2 + 2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(-2 - 2)) != solution.end());


        Tools::searchNodesAtDistance<int>(itParent, 3, solution);

        CHECK(solution.size() == 4);

        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(1 + 2*2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(-1 - 2*2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(2 + 2*2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(-2 - 2*2)) != solution.end());

        Tools::searchNodesAtDistance<int>(itParent, 4, solution);

        CHECK(solution.size() == 4);

        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(1 + 3*2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(-1 - 3*2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(2 + 3*2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(-2 - 3*2)) != solution.end());

        Tools::searchNodesAtDistance<int>(itParent, 5, solution);

        CHECK(solution.size() == 4);

        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(1 + 4*2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(-1 - 4*2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(2 + 4*2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(-2 - 4*2)) != solution.end());
    }

    SECTION("Invalid distance")
    {
        Tools::Solution_t<int> solution;
        Tools::searchNodesAtDistance<int>(itParent, 6, solution);

        CHECK(solution.empty());


        Tools::searchNodesAtDistance<int>(itParent, -1, solution);

        CHECK(solution.empty());
    }

    SECTION("Modified graph 1-5")
    {
        graph.connectNodes(graph.findNode(1), graph.findNode(-1));
        graph.connectNodes(graph.findNode(2), graph.findNode(-2));

        graph.connectNodes(graph.findNode(3), graph.findNode(-3));
        graph.connectNodes(graph.findNode(4), graph.findNode(-4));

        graph.connectNodes(graph.findNode(5), graph.findNode(-5));
        graph.connectNodes(graph.findNode(6), graph.findNode(-6));

        graph.connectNodes(graph.findNode(8), graph.findNode(-8));
        graph.connectNodes(graph.findNode(8), graph.findNode(-8));


        Tools::Solution_t<int> solution;
        Tools::searchNodesAtDistance<int>(itParent, 1, solution);

        CHECK(solution.size() == 4);

        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(1)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(-1)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(-2)) != solution.end());

        Tools::searchNodesAtDistance<int>(itParent, 2, solution);

        CHECK(solution.size() == 4);

        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(1 + 2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(-1 - 2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(2 + 2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(-2 - 2)) != solution.end());


        Tools::searchNodesAtDistance<int>(itParent, 3, solution);

        CHECK(solution.size() == 4);

        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(1 + 2*2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(-1 - 2*2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(2 + 2*2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(-2 - 2*2)) != solution.end());

        Tools::searchNodesAtDistance<int>(itParent, 4, solution);

        CHECK(solution.size() == 4);

        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(1 + 3*2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(-1 - 3*2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(2 + 3*2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(-2 - 3*2)) != solution.end());

        Tools::searchNodesAtDistance<int>(itParent, 5, solution);

        CHECK(solution.size() == 4);

        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(1 + 4*2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(-1 - 4*2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(2 + 4*2)) != solution.end());
        CHECK(std::find(solution.begin(), solution.end(), graph.findNode(-2 - 4*2)) != solution.end());
    }


}