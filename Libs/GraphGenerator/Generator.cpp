#include "Generator.h"


#include <fstream>
#include <utility>
#include <iostream>

void Generator::m_insertIntoMap()
{
    std::ifstream inputFile;
    inputFile.open(m_filePath);
    if (inputFile.fail())
        throw std::invalid_argument("Failed to open file!");

    const int WORD_SIZE = m_startingWord.size();

    std::string word;
    while(std::getline(inputFile, word))
    {
        //we remove the '\r';
        if (word[word.size() - 1] == '\r')
            word.pop_back();

        if (word.size() == WORD_SIZE)
        {
            for (int i = 0; i < WORD_SIZE; ++i)
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
}


Generator::Generator(std::string startingWord, std::string filePath)
                : m_startingWord{std::move(startingWord)}, m_filePath{std::move(filePath)}
{

}

void Generator::m_generateStartingKey()
{
    for (int i = 0; i < m_startingWord.size(); ++i)
    {
        std::string wildCardWord = m_startingWord;
        wildCardWord[i] = '*';

        m_map.insert({wildCardWord, {m_startingWord}});
    }

}

const Graph<std::string> &Generator::generateGraph()
{
    //m_generateStartingKey();
    m_insertIntoMap();

    erase_if(m_map,[](const auto &pair)->bool{ return pair.second.size() <= 1;});


    m_graph = Graph<std::string>();
    for (const auto &pair : m_map)
    {
        MapNodesPtr_t<std::string> neighboursPtr = {};
        for (const auto &elem : pair.second)
        {
            MapNodes_t<std::string>::iterator itCurrent = m_graph.findNode(elem);
            if (itCurrent == m_graph.end())
                itCurrent = m_graph.addNode(elem, neighboursPtr);
            else
            {
                for (const auto &neighbour : neighboursPtr)
                {
                    m_graph.connectNodes(itCurrent, neighbour.second->getIterator());
                }

            }
            neighboursPtr.insert({elem, &(itCurrent->second)});

        }
    }
    return m_graph;
}

Tools::Solution_t <std::string> Generator::findPath(const std::string &searchedWord)
{
    Tools::Solution_t<std::string> solution;
    auto itStart = m_graph.findNode(m_startingWord);
    Tools::BFS(itStart, searchedWord, solution);
    return solution;
}
