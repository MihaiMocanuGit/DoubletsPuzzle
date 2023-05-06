#include "BFS.h"

#include <set>

namespace Tools
{

namespace Private
{
template<typename T>
struct NodePair
{
    NodeIterator_t<T> current;
    NodeIterator_t<T> prev;
    int indexPrev;
};

template<typename T>
void generateSolution(const std::deque<Private::NodePair<T>> &queue, Solution_t<T> &out_solutionPath)
{
    NodeIterator_t<T> current = queue.back().current;
    int indexCurrent = queue.size() - 1;

    NodeIterator_t<T> start = queue.front();
    int indexStart = 0;

    out_solutionPath.push_back(current);

    while (indexCurrent > indexStart)
    {
        current = queue[indexCurrent].prev;
        indexCurrent = queue[indexCurrent].indexPrev;
        out_solutionPath.push_back(queue[indexCurrent]);
    }

}

}


template<typename T>
void BFS(const Graph<T> &graph, const NodeIterator_t<T> &start, const T &searchedValue,
         Solution_t<T> &out_solutionPath)
{
    std::deque<Private::NodePair<T>> queue;

    std::set<T> explored = {start->first};
    queue.push({start, start, 0});

    while (not queue.empty())
    {
        NodeIterator_t<T> current = queue.front().current;
        queue.pop();

        if (current->first == searchedValue)
        {
            generateSolution(queue, out_solutionPath);
            return;
        }
        for (auto next = current->second.beginNeighbours(); next != current->second.endNeighbours(); next++)
        {
            if (explored.find(next.first) == explored.end())
            {
                explored.insert(next.first);
                queue.push({next, current, queue.size() - 1});
            }
        }
    }
}


}