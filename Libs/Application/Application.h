#pragma once
#include "../UI/UI.h"
#include "../Logging/UserInfo.h"


class Application
{
private:

    UserInfo user;
public:
    Application() = default;

    void startAutomaticMode();
    void startPlayingMode();


};
