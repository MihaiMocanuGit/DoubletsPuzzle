#include "UI.h"

#include <iostream>
#include <limits>

void UI::m_printInputTemplate()
{
    std::cout << INPUT_TEMPLATE;
}

void UI::printMessage(const std::string &message, bool newLineAfter)
{
    std::cout << message;

    if (newLineAfter)
        std::cout << std::endl;
}

bool UI::askForWord(const std::string &message, std::string &output,
                    const std::function< bool(const std::string &) > &validator)
{
    printMessage(message);

    m_printInputTemplate();
    std::string result;
    std::cin >> result;

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (not validator(result))
    {
        printMessage("Invalid word, try again");
        m_printInputTemplate();
        std::cin >> result;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    output = result;
    return true;
}

bool UI::askForYesNo(const std::string &message, bool &output, bool yesDefault)
{
    if (yesDefault)
    {
        printMessage(message + DEFAULT_YES + " ", false);

        std::string option;
        std::cin.get();
        std::getline(std::cin, option);

        if (option.empty() or (option.size() == 1 and toupper(option[0]) == 'Y'))
            output = true;
        else
            output = false;
    }
    else
    {
        printMessage(message + DEFAULT_NO + " ", false);

        std::string option;
        std::getline(std::cin, option);

        if (option.size() == 1 and toupper(option[0]) == 'Y')
            output = true;
        else
            output = false;
    }

    std::cout << std::endl;
    return true;
}

bool UI::askForInteger(const std::string &message, int &output, const std::function< bool(int) > &validator)
{
    printMessage(message);

    m_printInputTemplate();
    int result;
    std::cin >> result;

    while (not validator(result))
    {
        printMessage("Invalid integer, try again");
        m_printInputTemplate();

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> result;

    }

    output = result;

    return true;
}

void UI::clear()
{
    std::cout << "\x1B[2J\x1B[H";
}





