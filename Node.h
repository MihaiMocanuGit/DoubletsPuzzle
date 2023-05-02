#pragma once

#include <set>

template <typename T>
class Graph;

template <typename T>
class Node
{
    friend class Graph<T>;
private:
    Graph<T> &m_r_parentGraph;
    std::set<T &> m_r_neighbourNodes = {};
public:
    explicit Node(Graph<T> &parentGraph, const std::set<T &> &r_neighbourNodes = {});

    bool addEdge(Node<T> &withNode);
    bool removeEdge(Node<T> &withNode);

};

template<typename T>
bool Node<T>::removeEdge(Node<T> &withNode)
{
    //returns 1 if it erased the node (i.e it did exist in the set), zero otherwise
    return m_r_neighbourNodes.erase(withNode);
}

template<typename T>
bool Node<T>::addEdge(Node<T> &withNode)
{
    //returns 1 if it inserted the node (i.e it did not exist in the set), zero otherwise
    return m_r_neighbourNodes.insert(withNode);
}

template<typename T>
Node<T>::Node(Graph<T> &parentGraph, const std::set<T &> &r_neighbourNodes) : m_r_parentGraph{parentGraph}
{
    std::copy(r_neighbourNodes.begin(), r_neighbourNodes.end(),
              m_r_neighbourNodes.begin());
}



