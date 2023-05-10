#pragma once
#include "../UI/UI.h"
#include "../GraphGenerator/Generator.h"
#include "../Logging/UserInfo.h"


class Application
{
private:

    UserInfo user;
    static constexpr std::string WORD_PATH = "words_alpha.txt";

    Generator m_generator;

    int m_generateRandomNumber(int low, int high);
    bool m_generateStartingWord(int wordLength, MapNodes_t<std::string>::const_iterator &out_startWordIt);
    bool m_generateFinalWords(const MapNodes_t<std::string>::const_iterator &startWordIt,
                              Tools::Solution_t<std::string> &out_finalWords, int &out_maxDistance);
    bool m_askForDifficulty(int maxDistance, int &out_difficulty);

    bool m_initUser(std::string &out_startWord, std::string &out_finalWord, int &out_chainLength);

    bool m_askForHint(const std::string &currentWord);
    bool m_askAndValidateNextWord(const std::string &prevWord, std::string &out_nextWord);

    void m_printWordChain(const std::vector<std::string> &chain, const std::string &startWord,
                          const std::string &finalWord);

public:
    Application();

    void startAutomaticMode();
    void startPlayingMode();
    void startAnalyticsMode();

    //this should ask the user what mode to play
    void startApp();


};
