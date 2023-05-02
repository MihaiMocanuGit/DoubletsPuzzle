#pragma once

#include <set>
#include <stdexcept>
#include <algorithm>

template <typename T>
class Graph;

template <typename T>
class Node
{
    friend class Graph<T>;
private:
    Graph<T> &m_r_parentGraph;
    std::set<Node<T> &> m_r_neighbourNodes = {};

    bool m_nodeExistsInGraph(const Node<T> &node) const;
public:
    explicit Node(Graph<T> &parentGraph, const std::set<Node<T> &> &r_neighbourNodes = {});

    bool addEdge(Node<T> &withNode);
    bool removeEdge(Node<T> &withNode);

};


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////PRIVATE/////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
bool Node<T>::m_nodeExistsInGraph(const Node<T> &node) const
{
    return this->m_r_parentGraph == node.m_r_parentGraph;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////PUBLIC//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////


template<typename T>
bool Node<T>::removeEdge(Node<T> &withNode)
{
    if (not m_nodeExistsInGraph(withNode))
        throw std::logic_error("Given Node is not from the same graph!");

    //returns 1 if it erased the node (i.e it did exist in the set), zero otherwise
    return m_r_neighbourNodes.erase(withNode);
}

template<typename T>
bool Node<T>::addEdge(Node<T> &withNode)
{
    if (not m_nodeExistsInGraph(withNode))
        throw std::logic_error("Given Node is not from the same graph!");

    //returns 1 if it inserted the node (i.e it did not exist in the set), zero otherwise
    return m_r_neighbourNodes.insert(withNode);
}

template<typename T>
Node<T>::Node(Graph<T> &parentGraph, const std::set<Node<T> &> &r_neighbourNodes) : m_r_parentGraph{parentGraph}
{
    bool badNode = false;

    //we copy only the nodes from the same parent graph.
    std::copy_if(r_neighbourNodes.begin(), r_neighbourNodes.end(),
              m_r_neighbourNodes.begin(), [&](const Node<T> &node)->bool
              {
                    if (not m_nodeExistsInGraph(node.m_r_parentGraph))
                    {
                        badNode = true;
                        return true;
                    }
                    else
                        return false;
              });

    //if we have at least one Node which is not from the same graph, badNode will be set to true;
    if (badNode)
        throw std::logic_error("At least one of the given node is not from the same graph");
}



