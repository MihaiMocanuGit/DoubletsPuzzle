#include "UI.h"

#include <iostream>

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

bool UI::askForWord(const std::string &message, std::string &output)
{
    printMessage(message);

    m_printInputTemplate();
    std::cin >> output;

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

bool UI::askForInteger(const std::string &message, int &output)
{
    printMessage(message);

    m_printInputTemplate();
    //TODO: Validate output to be a number
    std::cin >> output;

    return true;
}



