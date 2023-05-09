#pragma once

#include <map>
#include <stdexcept>
#include <algorithm>



template <typename T>
class Node;

template <typename T>
using MapNodes_t = std::map<T, Node<T>>;

template <typename T>
using MapNodesPtr_t = std::map<T, Node<T> *>;


template <typename T>
class Graph;

template <typename T>
class Node
{
    friend class Graph<T>;
private:
    T m_data;
    Graph<T> &m_r_parentGraph;
    MapNodes_t<T>::iterator m_iterator;


    MapNodesPtr_t<T> m_neighbourNodesPtr = {};


    bool m_connectNode(Node<T> &withNode);
    bool m_disconnectNode(const Node<T> &fromNode);

    Node(const T &data, Graph<T> &parentGraph, const MapNodesPtr_t<T> &r_neighbourNodes = {});
    void m_setIterator(const MapNodes_t<T>::iterator &it);

public:
    const T &getData() const;
    const MapNodes_t<T>::iterator &getIterator() const;
    MapNodes_t<T>::iterator getIterator();

    const MapNodesPtr_t<T> &getNeighbours() const;

    MapNodesPtr_t<T>::const_iterator beginNeighbours() const;
    MapNodesPtr_t<T>::iterator beginNeighbours();

    MapNodesPtr_t<T>::const_iterator endNeighbours() const;
    MapNodesPtr_t<T>::iterator endNeighbours();

};

template<typename T>
std::map<T, Node<T>>::iterator Node<T>::getIterator()
{
    return m_iterator;
}

template<typename T>
const MapNodes_t<T>::iterator &Node<T>::getIterator() const
{
    return m_iterator;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////PRIVATE/////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
Node<T>::Node(const T &data, Graph<T> &parentGraph, const MapNodesPtr_t<T> &r_neighbourNodes)
    : m_data{data}, m_r_parentGraph{parentGraph}
{
    bool badNode = false;

    //we copy only the nodes from the same parent graph.
    //std::copy(r_neighbourNodes.begin(), r_neighbourNodes.end(), m_neighbourNodesPtr.begin());
    m_neighbourNodesPtr = MapNodesPtr_t<T>(r_neighbourNodes.begin(), r_neighbourNodes.end());
    /*, [&](const auto &node)->bool
              {
                    if (not m_nodeExistsInGraph(node))
                {
                        badNode = true;
                        return false;
                    }
                    else
                        return true;
              });*/

    //if we have at least one Node which is not from the same graph, badNode will be set to true;
    if (badNode)
        throw std::logic_error("At least one of the given node is not from the same graph");
}

template<typename T>
void Node<T>::m_setIterator(const std::map<T, Node<T>>::iterator &it)
{
    m_iterator = it;
}

template<typename T>
bool Node<T>::m_connectNode(Node<T> &withNode)
{

    const auto [it, success] = m_neighbourNodesPtr.insert(std::make_pair(withNode.m_data, &withNode));
    return success;
}

template<typename T>
bool Node<T>::m_disconnectNode(const Node<T> &fromNode)
{
    return (bool)m_neighbourNodesPtr.erase(fromNode.m_data);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////PUBLIC//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
const T &Node<T>::getData() const
{
    return m_data;
}

template<typename T>
const MapNodesPtr_t<T> &Node<T>::getNeighbours() const
{
    return m_neighbourNodesPtr;
}


template<typename T>
std::map<T, Node<T> *>::iterator Node<T>::endNeighbours()
{
    return m_neighbourNodesPtr.end();
}

template<typename T>
std::map<T, Node<T> *>::const_iterator Node<T>::endNeighbours() const
{
    return m_neighbourNodesPtr.end();
}

template<typename T>
std::map<T, Node<T> *>::iterator Node<T>::beginNeighbours()
{
    return m_neighbourNodesPtr.begin();
}

template<typename T>
std::map<T, Node<T> *>::const_iterator Node<T>::beginNeighbours() const
{
    return m_neighbourNodesPtr.begin();
}
