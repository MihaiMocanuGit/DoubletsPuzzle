#include "UserInfo.h"

UserInfo::UserInfo(std::string username) : m_username(std::move(username))
{

}

void UserInfo::initStartingInfo(const std::string &startingWord, const std::string &finalWord, int optimalNoMoves,
                                const AppClock_t &start)
{
    if (startingWord.size() != finalWord.size())
        throw std::logic_error("Word size does not match!");

    m_startingWord = startingWord;
    m_finalWord = finalWord;
    m_optimalNoMoves = optimalNoMoves;

    m_startingTime = start;
}

void UserInfo::usedHint(int noUsedHints)
{
    m_hintsUsed += noUsedHints;
}

void UserInfo::madeMove(int noMadeMoves)
{
    m_movesMade += noMadeMoves;
}

void UserInfo::finishTime(const AppClock_t &finish)
{
    m_finishTime = finish;
}

void UserInfo::usedWord(const std::string &usedWord)
{
    if (usedWord.size() != m_startingWord.size())
        throw std::logic_error("Size of words does not match!");

    m_wordsUsed.push_back(usedWord);
}
