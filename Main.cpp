#include <iostream>
#include "clsBankClient.h"
#include "clsLoginScreen.h"

std::vector<clsBankClient> clsBankClient::_clientsList;

int main()
{

//  clsMainScreen::ShowMainMenue();
    while (true) {

        clsLoginScreen::ShowLoginScreen();
    }



    system("read");
    return 0;
}
