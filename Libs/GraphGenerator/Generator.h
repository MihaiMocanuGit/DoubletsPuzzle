#pragma once
#include <unordered_map>
#include <set>
#include <string>

#include "../Tools/BFS.h"
#include "../Graph/Graph.h"


class Generator
{
    usinggit status WordMap_t = std::unordered_map<std::string, std::unordered_set<std::string>>;
private:
    std::string m_startingWord;
    std::string m_filePath;

    WordMap_t m_map = {};
    Graph<std::string> m_graph = {};

    void m_generateStartingKey();
    void m_insertIntoMap();
public:
    Generator(std::string startingWord, std::string filePath);
    const Graph<std::string> &generateGraph();

    Tools::Solution_t<std::string> findPath(const std::string &searchedWord);




};
