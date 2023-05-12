#include "Generator.h"


#include <fstream>
#include <utility>
#include <iostream>


Generator::Generator(std::string filePath) : m_filePath{std::move(filePath)}
{

}

void Generator::m_insertIntoMap()
{
    std::ifstream inputFile;
    inputFile.open(m_filePath);
    if (inputFile.fail())
        throw std::invalid_argument("Failed to open file!");

    const unsigned int WORD_SIZE = m_noLetters;

    std::string word;
    while(std::getline(inputFile, word))
    {
        //we remove the '\r';
        if (word[word.size() - 1] == '\r')
            word.pop_back();

        if (word.size() == WORD_SIZE)
        {
            for (unsigned int i = 0; i < WORD_SIZE; ++i)
            {
                std::string wildCardWord = word;
                wildCardWord[i] = '*';

                auto it = m_map.find(wildCardWord);
                if (it == m_map.end())
                    m_map.insert(it, {wildCardWord,{word}});
                else
                    it->second.insert(word);
            }

        }

    }

    inputFile.close();
}



void Generator::m_extractGraph()
{
    m_graph = Graph<std::string>();

    for (const auto &pair: m_map)
    {
        MapNodesPtr_t<std::string> neighboursPtr = {};
        for (const auto &elem: pair.second)
        {
            MapNodes_t<std::string>::iterator itCurrent = m_graph.findNode(elem);
            if (itCurrent == m_graph.end())
                itCurrent = m_graph.addNode(elem, neighboursPtr);
            else
                for (const auto &neighbour: neighboursPtr)
                    m_graph.connectNodes(itCurrent, neighbour.second->getIterator());

            neighboursPtr.insert({elem, &(itCurrent->second)});

        }
    }
}

const Graph<std::string> &Generator::generateGraph(unsigned int noLetters)
{
    if(m_noLetters != noLetters)
    {
        m_noLetters = noLetters;

        //we are clearing m_map by swapping its elements with an empty map
        WordMap_t().swap(m_map);
        m_insertIntoMap();

        erase_if(m_map, [](const auto &pair) -> bool
        { return pair.second.size() <= 1; });

        m_extractGraph();
    }


    return m_graph;
}

Tools::Solution_t <std::string> Generator::findPath(const std::string &startingWord, const std::string &searchedWord)
{
    if (startingWord.size() != searchedWord.size() or startingWord.size() != m_noLetters)
        throw std::runtime_error("wrong word size");

    Tools::Solution_t<std::string> solution = {};
    auto itStart = m_graph.findNode(startingWord);

    if (itStart != m_graph.end())
        Tools::BFS(itStart, searchedWord, solution);
    return solution;
}

const Graph<std::string> &Generator::graph() const
{
    return m_graph;
}

