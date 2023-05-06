#pragma once

#include <queue>

#include "../Graph/Graph.h"

namespace Tools
{
template<typename T>
using NodeIterator_t = typename MapNodes_t<T>::iterator;

template<typename T>
using Solution_t = std::deque<NodeIterator_t<T>>;


template<typename T>
void BFS(const Graph<T> &graph, const NodeIterator_t<T> &start, const T &searchedValue,
         Solution_t<T> &out_solutionPath);

}



#include <set>
#include <iostream>
namespace Tools
{

namespace Private
{

template<typename T>
struct NodePair
{
    NodeIterator_t<T> current;
    NodeIterator_t<T> prev;
    long unsigned indexPrev;
};

template<typename T>
void generateSolution(const std::deque<Private::NodePair<T>> &queue, Solution_t<T> &out_solutionPath)
{
    NodeIterator_t<T> current = queue.back().current;
    long unsigned  indexCurrent = queue.size() - 1;

    NodeIterator_t<T> start = queue.front().current;

    out_solutionPath.push_back(current);

    while (indexCurrent > 0)
    {
        current = queue[indexCurrent].prev;
        indexCurrent = queue[indexCurrent].indexPrev;
        out_solutionPath.push_back(queue[indexCurrent].current);
    }

}

}


template<typename T>
void BFS(const Graph<T> &graph, const NodeIterator_t<T> &start, const T &searchedValue,
         Solution_t<T> &out_solutionPath)
{
    std::deque<NodeIterator_t<T>> queue;
    std::deque<Private::NodePair<T>> history;

    std::set<T> explored = {start->first};
    queue.push_back(start);
    history.push_back({start, start, 0});

    long unsigned indexCurrent = 0;
    while (not queue.empty())
    {
        NodeIterator_t<T> current = queue.front();
        std::cout << current->first << ' ';
        queue.pop_front();

        if (current->first == searchedValue)
        {
            generateSolution(history, out_solutionPath);
            std::cout << std::endl;
            return;
        }
        for (auto next = current->second.beginNeighbours(); next != current->second.endNeighbours(); next++)
        {
            if (explored.find(next->first) == explored.end())
            {
                explored.insert(next->first);
                queue.push_back(next->second->getIterator());
                history.push_back({next->second->getIterator(), current, indexCurrent});

            }
        }
        indexCurrent++;
    }
    std::cout << std::endl;
}


}