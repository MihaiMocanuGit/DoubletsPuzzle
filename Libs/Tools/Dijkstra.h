#pragma once

#include "../Graph/Graph.h"

namespace Tools
{
    template <typename T>
    void dijkstra(Graph<T> graph, const T &searchedValue);
}
