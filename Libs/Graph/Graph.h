#pragma once

#include <vector>
#include <stdexcept>

#include "Node.h"



template <typename T>
class Graph
{
private:
    std::set<Node<T>> m_nodes = {};

public:
    Graph() = default;
    Graph(int noEmptyNodes);

    void addNode(const SetOfNodesRef_t<T> &neighbourNodes);
    void removeNode(Node<T> &r_node);
    void connectNodes(Node<T> &firstNode, Node<T> &secondNode);

    const std::set<Node<T>> &getNodes() const;
};

template<typename T>
Graph<T>::Graph(int noEmptyNodes)
{
    for (int i = 0; i < noEmptyNodes; ++i)
        m_nodes.insert( Node<T>(*this));
}

template<typename T>
void Graph<T>::addNode(const SetOfNodesRef_t<T> &neighbourNodes)
{
    auto pair = m_nodes.emplace(*this, neighbourNodes);

    //if the node was inserted (meaning it wasn't already inside the graph)
    if (pair.second)
        //update all nodes with which it has an edge so that they know about their new neighbour
        for (Node<T> &r_node : neighbourNodes)
            if (not r_node.addEdge(*(pair.first)))
                throw std::logic_error("Edge already existed!");
}

template<typename T>
void Graph<T>::removeNode(Node<T> &r_node)
{
    //remove all edges from the other nodes to this node
    for (auto &neighbours : r_node.m_r_neighbourNodes)
        if (not neighbours.removeEdge(r_node))
            throw std::logic_error("Edge did not exist!");
}

template<typename T>
void Graph<T>::connectNodes(Node<T> &firstNode, Node<T> &secondNode)
{
    if (firstNode.addEdge(secondNode) != secondNode.addEdge(firstNode))
        throw std::logic_error("Only one of the node was already connected!");
}

template<typename T>
const std::set<Node<T>> &Graph<T>::getNodes() const
{
    return m_nodes;
}

