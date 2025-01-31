#pragma once

#include <iostream>
#include "clsScreen.h"
#include "clsUser.h"
#include <iomanip>
#include "clsMainScreen.h"
#include "Global.h"
#include "clsDate.h"

class clsLoginScreen :protected clsScreen
{



private :

    

    static  void _Login()
    {
        short CounFaildLogin = 3;
        bool LoginFaild = false;

        string Username, Password;
        do
        {
            // cout << "Debug: Start of loop, CounFaildLogin = " << CounFaildLogin << endl;

            if (LoginFaild)
            {
                CounFaildLogin--;
                cout << "\nInvalid Username/Password!\n";
                cout << "You have " << CounFaildLogin << " Trials to login" << endl;
            }

            cout << "Enter Username? ";
            cin >> Username;

            cout << "Enter Password? ";
            cin >> Password;

            CurrentUser = clsUser::Find(Username, Password);

            LoginFaild = CurrentUser.IsEmpty();

            // cout << "Debug: After Find, LoginFaild = " << LoginFaild << endl;

            if (CounFaildLogin == 1) { // استخدام علامة المقارنة
                cout << "You are Locked out after 3 failed Trials " << endl;
                exit(0);
            }

            // cout << "Debug: End of loop, CounFaildLogin = " << CounFaildLogin << endl;

        } while (LoginFaild);
        
        CurrentUser.RegisterLogIn();
        clsMainScreen::ShowMainMenue();

    }

public:


    static void ShowLoginScreen()
    {
        system("clear");
        _DrawScreenHeader("\t  Login Screen");
        _Login();

    }

};
