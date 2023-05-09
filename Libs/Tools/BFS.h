#pragma once

#include <queue>

#include "../Graph/Graph.h"

namespace Tools
{
template<typename T>
using NodeCIterator_t = typename MapNodes_t<T>::const_iterator;

template<typename T>
using Solution_t = std::deque<NodeCIterator_t<T>>;


template<typename T>
void BFS(const NodeCIterator_t <T> &start, const T &searchedValue, Solution_t <T> &out_solutionPath);

template<typename T>
void searchNodesAtDistance(const NodeCIterator_t <T> &start, long unsigned  distance, Solution_t <T> &out_solutionPath);

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
    NodeCIterator_t<T> current;
    long unsigned indexParent;
};

template<typename T>
void generateSolution(const std::deque<Private::NodePair<T>> &history, Solution_t<T> &out_solutionPath)
{
    NodeCIterator_t<T> current = history.back().current;
    long unsigned indexCurrent = history.size() - 1;

    out_solutionPath.push_back(current);

    while (indexCurrent > 0)
    {
        indexCurrent = history[indexCurrent].indexParent;
        out_solutionPath.push_back(history[indexCurrent].current);
    }
}

} //end namespace Private

template<typename T>
void BFS(const NodeCIterator_t <T> &start, const T &searchedValue, Solution_t <T> &out_solutionPath)
{
    out_solutionPath = {};

    std::deque<NodeCIterator_t<T>> queue;
    std::deque<Private::NodePair<T>> history;

    std::set<T> explored = {start->first};
    queue.push_back(start);
    history.push_back({start, 0});

    if (start->first == searchedValue)
    {
        generateSolution(history, out_solutionPath);
        return;
    }

    long unsigned indexCurrent = 0;
    while (not queue.empty())
    {
        NodeCIterator_t<T> current = queue.front();
        queue.pop_front();


        for (auto next = current->second.beginNeighbours(); next != current->second.endNeighbours(); next++)
        {
            if (explored.find(next->first) == explored.end())
            {
                explored.insert(next->first);
                queue.push_back(next->second->getIterator());
                history.push_back({next->second->getIterator(), indexCurrent});
            }

            if (next->first == searchedValue)
            {
                generateSolution(history, out_solutionPath);
                return;
            }
        }
        indexCurrent++;
    }
}

template<typename T>
void searchNodesAtDistance(const NodeCIterator_t <T> &start, long unsigned  distance, Solution_t <T> &out_solutionPath)
{
    out_solutionPath = {};

    std::deque<std::pair< NodeCIterator_t<T>, long unsigned>> queue;

    std::set<T> explored = {start->first};
    queue.push_back({start, 0});

    if (distance == 0)
    {
        out_solutionPath.push_back(start);
        return;
    }
    while (not queue.empty())
    {
        NodeCIterator_t<T> current = queue.front().first;
        long unsigned currentDistance = queue.front().second;
        queue.pop_front();

        //we go through the neighbours of the current node
        for (auto next = current->second.beginNeighbours(); next != current->second.endNeighbours(); next++)
        {
            if (explored.find(next->first) == explored.end())
            {
                explored.insert(next->first);

                if (currentDistance + 1 < distance)
                    queue.push_back({next->second->getIterator(), currentDistance + 1});
                else
                    out_solutionPath.push_back(next->second->getIterator());

            }
        }
    }
}

} //end namespace Tools