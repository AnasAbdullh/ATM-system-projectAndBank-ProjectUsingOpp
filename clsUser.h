#pragma once
#include <iostream>
#include <string>
#include "clsPerson.h"
#include "clsString.h"
#include <vector>
#include <fstream>
#include "clsDate.h"
#include "clsUtil.h"

//#include "Global.h"
using namespace std;
class clsUser : public clsPerson
{
  
  private:

    enum enMode { EmptyMode = 0, UpdateMode = 1, AddNewMode = 2 };
    enMode _Mode;
    string _UserName;
    string _Password;
    int _Permissions;
    string _DateAndTime;
    bool _MarkedForDelete = false;
    
        

    static clsUser  _ConvertLinetoUserObject(const std::string &Line,
                             const std::string &Seperator = "#//#") {
    std::vector<std::string> vUserData =
        clsString::SplitString(Line, Seperator);

    if (vUserData.size() != 7) {
      return _GetEmptyUserObject();
    }

    try {
      return clsUser(enMode::UpdateMode, vUserData[0], vUserData[1], vUserData[2],
            vUserData[3], vUserData[4], clsUtil::DecryptText(vUserData[5],2), stoi(vUserData[6]));

    } catch (const std::invalid_argument &e) {
      std::cerr << "Invalid argument: " << e.what() << std::endl;
      return _GetEmptyUserObject();
    }
  }
  
  
   
    static string _ConverUserObjectToLine(clsUser User, string Seperator = "#//#")
    {

        string UserRecord = "";
        UserRecord += User._FirstName + Seperator;
        UserRecord += User._LastName + Seperator;
        UserRecord += User._Email + Seperator;
        UserRecord += User._Phone + Seperator;
        UserRecord += User._UserName + Seperator;
        UserRecord += clsUtil::EncryptText(User._Password,2) + Seperator;
        UserRecord += to_string(User._Permissions);

        return UserRecord;

    }
    
    string _PrepareLogInRecord( string Seperator = "#//#")
    {
        string LoginRecord = "";
        LoginRecord += clsDate::GetSystemDateTimeString() + Seperator;
        LoginRecord += GetUserName() + Seperator;
        LoginRecord += clsUtil::EncryptText(GetPassword(),2) + Seperator;
        LoginRecord += to_string(GetPermissions());
        return LoginRecord;
    }

    static  vector <clsUser> _LoadUsersDataFromFile()
    {

        vector <clsUser> vUsers;

        fstream MyFile;
        MyFile.open("/data/data/com.ikou.cppcoding/files/code_project/Hello_World/src/Users.txt", ios::in);//read Mode

        if (MyFile.is_open())
        {

            string Line;


            while (getline(MyFile, Line))
            {

                clsUser User = _ConvertLinetoUserObject(Line);

                vUsers.push_back(User);
            }

            MyFile.close();

        }

        return vUsers;

    }

    static void _SaveUsersDataToFile(vector <clsUser> vUsers)
    {

        fstream MyFile;
        MyFile.open("/data/data/com.ikou.cppcoding/files/code_project/Hello_World/src/Users.txt", ios::out);//overwrite

        string DataLine;

        if (MyFile.is_open())
        {

            for (clsUser U : vUsers)
            {
                if (U.MarkedForDeleted() == false)
                {
                    //we only write records that are not marked for delete.  
                    DataLine = _ConverUserObjectToLine(U);
                    MyFile << DataLine << endl;

                }

            }

            MyFile.close();

        }

    }

    void _Update()
    {
        vector <clsUser> _vUsers;
        _vUsers = _LoadUsersDataFromFile();

        for (clsUser& U : _vUsers)
        {
            if (U._UserName == _UserName)
            {
                U = *this;
                break;
            }

        }

        _SaveUsersDataToFile(_vUsers);

    }

    void _AddNew()
    {

        _AddDataLineToFile(_ConverUserObjectToLine(*this));
    }

    void _AddDataLineToFile(string  stDataLine)
    {
        fstream MyFile;
        MyFile.open("/data/data/com.ikou.cppcoding/files/code_project/Hello_World/src/Users.txt", ios::out | ios::app);

        if (MyFile.is_open())
        {

            MyFile << stDataLine << endl;

            MyFile.close();
        }

    }

    static clsUser _GetEmptyUserObject()
    {
        return clsUser(enMode::EmptyMode, "", "", "", "", "", "", 0);
    }

public:

    enum enPermissions {
        eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4,
        pUpdateClients = 8, pFindClient = 16, pTranactions = 32, pManageUsers = 64, pLoginRegister = 128,pCurrencyExhange = 256,
    };

    clsUser(enMode Mode, string FirstName, string LastName,
        string Email, string Phone, string UserName, string Password,
        int Permissions) :
        clsPerson(FirstName, LastName, Email, Phone)

    {
        _Mode = Mode;
        _UserName = UserName;
        _Password = Password;
        _Permissions = Permissions;
    }
    
    
    struct  stLoginRegisterRecord {

    string DateTime;

    string UserName;

    string Password;

