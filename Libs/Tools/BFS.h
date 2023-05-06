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
