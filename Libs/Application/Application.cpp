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
    unsigned long maxNeighbours = 0;
    auto maxIterator = GRAPH.cend();
    int maximumsFoundUntilStopping = 10;
    for (auto nodeIt = GRAPH.cbegin(); nodeIt != GRAPH.cend(); nodeIt++)
    {
        if (maximumsFoundUntilStopping <= 0)
            break;
        unsigned long noNeighbours = nodeIt->second.getNeighbours().size();
        if (noNeighbours >= maxNeighbours)
        {
            maxIterator = nodeIt;
            maxNeighbours = noNeighbours;
            maximumsFoundUntilStopping--;
        }
    }

    if (maxIterator == GRAPH.cend())
    {
        UI::printMessage("Could not find a word with so many letters!");
        return;
    }
    //we are searching for the biggest chain smaller than 10
    Tools::Solution_t<std::string> finalWords;
    int maxDistance = 10;
    do
    {
        Tools::searchNodesAtDistance<std::string>(maxIterator, maxDistance, finalWords);
        maxDistance--;
    } while (finalWords.empty());
    maxDistance++;

    if (maxDistance < 3)
    {
        UI::printMessage("Could not generate a link between two words of the given size that contains more"
                         " then 2 words, only found a link of " + std::to_string(maxDistance) + "!");
        return;
    }

    std::string messageAskDifficulty = "Choose difficulty, insert a number between 3 and " + std::to_string(maxDistance);
    messageAskDifficulty += "\nYour choice will represent the number of perfect moves you will have to do in order to get"
                            "to the final word";

    int length;
    UI::askForInteger(messageAskDifficulty, length, [&](int number)->bool{
        return number >= 3 and number <= maxDistance;
    });

    Tools::searchNodesAtDistance<std::string>(maxIterator, length, finalWords);

    std::string startingWord = maxIterator->first;
    std::string finalWord = finalWords[0]->first;

    std::cout << startingWord << '\t' << finalWord;
}

