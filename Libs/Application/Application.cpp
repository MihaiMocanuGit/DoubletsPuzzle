#include <random>
#include <fstream>

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
    while (not m_generateStartingWord(wordLength, startWordIt))
    {
        UI::askForInteger("How many letters shall the new word have?", wordLength);
    }


    //we are searching for the biggest chain of words, starting from the startWordIt. We ignore the chains bigger than 10
    Tools::Solution_t<std::string> finalWords;
    int maxDistance;

    //TODO: get rid of this copy paste logic somehow
    while (not m_generateFinalWords(startWordIt, finalWords, maxDistance))
    {
        UI::askForInteger("How many letters shall the new word have?", wordLength);
        while (not m_generateStartingWord(wordLength, startWordIt))
        {
            UI::askForInteger("How many letters shall the new word have?", wordLength);
        }

    }


    //we ask the user for the difficulty. We consider the difficulty level to be the length of the word chain.
    //A longer chain implies more perfect moves.
    int difficultyLength;
    m_askForDifficulty(maxDistance, difficultyLength);

    //we find all words that represent the end of a chain of the given length
    Tools::searchNodesAtDistance<std::string>(startWordIt, difficultyLength, finalWords);

    std::string startingWord = startWordIt->first;
    std::string finalWord = finalWords[m_generateRandomNumber(0, finalWords.size() - 1)]->first;

    user.initStartingInfo(startingWord, finalWord, difficultyLength, std::chrono::system_clock::now());

    out_startWord = startingWord;
    out_finalWord = finalWord;
    out_chainLength = difficultyLength;
    return true;
}

bool Application::m_askAndValidateNextWord(const std::string &prevWord, std::string &out_nextWord)
{
    std::string nextWord;
    UI::askForWord("Insert your next move", nextWord);

    if (prevWord.size() != nextWord.size())
    {
        UI::printMessage("Word length does not match");
        m_askForHint(prevWord);

        out_nextWord = nextWord;
        m_askAndValidateNextWord(prevWord, out_nextWord);
        return true;
    }

    if (m_generator.graph().findNode(nextWord) == m_generator.graph().cend())
    {
        UI::printMessage("Word does not exist");
        user.madeMove();
        m_askForHint(prevWord);

        out_nextWord = nextWord;
        m_askAndValidateNextWord(prevWord, out_nextWord);
        return true;
    }

    int noChanges = 0;
    for (unsigned int i = 0; i < prevWord.size(); ++i)
    {
        if (prevWord[i] != nextWord[i])
            noChanges++;

        if (noChanges >= 2)
        {
            UI::printMessage("You changed more than one letter");
            m_askForHint(prevWord);
            out_nextWord = nextWord;
            m_askAndValidateNextWord(prevWord, out_nextWord);
            return true;
        }
    }

    out_nextWord = nextWord;
    user.usedWord(nextWord);
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
    //TODO: decrease min difficulty to 2
    bool repeat;

    do
    {
        std::string startWord, finalWord;
        int chainLength;
        //this function will ask the user for options in the terminal.
        bool status = m_initUser(startWord, finalWord, chainLength);
        if (status == false)
        {
            repeat = true;
            continue;
        }
        std::string prevWord = startWord;


        UI::printMessage(startWord + " ---> ... ---> " + finalWord);
        bool gameFinished;
        do
        {
            std::string word;
            m_askAndValidateNextWord(prevWord, word);
            prevWord = word;

            UI::clear();

            m_printWordChain(user.getWords(), startWord, finalWord);

            gameFinished = (word == finalWord);
        } while (not gameFinished);

        user.finishTime(std::chrono::system_clock::now());

        UI::clear();
        UI::printMessage(user.toString());
        m_updateCsv();
        UI::askForYesNo("\nYou finished, do you want to play again?", repeat);
    } while (repeat);

}

void Application::m_printWordChain(const std::vector<std::string> &chain, const std::string &startWord,
                                   const std::string &finalWord)
{
    //TODO: maybe don't print redundant chains
    std::string message = startWord + " ---> ";
    for (const auto &word : chain)
        message += word + " ---> ";
    message += " ... ---> " + finalWord;

    UI::printMessage(message);
}

bool Application::m_askForHint(const std::string &currentWord)
{
    bool wantHint;
    UI::askForYesNo("Do you need a hint?", wantHint, false);

    if (wantHint)
    {
        Tools::Solution_t<std::string> chain = m_generator.findPath(currentWord, user.getFinalWord());
        std::string nextWord = chain.at(chain.size() - 2)->first;


        bool wantCostlyHint;
        UI::askForYesNo("Do want the costly hint ?", wantCostlyHint, false);

        std::cout << "Hint:\t";
        //nextWord and currentWord will be of the same size and differ in at most one letter
        for (unsigned int i = 0; i < currentWord.size(); ++i)
        {
            if (nextWord[i] == currentWord[i])
                std::cout<<currentWord[i];
            else
            {
                if (wantCostlyHint)
                {
                    std::cout << "\033[1;33m" << nextWord[i]  << "\033[0m";
                    user.usedHint(4);
                }
                else
                {
                    //TODO: Modify from toupper to another color
                    std::cout << "\033[1;35m" << currentWord[i] << "\033[0m";
                    user.usedHint(1);
                }

            }
        }
        std::cout << std::endl;
    }

    return true;
}

void Application::m_updateCsv()
{
    std::ofstream outfile;
    outfile.open(user.getUsername() + ".csv", std::ios_base::app);
    outfile << user.csvFormat() << '\n';
}


