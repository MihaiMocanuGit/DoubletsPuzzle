#pragma once
#include <string>
#include <functional>

#include "../Tools/BFS.h"

class UI
{
private:
    static constexpr std::string DEFAULT_YES = "[Y/n]";
    static constexpr std::string DEFAULT_NO = "[y/N]";

    static constexpr std::string INPUT_TEMPLATE = ">>> ";
    static void m_printInputTemplate();

public:
    UI() = default;

    static void printMessage(const std::string &message, bool newLineAfter = true);

    template <typename T>
    static void printSolution(const std::string &message, const Tools::Solution_t<T> &solution);

    static bool askForWord(const std::string &message, std::string &output,
                           const std::function< bool(const std::string &) > & = [](const std::string &){return true;});
    static bool askForYesNo(const std::string &message, bool &output, bool yesDefault = true);
    static bool askForInteger(const std::string &message, int &output,
                              const std::function< bool(int) >& validator = [](int){return true;});
};


template <typename T>
void UI::printSolution(const std::string &message, const  Tools::Solution_t<T> &solution)
{
    printMessage(message);

    if (solution.empty())
    {
        std::cout << "The given words could not be connected, try another pair." << std::endl;
        return;
    }

    std::cout << "Starting word = ";
    auto it = solution.crbegin();
    for (; it != std::prev(solution.crend()); ++it)
        std::cout << (*it)->first << " ----> ";
    std::cout << (*it)->first;
    std::cout << " = Final word" << std::endl;
}

