#include <random>
#include "Application.h"
#include "../GraphGenerator/Generator.h"


Application::Application() : m_generator{Generator(WORD_PATH)}
{

}


int Application::m_generateRandomNumber(int low, int high)
{

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(low, high); // define the range

    return distr(gen); // generate numbers
}

bool Application::m_generateStartingWord(int wordLength, MapNodes_t<std::string>::const_iterator &out_startWordIt)
{

    const Graph<std::string> &GRAPH = m_generator.generateGraph(wordLength);

    int high = GRAPH.size();
    int low = 0;
    int randomIndex = m_generateRandomNumber(low, high);

    //we are searching for a node which has quite a bit of neighbours;
    //We won't stay to find the node with the most neighbours, we are going to stop after we found a certain
    //number of local maximums
    unsigned long maxNeighbours = 0;

    //the iterator for the node with the (almost) most neighbours
    auto maxIterator = GRAPH.cend();

    //we won't check all nodes, we will consider to have found one that is good enough if it was the 10th maximal node
    int maximumsFoundUntilStopping = 10;

    //we fix a random pivot, and we will search for a good starting word by starting at that pivot and going through the end
    auto pivotIt = GRAPH.cbegin();
    std::advance(pivotIt, randomIndex);
    for(auto nodeIt = pivotIt; nodeIt != GRAPH.cend(); nodeIt++)
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

    //if we didn't find a node that is good enough, we change the travel direction
    for(auto nodeIt = pivotIt; nodeIt != GRAPH.cbegin(); nodeIt--)
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


    out_startWordIt = maxIterator;
    if (maxIterator == GRAPH.cend())
    {
        UI::printMessage("Could not find a word with so many letters!");
        return false;
    }

    return true;
}

bool Application::m_generateFinalWords(const MapNodes_t<std::string>::const_iterator &startWordIt,
                                       Tools::Solution_t<std::string> &out_finalWords, int &out_maxDistance)
{
    out_maxDistance = 10;
    do
    {
        Tools::searchNodesAtDistance<std::string>(startWordIt, out_maxDistance, out_finalWords);
        out_maxDistance--;
    } while (out_finalWords.empty());
    out_maxDistance++;

    if (out_maxDistance < 3)
    {
        UI::printMessage("Could not generate a link between two words of the given size that contains more"
                         " then 2 words, only found a link of " + std::to_string(out_maxDistance) + "!");
        return false;
    }
    return true;
}

bool Application::m_askForDifficulty(int maxDistance, int &out_difficulty)
{
    std::string messageAskDifficulty = "Choose difficulty, insert a number between 3 and " + std::to_string(maxDistance);
    messageAskDifficulty += "\nYour choice will represent the number of perfect moves you will have to do in order to get"
                            " to the final word";

    int length;
    UI::askForInteger(messageAskDifficulty, length, [maxDistance](int number)->bool{
        return number >= 3 and number <= maxDistance;
    });

    out_difficulty = length;

    return true;
}

bool Application::m_initUser(std::string &out_startWord, std::string &out_finalWord, int &out_chainLength)
{
    //we ask the user for a username, it should be composed of only one word, no spaces allowed;
    std::string username;
    UI::askForWord("What's your username?", username);
    user = UserInfo(username);

    //we ask the user for the length of the words, and we generate a random starting word
    int wordLength;
    UI::askForInteger("How many letters shall the word have?", wordLength);
    MapNodes_t<std::string>::const_iterator startWordIt;
    if (not m_generateStartingWord(wordLength, startWordIt))
    {
        return false;
    }


    //we are searching for the biggest chain of words, starting from the startWordIt. We ignore the chains bigger than 10
    Tools::Solution_t<std::string> finalWords;
    int maxDistance;

    if (not m_generateFinalWords(startWordIt, finalWords, maxDistance))
    {
        return false;
    }


    //we ask the user for the difficulty. We consider the difficulty level to be the length of the word chain.
    //A longer chain implies more perfect moves.
    int difficultyLength;
    m_askForDifficulty(maxDistance, difficultyLength);

    //we find all words that represent the end of a chain of the given length
    Tools::searchNodesAtDistance<std::string>(startWordIt, difficultyLength, finalWords);

    std::string startingWord = startWordIt->first;
    std::string finalWord = finalWords[m_generateRandomNumber(0, finalWords.size())]->first;

    user.initStartingInfo(startingWord, finalWord, difficultyLength, std::chrono::system_clock::now());

    out_startWord = startingWord;
    out_finalWord = finalWord;
    out_chainLength = difficultyLength;
    return true;
}

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

    std::string startWord, finalWord;
    int chainLength;
    //this function will ask the user for options in the terminal.
    bool status = m_initUser(startWord, finalWord, chainLength);

}



