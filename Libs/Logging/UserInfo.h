#pragma once

#include <string>
#include <vector>
#include <chrono>

using AppClock_t = std::chrono::time_point<std::chrono::system_clock>;
class UserInfo
{
private:
    std::string m_username;

    int m_hintsUsed = 0;
    std::string m_startingWord;
    std::string m_finalWord;
    std::vector<std::string> m_wordsUsed;
    int m_movesMade = 0;
    int m_optimalNoMoves;

    AppClock_t m_startingTime;
    AppClock_t m_finishTime;

public:
    UserInfo() = default;
    explicit UserInfo(std::string username);


    void initStartingInfo(const std::string &startingWord, const std::string &finalWord,
                          int optimalNoMoves, const AppClock_t &start);

    void usedHint(int noUsedHints = 1);
    void madeMove(int noMadeMoves = 1);
    void usedWord(const std::string &usedWord);

    void finishTime(const AppClock_t &finish);
};
