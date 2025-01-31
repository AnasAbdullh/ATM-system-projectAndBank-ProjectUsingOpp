#pragma once
#include <iostream>
#include "clsScreen.h"
#include "clsCurrency.h"
#include "clsInputValidate.h"
#include <iomanip>

class clsCurrencyCalculatorScreen :protected clsScreen

{
private:


    static void _PrintCurrency(clsCurrency Currency, string NameConvert = "Convert From") {
        cout << "\n" + NameConvert + ":\n";
        cout << "_____________________________\n";
        cout << "\nCountry    : " << Currency.Country();
        cout << "\nCode       : " << Currency.CurrencyCode();
        cout << "\nName       : " << Currency.CurrencyName();
        cout << fixed << setprecision(6); // تحديد دقة عرض الأرقام العشرية
        cout << "\nRate(1$) = : " << Currency.Rate(); // عرض Rate بدقة 6 خانات عشرية

        cout << "\n_____________________________\n";
    }

    static clsCurrency _FillClass(string CurrencyCode)
    {

        while (!clsCurrency::IsCurrencyExist(CurrencyCode))
        {
            cout << "\nCurrency is not found, choose another one: ";
            CurrencyCode = clsInputValidate::ReadString();
        }

        return clsCurrency::FindByCode(CurrencyCode);
    }

    static double _ReadAmountExchange()
    {
        cout << "\nEnter Amount to Exchange :";
        return clsInputValidate::ReadFloatNumber();
    }

public:

    static void ShowCurrencyCalculatorScreen()
    {

        char Answer = 'y';

        do {


            system("clear");

            _DrawScreenHeader("\tCurrency Calculator Screen");

            string CurrencyCode = "";

            cout << "\nPlease Enter Currency1 Code: ";
            CurrencyCode = clsInputValidate::ReadString();
            clsCurrency Currency1 = _FillClass(CurrencyCode) ;

            cout << "\nPlease Enter Currency2 Code: ";
            CurrencyCode = clsInputValidate::ReadString();
            clsCurrency Currency2 = _FillClass(CurrencyCode) ;

            int AmountExchange = _ReadAmountExchange();

            _PrintCurrency(Currency1);

            cout << "\n" << to_string(AmountExchange) << " " << Currency1.CurrencyCode() << "  = " << Currency1.ConvertCurrencyToUS(AmountExchange) << " " << "USA" << endl;

            double ResultConvertToDollar = Currency1.ConvertCurrencyToUS(AmountExchange);

            if(clsCurrency::CurrencyCalculator(Currency2))
            {
                cout <<"\nConverting from USD to:\n";
                _PrintCurrency(Currency2,"To");
                cout <<"\n" + to_string(AmountExchange) +" "+ Currency1.CurrencyCode() + "  = " << Currency2.ConvertFromOneCurrencyToAnother(ResultConvertToDollar) <<  + " " + Currency2.CurrencyCode() << endl;
            }

            cout << "\nDo you want to perform another calculation? y/n ?";
            cin >> Answer;


        } while (Answer == 'y' || Answer == 'Y');


    }
};
