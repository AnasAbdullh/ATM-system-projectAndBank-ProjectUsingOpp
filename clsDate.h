#pragma once
#include "clsString.h"
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class clsDate {

private:
  short _Day = 21;
  short _Month = 2;
  short _Year = 2004;

  enum enDateCompare { Before = -1, Equal = 0, After = 1 };

public:
  clsDate() {
    time_t t = time(0); // get time now
    tm *now = localtime(&t);
    _Year = now->tm_year + 1900;
    _Month = now->tm_mon + 1;
    _Day = now->tm_mday;
  }

  clsDate(string sDate) {

    vector<string> vDate;
    vDate = clsString::SplitString(sDate, "/");

    _Day = stoi(vDate[0]);
    _Month = stoi(vDate[1]);
    _Year = stoi(vDate[2]);
  }

  clsDate(short Day, short Month, short Year) {

    _Day = Day;
    _Month = Month;
    _Year = Year;
  }

  clsDate(short DateOrderInYear, short Year) {

    clsDate Date = GetDateFromDayOrderInYear(DateOrderInYear, Year);
    _Day = Date._Day;
    _Month = Date._Month;
    _Year = Date._Year;
  }

  static bool IsDate1EqualDate2(clsDate Date1, clsDate Date2) {
    return (Date1._Year == Date2._Year)
               ? ((Date1._Month == Date2._Month)
                      ? ((Date1._Day == Date2._Day) ? true : false)
                      : false)
               : false;
  }
  static bool IsDate1AfterDate2(clsDate Date1, clsDate Date2) {
    return (!IsDate1BeforeDate2(Date1, Date2) &&
            !IsDate1EqualDate2(Date1, Date2));
  }

  static enDateCompare CompareDates(clsDate Date1, clsDate Date2) {
    if (IsDate1BeforeDate2(Date1, Date2))
      return enDateCompare::Before;
    if (IsDate1EqualDate2(Date1, Date2))
      return enDateCompare::Equal;
    /* if (IsDate1AfterDate2(Date1,Date2))
    return enDateCompare::After;*/
    // this is faster
    return enDateCompare::After;
  }
  static bool isDateInPeriod(clsDate Date, clsDate Period) {
    return !(CompareDates(Date, Period) == enDateCompare::Before ||
             CompareDates(Date, Period) == enDateCompare::After);
  }

  static string NumberToText(int Number) {
    if (Number == 0) {
      return "";
    }
    if (Number >= 1 && Number <= 19) {
      string arr[] = {"",        "One",       "Two",      "Three",
                      "Four",    "Five",      "Six",      "Seven",
                      "Eight",   "Nine",      "Ten",      "Eleven",
                      "Twelve",  "Thirteen",  "Fourteen", "Fifteen",
                      "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
      return arr[Number] + " ";
    }
    if (Number >= 20 && Number <= 99) {
      string arr[] = {"",      "",      "Twenty",  "Thirty", "Forty",
                      "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
      return arr[Number / 10] + " " + NumberToText(Number % 10);
    }
    if (Number >= 100 && Number <= 199) {
      return "One Hundred " + NumberToText(Number % 100);
    }
    if (Number >= 200 && Number <= 999) {
      return NumberToText(Number / 100) + "Hundreds " +
             NumberToText(Number % 100);
    }
    if (Number >= 1000 && Number <= 1999) {
      return "One Thousand " + NumberToText(Number % 1000);
    }

    if (Number >= 2000 && Number <= 999999) {
      return NumberToText(Number / 1000) + "Thousands " +
             NumberToText(Number % 1000);
    }
    if (Number >= 1000000 && Number <= 1999999) {
      return "One Million " + NumberToText(Number % 1000000);
    }
    if (Number >= 2000000 && Number <= 999999999) {
      return NumberToText(Number / 1000000) + "Millions " +
             NumberToText(Number % 1000000);
    }
    if (Number >= 1000000000 && Number <= 1999999999) {
      return "One Billion " + NumberToText(Number % 1000000000);
    } else {
      return NumberToText(Number / 1000000000) + "Billions " +
             NumberToText(Number % 1000000000);
    }
  }

  string NumberToText() { return NumberToText(_Year); }

  static bool isLeapYear(short Year) {
    // if year is divisible by 4 AND not divisible by 100
    // OR if year is divisible by 400
    // then it is a leap year
    return (Year % 4 == 0 && Year % 100 != 0) || (Year % 400 == 0);
  }

  bool isLeapYear() { return isLeapYear(_Year); }

  static short NumberOfDaysInAYear(short Year) {
    return isLeapYear(Year) ? 366 : 365;
  }
  static short NumberOfHoursInAYear(short Year) {
    return NumberOfDaysInAYear(Year) * 24;
  }
  static int NumberOfMinutesInAYear(short Year) {
    return NumberOfHoursInAYear(Year) * 60;
  }
  static int NumberOfSecondsInAYear(short Year) {
    return NumberOfMinutesInAYear(Year) * 60;
  }

  short NumberOfDaysInAYear() { return NumberOfDaysInAYear(_Year); }

  short NumberOfHoursInAYear() { return NumberOfHoursInAYear(_Year); }

  short NumberOfMinutesInAYear() { return NumberOfMinutesInAYear(_Year); }

  short NumberOfSecondsInAYear() { return NumberOfSecondsInAYear(_Year); }

  static short ReadYear() {
    short Year;
    cout << "\nPlease enter a year to check? ";
    cin >> Year;
    return Year;
  }

  static short Month() {
    short month;
    cout << "\nPlease enter a Month to check? ";
    cin >> month;
    return month;
  }

  static short NumberOfDaysInAMonth(short Month, short Year) {
    if (Month < 1 || Month > 12)
      return 0;
    int NumberOfDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return (Month == 2) ? (isLeapYear(Year) ? 29 : 28)
                        : NumberOfDays[Month - 1];
  }

  short NumberOfDaysInAMonth() { return NumberOfDaysInAMonth(_Month, _Year); }

  static short DayOfWeekOrder(short Day, short Month, short Year) {
    short a, y, m;
    a = (14 - Month) / 12;
    y = Year - a;
    m = Month + (12 * a) - 2;
    // Gregorian:
    // 0:sun, 1:Mon, 2:Tue...etc
    return (Day + y + (y / 4) - (y / 100) + (y / 400) + ((31 * m) / 12)) % 7;
  }
  static string DayShortName(short DayOfWeekOrder) {
    string arrDayNames[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    return arrDayNames[DayOfWeekOrder];
  }

  short DayOfWeekOrder() { return DayOfWeekOrder(_Day, _Month, _Year); }

  string DayShortName() { return DayShortName(_Day); }

  static string MonthShortName(short MonthNumber) {
    string Months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                         "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    return (Months[MonthNumber - 1]);
  }

  string MonthShortName() { return MonthShortName(_Month); }

  static void PrintMonthCalendar(short Month, short Year) {
    int NumberOfDays;
    // Index of the day from 0 to 6
    int current = DayOfWeekOrder(1, Month, Year);
    NumberOfDays = NumberOfDaysInAMonth(Month, Year);
    // Print the current month name
    printf("\n _______________%s_______________\n\n",
           MonthShortName(Month).c_str());
    // Print the columns
    printf(" Sun Mon Tue Wed Thu Fri Sat\n");
    // Print appropriate spaces
    int i;
    for (i = 0; i < current; i++)
      printf(" ");
    for (int j = 1; j <= NumberOfDays; j++) {
      printf("%5d", j);
      if (++i == 7) {
        i = 0;
        printf("\n");
      }
    }
    printf("\n _________________________________\n");
  }

  static void PrintYearCalendar(int Year) {
    printf("\n _________________________________\n\n");
    printf(" Calendar - %d\n", Year);
    printf(" _________________________________\n");
    for (int i = 1; i <= 12; i++) {
      PrintMonthCalendar(i, Year);
    }
    return;
  }

  void PrintMonthCalendar() { PrintMonthCalendar(_Month, _Year); }

  void PrintYearCalendar() { PrintYearCalendar(_Year); }

  static short NumberOfDaysFromTheBeginingOfTheYear(short Day, short Month,
                                                    short Year) {
    short TotalDays = 0;
    for (int i = 1; i <= Month - 1; i++) {
      TotalDays += NumberOfDaysInAMonth(i, Year);
    }
    TotalDays += Day;
    return TotalDays;
  }

  short NumberOfDaysFromTheBeginingOfTheYear() {
    return NumberOfDaysFromTheBeginingOfTheYear(_Day, _Month, _Year);
  }

  static clsDate GetDateFromDayOrderInYear(short DateOrderInYear, short Year) {
    clsDate Date;
    short RemainingDays = DateOrderInYear;
    short MonthDays = 0;
    Date._Year = Year;
    Date._Month = 1;
    while (true) {
      MonthDays = NumberOfDaysInAMonth(Date._Month, Year);
      if (RemainingDays > MonthDays) {
        RemainingDays -= MonthDays;
        Date._Month++;
      } else {
        Date._Day = RemainingDays;
        break;
      }
    }
    return Date;
  }

  clsDate GetDateFromDayOrderInYear() {
    return GetDateFromDayOrderInYear(
        NumberOfDaysFromTheBeginingOfTheYear(_Day, _Month, _Year), _Year);
  }

  static void DateAddDays(short Days, clsDate Date) {
    short RemainingDays = Days + NumberOfDaysFromTheBeginingOfTheYear(
                                     Date._Day, Date._Month, Date._Year);
    short MonthDays = 0;
    Date._Month = 1;
    while (true) {
      MonthDays = NumberOfDaysInAMonth(Date._Month, Date._Year);
      if (RemainingDays > MonthDays) {
        RemainingDays -= MonthDays;
        Date._Month++;
        if (Date._Month > 12) {
          Date._Month = 1;
          Date._Year++;
        }
      } else {
        Date._Day = RemainingDays;
        break;
      }
    }
  }

  void DateAddDays() { DateAddDays(_Day, *this); }

  static bool IsDate1BeforeDate2(clsDate Date1, clsDate Date2) {
    return (Date1._Year < Date2._Year)
               ? true
               : ((Date1._Year == Date2._Year)
                      ? (Date1._Month < Date2._Month
                             ? true
                             : (Date1._Month == Date2._Month
                                    ? Date1._Day < Date2._Day
                                    : false))
                      : false);
  }

  bool IsDate1BeforeDate2(clsDate Date2) {
    return IsDate1BeforeDate2(*this, Date2);
  }

  static clsDate IncreaseDateByOneDay(clsDate Date) {
    if (IsLastDayInMonth(Date)) {
      if (IsLastMonthInYear(Date._Month)) {
        Date._Month = 1;
        Date._Day = 1;
        Date._Year++;
      } else {
        Date._Day = 1;
        Date._Month++;
      }
    } else {
      Date._Day++;
    }
    return Date;
  }

  static bool IsLastDayInMonth(clsDate Date) {
    return (Date._Day == NumberOfDaysInAMonth(Date._Month, Date._Year));
  }

  static bool IsLastMonthInYear(short Month) { return (Month == 12); }

  static bool IsValidDate(clsDate Date) {
    return (Date._Month < 12 && IsLastDayInMonth(Date));
  }

  void Print() { cout << _Day << "/" << _Month << "/" << _Year << endl; }
  
  static string DateToString(clsDate Date)
	{
		return  to_string(Date._Day) + "/" + to_string(Date._Month) + "/" + to_string(Date._Year);
	}

    string DateToString()
	{
		return  DateToString(*this);
	}
  
  
  
    
    static string GetTimeNow() {
    // الحصول على الوقت الحالي
    time_t t = time(0);

    // تحويل الوقت إلى بنية tm للتوقيت المحلي
    tm* localTime = localtime(&t);

    // استخراج الساعة والدقائق والثواني
    int hour = localTime->tm_hour;   // الساعة
    int min = localTime->tm_min;     // الدقائق
    int sec = localTime->tm_sec;     // الثواني

    // تحويل الوقت إلى سلسلة نصية
    string timeString = 
                          to_string(hour) + ":" 
                        + to_string(min) + ":" 
                        + to_string(sec);

    return timeString;
  }
  
  static string GetSystemDateTimeString()
    {
        //system datetime string
        time_t t = time(0);
        tm* now = localtime(&t);

        short Day, Month, Year,Hour,Minute,Second;

        Year = now->tm_year + 1900;
        Month = now->tm_mon + 1;
        Day = now->tm_mday;
        Hour = now->tm_hour;
        Minute = now->tm_min;
        Second = now->tm_sec;

        return (to_string(Day) + "/" + to_string(Month) + "/"
                + to_string(Year) + " - "
                + to_string(Hour) + ":" + to_string(Minute)
                + ":" + to_string(Second));

    }
    
    
  
};
