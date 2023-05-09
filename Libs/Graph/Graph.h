#pragma once

#include <vector>
#include <unordered_set>
#include <stdexcept>

#include "Node.h"

template <typename T>
using NeighbhoursIterators_t = std::unordered_set<typename MapNodes_t<T>::iterator>;


template <typename T>
class Graph
{
private:
    MapNodes_t<T> m_nodes = {};

public:
    Graph() = default;

    bool nodeIsFromGraph(const Node<T> &node) const;

    MapNodes_t<T>::iterator addNode(const T &data, const MapNodesPtr_t<T> &neighbourNodes = {});

    //the iterator will be modified, it will be set to the iterator following the last removed element!
    void removeNode(MapNodes_t<T>::iterator &nodeIt);
    void removeNode(MapNodes_t<T>::iterator &&nodeIt); //r_value reference version for ease of usability

    //the iterator will be modified, it will be set to its new position!
    void modifyNodeData(MapNodes_t<T>::iterator &nodeIt, const T& data);
    void modifyNodeData(MapNodes_t<T>::iterator &&nodeIt, const T& data); //r_value reference version for ease of usability

    void connectNodes(const MapNodes_t<T>::iterator &firstNodeIt, const MapNodes_t<T>::iterator &secondNodeIt);
    void disconnectNodes(const MapNodes_t<T>::iterator &firstNodeIt, const MapNodes_t<T>::iterator &secondNodeIt);

    const MapNodes_t<T> &getNodes() const;

    MapNodes_t<T>::const_iterator cbegin() const;
    MapNodes_t<T>::iterator begin();

    MapNodes_t<T>::const_iterator cend() const;
    MapNodes_t<T>::iterator end();

    unsigned int size() const;

    const MapNodes_t<T>::iterator &findNode(const T &withData) const;
    MapNodes_t<T>::iterator findNode(const T &withData);

};


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////PUBLIC//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
bool Graph<T>::nodeIsFromGraph(const Node<T> &node) const
{
    return &node.m_r_parentGraph == this;
}

template<typename T>
MapNodes_t<T>::iterator Graph<T>::addNode(const T &data, const MapNodesPtr_t<T> &neighbourNodes)
{
    if (m_nodes.find(data) != m_nodes.end())
        throw std::logic_error("Node with same data already exists");

    Node<T> node(data, *this, neighbourNodes);
    const auto [it, success] = m_nodes.insert(std::make_pair(data, node));
    it->second.m_setIterator(it);
    //we connect all neighbours with itself
    for (auto &neighbour : neighbourNodes)
        neighbour.second->m_connectNode(it->second);

    return it;
}

template<typename T>
void Graph<T>::removeNode(MapNodes_t<T>::iterator &nodeIt)
{
    if (nodeIt == m_nodes.end())
        throw std::logic_error("Invalid Iterator");
    if (not nodeIsFromGraph(nodeIt->second))
        throw std::logic_error("The node is not from this graph");

    //we remove all edges from its neighbours with itself
    for (auto &neighbour : nodeIt->second.m_neighbourNodesPtr)
       if (not neighbour.second->m_disconnectNode(nodeIt->second))
           throw std::logic_error("Node was not mutually connected with neighbour");

   nodeIt = m_nodes.erase(nodeIt);
}

template<typename T>
void Graph<T>::removeNode(std::map<T, Node<T>>::iterator &&nodeIt)
{
    removeNode(nodeIt);
}

template<typename T>
void Graph<T>::modifyNodeData(std::map<T, Node<T>>::iterator &nodeIt, const T &data)
{
    if (nodeIt == m_nodes.end())
        throw std::logic_error("Invalid Iterator");
    if (not nodeIsFromGraph(nodeIt->second))
        throw std::logic_error("The node is not from this graph");

    auto copyOfNeighbours = nodeIt->second.m_neighbourNodesPtr;

    removeNode(nodeIt);

    nodeIt = addNode(data, copyOfNeighbours);
}
template<typename T>
void Graph<T>::modifyNodeData(std::map<T, Node<T>>::iterator &&nodeIt, const T &data)
{
    modifyNodeData(nodeIt, data);
}

template<typename T>
void Graph<T>::connectNodes(const std::map<T, Node<T>>::iterator &firstNodeIt,
                            const std::map<T, Node<T>>::iterator &secondNodeIt)
{
    if (firstNodeIt == m_nodes.end() or secondNodeIt == m_nodes.end() )
        throw std::logic_error("Invalid Iterator(s)");
    if (not nodeIsFromGraph(firstNodeIt->second) or not nodeIsFromGraph(secondNodeIt->second))
        throw std::logic_error("At least one node is not from this graph");

    if(firstNodeIt->second.m_connectNode(secondNodeIt->second) != secondNodeIt->second.m_connectNode(firstNodeIt->second))
        throw std::logic_error("One node was connected with the other");
}



template<typename T>
void Graph<T>::disconnectNodes(const std::map<T, Node<T>>::iterator &firstNodeIt,
                               const std::map<T, Node<T>>::iterator &secondNodeIt)
{
    if (firstNodeIt == m_nodes.end() or secondNodeIt == m_nodes.end() )
        throw std::logic_error("Invalid Iterator(s)");
    if (not nodeIsFromGraph(firstNodeIt->second) or not nodeIsFromGraph(secondNodeIt->second))
        throw std::logic_error("At least one node is not from this graph");

    if(firstNodeIt->second.m_disconnectNode(secondNodeIt->second) != secondNodeIt->second.m_disconnectNode(firstNodeIt->second))
        throw std::logic_error("One node was connected with the other");
}

template<typename T>
const MapNodes_t<T> &Graph<T>::getNodes() const
{
    return m_nodes;
}

template<typename T>
std::map<T, Node<T>>::const_iterator Graph<T>::cbegin() const
{
    return m_nodes.begin();
}

template<typename T>
std::map<T, Node<T>>::iterator Graph<T>::begin()
{
    return m_nodes.begin();
}

template<typename T>
std::map<T, Node<T>>::const_iterator Graph<T>::cend() const
{
    return m_nodes.end();
}

template<typename T>
std::map<T, Node<T>>::iterator Graph<T>::end()
{
    return m_nodes.end();
}

template<typename T>
unsigned int Graph<T>::size() const
{
    return m_nodes.size();
}


template<typename T>
std::map<T, Node<T>>::iterator Graph<T>::findNode(const T &withData)
{
    return m_nodes.find(withData);
}

template<typename T>
const std::map<T, Node<T>>::iterator &Graph<T>::findNode(const T &withData) const
{
    return m_nodes.find(withData);
}



