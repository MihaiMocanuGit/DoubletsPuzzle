#pragma once

#include <map>
#include <stdexcept>
#include <algorithm>

template <typename T>
class Graph;

template <typename T>
class Node;

template <typename T>
using NodeRefWrap_t = std::reference_wrapper<Node<T>>;

template <typename T>
using MapNodesRef_t = std::map<T, NodeRefWrap_t<T>>;

template <typename T>
class Node
{
    friend class Graph<T>;
private:
    Graph<T> &m_r_parentGraph;
    T m_data;

    MapNodesRef_t<T> m_r_neighbourNodes = {};


    bool m_connectNode(const Node<T> &withNode);
    bool m_disconnectNode(const Node<T> &fromNode);
public:
    Node(const T &data, Graph<T> &parentGraph, const MapNodesRef_t<T> &r_neighbourNodes = {});
    const T &getData() const;

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////PRIVATE/////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
Node<T>::Node(const T &data, Graph<T> &parentGraph, const MapNodesRef_t<T> &r_neighbourNodes)
    : m_data{data}, m_r_parentGraph{parentGraph}
{
    bool badNode = false;

    //we copy only the nodes from the same parent graph.
    //std::copy(r_neighbourNodes.begin(), r_neighbourNodes.end(), m_r_neighbourNodes.begin());
    m_r_neighbourNodes = MapNodesRef_t<T>(r_neighbourNodes.begin(), r_neighbourNodes.end());
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
bool Node<T>::m_connectNode(const Node<T> &withNode)
{

    const auto [it, success] = m_r_neighbourNodes.insert(std::make_pair(withNode.m_data, withNode));
    return success;
}

template<typename T>
bool Node<T>::m_disconnectNode(const Node<T> &fromNode)
{
    return (bool)m_r_neighbourNodes.erase(fromNode.m_data);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////PUBLIC//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
const T &Node<T>::getData() const
{
    return m_data;
}

