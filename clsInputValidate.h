#pragma once
#include <iostream>
#include <string>
#include "clsString.h"
#include "clsDate.h"

class clsInputValidate
{

public:

   template <typename T> static bool IsNumberBetween(T Number, int From, T To)
    {
        if (Number >= From && Number <= To)
            return true;
        else
            return false;

    }

    template <typename T1, typename T2, typename T3>
static bool IsNumberBetween(T1 Number, T2 From, T3 To)
{
    return (static_cast<double>(Number) >= static_cast<double>(From) &&
            static_cast<double>(Number) <= static_cast<double>(To));
}

   template <typename T> static bool IsDateBetween(T Date, T From, T To)
    {
        //Date>=From && Date<=To
        if ((clsDate::IsDate1AfterDate2(Date, From) || clsDate::IsDate1EqualDate2(Date, From))
                &&
                (clsDate::IsDate1BeforeDate2(Date, To) || clsDate::IsDate1EqualDate2(Date, To))
           )
        {
            return true;
        }

        //Date>=To && Date<=From
        if ((clsDate::IsDate1AfterDate2(Date, To) || clsDate::IsDate1EqualDate2(Date, To))
                &&
                (clsDate::IsDate1BeforeDate2(Date, From) || clsDate::IsDate1EqualDate2(Date, From))
           )
        {
            return true;
        }

        return false;
    }


   template <typename T = int> static T ReadShortNumber(string ErrorMessage = "Invalid Number, Enter again\n")
{
    T Number;
    while (!(cin >> Number)) {
        cin.clear();  // تنظيف حالة الخطأ
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // تجاهل المدخلات السابقة
        cout << ErrorMessage;
    }
    return Number;
}

   template <typename T = int> static T ReadIntNumber(string ErrorMessage = "Invalid Number, Enter again\n")
    {
        T Number;
        while (!(cin >> Number)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << ErrorMessage;
        }
        return Number;
    }

    template <typename T = short> static T ReadShortNumberBetween(T From, T To, string ErrorMessage = "Number is not within range, Enter again:\n")
    {
        T Number = ReadShortNumber();

        while (!IsNumberBetween(Number, From, To))
        {
            cout << ErrorMessage;
            Number = ReadShortNumber();
        }
        return Number;
    }

    template <typename T = int> static T ReadIntNumberBetween(T From, T To, string ErrorMessage = "Number is not within range, Enter again:\n")
    {
        T Number = ReadIntNumber();

        while (!IsNumberBetween(Number, From, To))
        {
            cout << ErrorMessage;
            Number = ReadIntNumber();
        }
        return Number;
    }

    template <typename T = double> static T ReadFloatNumber(string ErrorMessage = "Invalid Number, Enter again\n")
    {
        T Number;
        while (!(cin >> Number)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << ErrorMessage;
        }
        return Number;
    }

    template <typename T = double> static T ReadFloatNumberBetween(T From, T To, string ErrorMessage = "Number is not within range, Enter again:\n")
    {
        T Number = ReadFloatNumber();

        while (!IsNumberBetween(Number, From, To)) {
            cout << ErrorMessage;
            Number = ReadDblNumber();
        }
        return Number;
    }

    template <typename T = double> static T ReadDblNumber(string ErrorMessage = "Invalid Number, Enter again\n")
    {
        T Number;
        while (!(cin >> Number)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << ErrorMessage;
        }
        return Number;
    }

    template <typename T = double> static T ReadDblNumberBetween(T From, T To, string ErrorMessage = "Number is not within range, Enter again:\n")
    {
        T Number = ReadDblNumber();

        while (!IsNumberBetween(Number, From, To)) {
            cout << ErrorMessage;
            Number = ReadDblNumber();
        }
        return Number;
    }

   template <typename T>  static bool IsValideDate(T Date)
    {
        return	clsDate::IsValidDate(Date);
    }

   template <typename T = string > static string ReadString()
    {
        T  S1="";
        // Usage of std::ws will extract allthe whitespace character
        getline(cin >> ws, S1);
        return S1;
    }


};
    
