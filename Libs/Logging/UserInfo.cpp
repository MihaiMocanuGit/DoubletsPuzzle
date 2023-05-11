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
    madeMove();
}

const std::vector<std::string> &UserInfo::getWords() const
{
    return m_wordsUsed;
}

const std::string &UserInfo::getFinalWord() const
{
    return m_finalWord;
}

const std::string &UserInfo::getStartWord() const
{
    return m_startingWord;
}

std::string UserInfo::toString()
{
    std::string message = "Hi, " + m_username + " here are your stats:";
    message += "\n\t " + m_startingWord;
    for (const auto &word : m_wordsUsed)
        message += " ---> " + word ;
    message += "\n\t Moves made to get to the final word: " + std::to_string(m_movesMade);
    message += "\n\t The number of optimal moves would've been: " + std::to_string(m_optimalNoMoves);
    message += "\n\t The number of hints (1 costly hint = 4 cheap ones): " + std::to_string(m_hintsUsed);

    std::time_t startTime_t = std::chrono::system_clock::to_time_t(m_startingTime);
    message += "\n\t You started at: " + std::string(std::ctime(&startTime_t));

    std::time_t finishTime_t = std::chrono::system_clock::to_time_t(m_finishTime);
    message += "\t You finished at: " + std::string(std::ctime(&finishTime_t));

    std::chrono::duration<double> elapsed_seconds = m_finishTime - m_startingTime;
    message += "\t You played for: " + std::to_string(elapsed_seconds.count()) + " seconds";
    return message;
}

std::string UserInfo::csvFormat()
{
    std::string message = m_username;

    message += ",";
    for (const auto &word : m_wordsUsed)
        message += " " + word;

    message += ", " + std::to_string(m_movesMade) + ", " + std::to_string(m_hintsUsed);

    std::time_t startTime_t = std::chrono::system_clock::to_time_t(m_startingTime);
    std::string startTime = std::string(std::ctime(&startTime_t));
    startTime.pop_back();
    message += ", " + startTime;

    std::time_t finishTime_t = std::chrono::system_clock::to_time_t(m_finishTime);
    std::string finishTime = std::string(std::ctime(&finishTime_t));
    finishTime.pop_back();

    message += ", " + finishTime;


    return message;
}

const std::string &UserInfo::getUsername() const
{
    return m_username;
}