    int Permissions;

    };
    
    
    static stLoginRegisterRecord _ConvertLinetoLoginRegister(const std::string &Line, const std::string &Seperator = "#//#") {
    stLoginRegisterRecord Record;

    std::vector<std::string> vRegisterUser = clsString::SplitString(Line, Seperator);
    
    if (vRegisterUser.size() >= 4) {
        Record.DateTime = vRegisterUser[0];
        Record.UserName = vRegisterUser[1];
        Record.Password = clsUtil::DecryptText(vRegisterUser[2],2);
        Record.Permissions = stoi(vRegisterUser[3]);
    }

    return Record; // النوع العائد متوافق مع تعريف الدالة.
    }

    bool IsEmpty()
    {
        return (_Mode == enMode::EmptyMode);
    }

    bool MarkedForDeleted()
    {
        return _MarkedForDelete;
    }

  const string GetUserName() const 
    {
        return _UserName;
    }

    void SetUserName(string UserName)
    {
        _UserName = UserName;
    }

    void SetPassword(string Password)
    {
        _Password = Password;
    }

    const string GetPassword() const 
    {
        return _Password;
    }
    
    void SetPermissions(int Permissions)
    {
        _Permissions = Permissions;
    }

   const  int GetPermissions() const 
    {
        return _Permissions;
    }
    
    static clsUser Find(string UserName)
    {
        fstream MyFile;
        MyFile.open("/data/data/com.ikou.cppcoding/files/code_project/Hello_World/src/Users.txt", ios::in);//read Mode

        if (MyFile.is_open())
        {
            string Line;
            while (getline(MyFile, Line))
            {
                clsUser User = _ConvertLinetoUserObject(Line);
                if (User.GetUserName() == UserName)
                {
                    MyFile.close();
                    return User;
                }
            }

            MyFile.close();

        }

        return _GetEmptyUserObject();
    }

    static clsUser Find(string UserName, string Password)
    {

        fstream MyFile;
        MyFile.open("/data/data/com.ikou.cppcoding/files/code_project/Hello_World/src/Users.txt", ios::in);//read Mode

        if (MyFile.is_open())
        {
            string Line;
            while (getline(MyFile, Line))
            {
                clsUser User = _ConvertLinetoUserObject(Line);
                if (User.GetUserName() == UserName && User.GetPassword() == Password)
                {
                    MyFile.close();
                    return User;
                }

            }

            MyFile.close();

        }
        return _GetEmptyUserObject();
    }

    enum enSaveResults { svFaildEmptyObject = 0, svSucceeded = 1, svFaildUserExists = 2 };

    enSaveResults Save()
    {

        switch (_Mode)
        {
        case enMode::EmptyMode:
        {
            if (IsEmpty())
            {
                return enSaveResults::svFaildEmptyObject;
            }
        }

        case enMode::UpdateMode:
        {
            _Update();
            return enSaveResults::svSucceeded;

            break;
        }

        case enMode::AddNewMode:
        {
            //This will add new record to file or database
            if (clsUser::IsUserExist(_UserName))
            {
                return enSaveResults::svFaildUserExists;
            }
            else
            {
                _AddNew();
                //We need to set the mode to update after add new
                _Mode = enMode::UpdateMode;
                return enSaveResults::svSucceeded;
            }

            break;
        }
        }

    }

    static bool IsUserExist(string UserName)
    {

        clsUser User = clsUser::Find(UserName);
        return (!User.IsEmpty());
    }

    bool Delete()
    {
        vector <clsUser> _vUsers;
        _vUsers = _LoadUsersDataFromFile();

        for (clsUser& U : _vUsers)
        {
            if (U.GetUserName() == _UserName)
            {
                U._MarkedForDelete = true;
                break;
            }

        }

        _SaveUsersDataToFile(_vUsers);

        *this = _GetEmptyUserObject();

        return true;

    }

    static clsUser GetAddNewUserObject(string UserName)
    {
        return clsUser(enMode::AddNewMode, "", "", "", "", UserName, "", 0);
    }

    static vector <clsUser> GetUsersList()
    {
        return _LoadUsersDataFromFile();
    }
    
    bool CheckAccessPermission(enPermissions Permission)
    {
        if (this->GetPermissions() == enPermissions::eAll)
            return true;

        if ((Permission & this->GetPermissions()) == Permission)
            return true;
        else
            return false;

    }
    
    
    void RegisterLogIn()
    {

        string stDataLine = _PrepareLogInRecord();

        fstream MyFile;
        MyFile.open("/data/data/com.ikou.cppcoding/files/code_project/Hello_World/src/LoginUsers.txt", ios::out | ios::app);

        if (MyFile.is_open())
        {

            MyFile << stDataLine << endl;

            MyFile.close();
        }

    }
    
    static vector <stLoginRegisterRecord> GetLoginRegisterList() {
      
      vector <stLoginRegisterRecord> vloginRegisterRecord;
      
      fstream MyFile;
        MyFile.open("/data/data/com.ikou.cppcoding/files/code_project/Hello_World/src/LoginUsers.txt", ios::in);//read Mode

        if (MyFile.is_open())
        {

            string Line;


            while (getline(MyFile, Line))
            {

                stLoginRegisterRecord Register = _ConvertLinetoLoginRegister(Line);

                vloginRegisterRecord.push_back(Register);
            }

            MyFile.close();

        }
        
        
        return vloginRegisterRecord;
      
      
      
    }
    

};
