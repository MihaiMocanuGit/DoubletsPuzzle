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

void Application::startPlayingMode()
{
    std::string username;
    UI::askForWord("What's your username?", username);

    int wordLength;
    UI::askForInteger("How many letters shall the word have?", wordLength);

    Generator generator("words_alpha.txt");

    const Graph<std::string> &GRAPH = generator.generateGraph(wordLength);
    Tools::Solution_t<std::string> searchedChainLength;

    //we are searching for a node which has quite a bit of neighbours;
    //We won't stay to find the node with the most neighbours, we are going to stop after we found a certain
    //number of local maximums
    int maxNeighbours = -1;
    MapNodes_t<std::string>::const_iterator maxIterator;
    int maximumsFoundUntilStopping = 10;
    for (auto nodeIt = GRAPH.begin(); nodeIt != GRAPH.end(); nodeIt++)
    {
        if (maximumsFoundUntilStopping <= 0)
            break;
        if (nodeIt->second.getNeighbours().size() > maxNeighbours)
        {
            maxIterator = nodeIt;
            maxNeighbours = nodeIt->second.getNeighbours().size();
            maximumsFoundUntilStopping--;
        }
    }

    Tools::Solution_t<std::string> finalWords;
    int maxDistance = 10;
    do
    {
        //Tools::searchNodesAtDistance<std::string>(maxIterator, maxDistance, finalWords);
        maxDistance--;
    } while (finalWords.empty());


}

