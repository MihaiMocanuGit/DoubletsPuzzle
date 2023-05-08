#include "Application.h"
#include "../GraphGenerator/Generator.h"




void Application::startAutomaticMode()
{

    UI::printMessage("Started Automatic Mode!");

    bool continueGame;
    do
    {

        std::string startingWord, finalWord;
        bool status1 = UI::askForWord("Insert the starting word:", startingWord);
        bool status2 = UI::askForWord("Insert the final word, they have to have the same size:", finalWord);

        while (startingWord.size() != finalWord.size())
            status2 = UI::askForWord("The size does not match, try another", finalWord);

        Generator generator("words_alpha.txt");

        generator.generateGraph(startingWord.size());

        //https://lewiscarrollresources.net/doublets/puzzles.html
        Tools::Solution_t<std::string> solution = generator.findPath(startingWord, finalWord);
        UI::printSolution<std::string>("", solution);

        UI::askForYesNo("Continue game?", continueGame);
    }while(continueGame);

    UI::printMessage("Goodbye!");
}

