#pragma once

#include <vector>
#include <stdexcept>

#include "Node.h"



template <typename T>
using MapNodes_t = std::map<T, Node<T>>;

template <typename T>
class Graph
{
private:
    MapNodes_t<T> m_nodes = {};

public:
    Graph() = default;
    bool nodeIsFromGraph(const Node<T> &node) const;

    MapNodes_t<T>::iterator addNode(const T &data, const MapNodesPtr_t<T> &neighbourNodes = {});

    //the iterator will be modified, it will be set Iterator following the last removed element!
    void removeNode(MapNodes_t<T>::iterator &nodeIt);

    //the iterator will be modified, it will be set to its new position!
    void modifyNodeData(MapNodes_t<T>::iterator &nodeIt, const T& data);

    void connectNodes(const MapNodes_t<T>::iterator &firstNodeIt, const MapNodes_t<T>::iterator &secondNodeIt);
    void disconnectNodes(const MapNodes_t<T>::iterator &firstNodeIt, const MapNodes_t<T>::iterator &secondNodeIt);

    const MapNodes_t<T>::iterator &begin() const;
    const MapNodes_t<T>::iterator &end() const;
    const MapNodes_t<T> &getNodes() const;
};

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

    //we connect all neighbours with itself
    for (auto &neighbour : neighbourNodes)
        neighbour.second->m_connectNode(it->second);

    return it;
}

template<typename T>
void Graph<T>::removeNode(std::map<T, Node<T>>::iterator &nodeIt)
{
    if (not nodeIsFromGraph(*nodeIt))
        throw std::logic_error("The node is not from this graph");

    //we remove all edges from its neighbours with itself
    for (auto &neighbour : nodeIt->second.m_r_neighbourNodes)
       if (not neighbour.removeNode(nodeIt->first))
           throw std::logic_error("Node was not mutually connected with neighbour");

   nodeIt = m_nodes.erase(nodeIt);
}

template<typename T>
void Graph<T>::modifyNodeData(std::map<T, Node<T>>::iterator &nodeIt, const T &data)
{
    if (not nodeIsFromGraph(*nodeIt))
        throw std::logic_error("The node is not from this graph");

    Node<T> copy = *nodeIt;
    copy.m_data = data;

    removeNode(nodeIt);

    nodeIt = addNode(copy.m_data, copy.m_r_neighbourNodes);
}

template<typename T>
void Graph<T>::connectNodes(const std::map<T, Node<T>>::iterator &firstNodeIt,
                            const std::map<T, Node<T>>::iterator &secondNodeIt)
{
    if (not nodeIsFromGraph(*firstNodeIt) or not nodeIsFromGraph(*secondNodeIt))
        throw std::logic_error("At least one node is not from this graph");

    if(firstNodeIt->second.m_connectNode(*secondNodeIt) == 0 or secondNodeIt->second.m_connectNode(*firstNodeIt) == 0)
        throw std::logic_error("At least one node was already connected");
}



template<typename T>
void Graph<T>::disconnectNodes(const std::map<T, Node<T>>::iterator &firstNodeIt,
                               const std::map<T, Node<T>>::iterator &secondNodeIt)
{
    if (not nodeIsFromGraph(*firstNodeIt) or not nodeIsFromGraph(*secondNodeIt))
        throw std::logic_error("At least one node is not from this graph");

    if(firstNodeIt->second.m_disconnectNode(*secondNodeIt) != secondNodeIt->second.m_disconnectNode(*firstNodeIt))
        throw std::logic_error("Only one node was connected with the other");
}

template<typename T>
const std::map<T, Node<T>>::iterator &Graph<T>::begin() const
{
    return m_nodes.begin();
}

template<typename T>
const std::map<T, Node<T>>::iterator &Graph<T>::end() const
{
    return m_nodes.end();
}

template<typename T>
const MapNodes_t<T> &Graph<T>::getNodes() const
{
    return m_nodes;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////PUBLIC//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
