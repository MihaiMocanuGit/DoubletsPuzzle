#pragma once
#include <unordered_map>
#include <set>
#include <string>

#include "../Tools/BFS.h"
#include "../Graph/Graph.h"


class Generator
{
    using WordMap_t = std::unordered_map<std::string, std::unordered_set<std::string>>;
private:
    std::string m_filePath;
    unsigned int m_noLetters = 0;

    WordMap_t m_map = {};
    Graph<std::string> m_graph = {};

    void m_insertIntoMap();
    void m_extractGraph();
public:
    explicit Generator(std::string filePath);
    const Graph<std::string> &generateGraph(unsigned int noLetters);
    const Graph<std::string> &graph() const;
    Tools::Solution_t<std::string> findPath(const std::string &startingWord, const std::string &searchedWord);



};
