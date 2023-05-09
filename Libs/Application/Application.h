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
    bool m_generateStartingWord(int wordLength, MapNodes_t<std::string>::const_iterator &out_startWordIt);
    bool m_generateFinalWords(const MapNodes_t<std::string>::const_iterator &startWordIt,
                              Tools::Solution_t<std::string> &out_finalWords, int &out_maxDistance);
    bool m_askForDifficulty(int maxDistance, int &out_difficulty);
public:
    Application();

    void startAutomaticMode();
    void startPlayingMode();


};
