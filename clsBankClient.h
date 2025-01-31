#pragma once
#include "clsPerson.h"
#include "clsString.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "clsDate.h"
#include "Global.h"
//#include "clsDeleteClientScreen.h"

class clsBankClient : public clsPerson {
private:
    enum class enMode { EmptyMode = 0, UpdateMode = 1, AddNewMode = 2 };
    enMode _Mode;
    std::string _AccountNumber;
    std::string _PinCode;
    float _AccountBalance;
    bool _MarkedForDelete = false;

    static std::vector<clsBankClient> _clientsList;

    static clsBankClient
    _ConvertLinetoClientObject(const std::string &Line,
                               const std::string &Seperator = "#//#") {
        std::vector<std::string> vClientData =
            clsString::SplitString(Line, Seperator);

        if (vClientData.size() != 7) {
            return _GetEmptyClientObject();
        }

        try {
            return clsBankClient(enMode::UpdateMode, vClientData[0], vClientData[1],
                                 vClientData[2], vClientData[3], vClientData[4],
                                 vClientData[5], std::stod(vClientData[6]));
        } catch (const std::invalid_argument &e) {
            std::cerr << "Invalid argument: " << e.what() << std::endl;
            return _GetEmptyClientObject();
        }
    }

    static std::string
    _ConverClientObjectToLine(const clsBankClient &Client,
                              const std::string &Seperator = "#//#") {
        std::string stClientRecord = "";
        stClientRecord += Client._FirstName + Seperator;
        stClientRecord += Client._LastName + Seperator;
        stClientRecord += Client._Email + Seperator;
        stClientRecord += Client._Phone + Seperator;
        stClientRecord += Client.AccountNumber() + Seperator;
        stClientRecord += Client._PinCode + Seperator;
        stClientRecord += std::to_string(Client._AccountBalance);
        
        return stClientRecord;
    }
    
    string _PrepareTransferRecord(float Amount,clsBankClient DestinationClient,  string Seperator = "#//#")
    {
        string LoginRecord = "";
        LoginRecord += clsDate::GetSystemDateTimeString() + Seperator;
        LoginRecord += AccountNumber() + Seperator;
        LoginRecord += DestinationClient.AccountNumber() + Seperator;
        LoginRecord += to_string(Amount) + Seperator;
        LoginRecord += to_string(GetAccountBalance()) + Seperator;
        LoginRecord += to_string(DestinationClient.GetAccountBalance()) + Seperator;
        LoginRecord += CurrentUser.GetUserName();
        
        
        
        return LoginRecord;
    }

    static void _LoadClientsDataFromFile() {
        _clientsList.clear();

        std::ifstream MyFile("/data/data/com.ikou.cppcoding/files/code_project/"
                             "Hello_World/src/Clients.txt",ios::in);
        if (MyFile.is_open()) {
            std::string Line;
            while (std::getline(MyFile, Line)) {
                _clientsList.push_back(_ConvertLinetoClientObject(Line));

            }
        } else {
            std::cerr << "Error: Unable to open file for reading!" << std::endl;
        }
        MyFile.close();
    }

    static void
    _SaveClientsDataToFile(const std::vector<clsBankClient> &clients) {
        std::ofstream MyFile("/data/data/com.ikou.cppcoding/files/code_project/"
                             "Hello_World/src/Clients.txt",
                             ios::out);
        if (MyFile.is_open()) {
            for (const auto &client : clients) {
                MyFile << _ConverClientObjectToLine(client) << std::endl;

            }
        } else {
            std::cerr << "Error: Unable to open file for writing!" << std::endl;
        }

        MyFile.close();
    }
    
    void _RegisterTransfer(float Amount,clsBankClient DestinationClient)
    {

        string stDataLine = _PrepareTransferRecord(Amount,DestinationClient);

        fstream MyFile;
        MyFile.open("/data/data/com.ikou.cppcoding/files/code_project/Hello_World/src/LogTransfer.txt", ios::out | ios::app);

        if (MyFile.is_open())
        {

            MyFile << stDataLine << endl;

            MyFile.close();
        }

    }

    void _Update() {
        auto clients = GetDataClient();
        for (auto &client : clients) {
            if (client.AccountNumber() == AccountNumber()) {
                client = *this;
                break;
            }
        }
        _SaveClientsDataToFile(clients);
    }

    void _AddNew()
    {

        _AddDataLineToFile(_ConverClientObjectToLine(*this));
    }

    void _AddDataLineToFile(string  stDataLine)
    {
        fstream MyFile;
        MyFile.open("/data/data/com.ikou.cppcoding/files/code_project/Hello_World/src/Clients.txt", ios::out | ios::app);

        if (MyFile.is_open())
        {

            MyFile << stDataLine << endl;

            MyFile.close();
        }

    } 

    static clsBankClient _GetEmptyClientObject() {
        return clsBankClient(enMode::EmptyMode, "", "", "", "", "", "", 0);
    }

public:
    clsBankClient(enMode Mode, const std::string &FirstName,
                  const std::string &LastName, const std::string &Email,
                  const std::string &Phone, const std::string &AccountNumber,
                  const std::string &PinCode, float AccountBalance)
        : clsPerson(FirstName, LastName, Email, Phone), _Mode(Mode),
          _AccountNumber(AccountNumber), _PinCode(PinCode),
          _AccountBalance(AccountBalance) {}

    bool IsEmpty() const {
        return (_Mode == enMode::EmptyMode);
    }

    std::string AccountNumber() const {
        return _AccountNumber;
    }

    void SetPinCode(const std::string &PinCode) {
        _PinCode = PinCode;
    }

    std::string GetPinCode() const {
        return _PinCode;
    }

    void SetAccountBalance(float AccountBalance) {
        _AccountBalance = AccountBalance;
    }

    float GetAccountBalance() const {
        return _AccountBalance;
    }


    static clsBankClient Find(const std::string &AccountNumber) {
        auto clients = GetDataClient();
        for (const auto &client : clients) {
            if (client.AccountNumber() == AccountNumber) {
                return client;
            }
        }
        return _GetEmptyClientObject();
    }

    static clsBankClient Find(const std::string &AccountNumber,
                              const std::string &PinCode) {
        auto client = Find(AccountNumber);
        if (client.GetPinCode() == PinCode) {
            return client;
        }
        return _GetEmptyClientObject();
    }

    enum class enSaveResults {
        svFaildEmptyObject = 0,
        svSucceeded = 1,
        svFaildAccountNumberExists = 3
    };

    enSaveResults Save() {

        switch (_Mode) {

        case enMode::UpdateMode: {
            _Update();
            return enSaveResults::svSucceeded;
            break;
        }
        case enMode::EmptyMode: {
            return enSaveResults::svFaildEmptyObject;
            break;
        }

        case enMode::AddNewMode: {

            if (clsBankClient::IsClientExist(_AccountNumber)) {

                return enSaveResults::svFaildAccountNumberExists;
            }

            _Mode = enMode::UpdateMode;
            _clientsList.push_back(*this);

            _AddNew();

            return enSaveResults::svSucceeded;
            break;

        }
        }
    }

    static bool IsClientExist(const std::string &AccountNumber) {
        return !Find(AccountNumber).IsEmpty();
    }

    static std::vector<clsBankClient> GetDataClient() {
        if (_clientsList.empty()) {
            _LoadClientsDataFromFile();
        }
        return _clientsList;
    }

    static clsBankClient GetAddNewClientObject(const std::string &AccountNumber) {

        return clsBankClient(enMode::AddNewMode, "", "", "", "", AccountNumber, "",0);
    }

    const bool Delete() {
        for (auto it = _clientsList.begin(); it != _clientsList.end(); ) {
            if (it->AccountNumber() == AccountNumber()) {
                it->_MarkedForDelete = true;
                it = _clientsList.erase(it); // حذف العنصر وتحديث المؤشر
                break; // الخروج من الحلقة بعد الحذف
            } else {
                ++it; // الانتقال إلى العنصر التالي
            }
        }

        _SaveClientsDataToFile(_clientsList);
        return true;
    }

    static double GetTotalBalance(const std::vector<clsBankClient> Balances) {
        double TotalBalance = 0;
        for(const auto &Total : Balances) {
            TotalBalance += Total._AccountBalance;
        }
        return TotalBalance;

    }

    void Deposit(double Amount)
    {
        _AccountBalance += Amount;
        Save();
    }

    bool Withdraw(double Amount)
    {
      
    if (Amount > _AccountBalance)
    {
        return false; // الرصيد غير كافٍ
    }

    _AccountBalance -= Amount; // تعديل الرصيد
     
     for (auto &client : _clientsList)
    {
        if (client.AccountNumber() == AccountNumber())
        {
            client._AccountBalance = _AccountBalance;
            break;
        }
    }

    Save(); // حفظ البيانات إلى الملف بعد التحديث

    return true;
    
    }

    bool Transfer(float Amount,clsBankClient & DestinationClient) {


        if(Amount > GetAccountBalance()) {
            return false;
        }

        Withdraw(Amount);
        DestinationClient.Deposit(Amount);
        _RegisterTransfer(Amount,DestinationClient);
        return true;

    }
    
    struct stTrnsferLogRecord {
    string DateTime;
    string SourceAccountNumber;  // تعديل الاسم من sAcctNumber
    string DestinationAccountNumber;  // تعديل الاسم من dAcctNumber
    float Amount;
    float srcBalanceAfter;  // تعديل الاسم من sBalance
    float destBalanceAfter; // تعديل الاسم من dBalance
    string UserName;
    
    };
    
    
    static stTrnsferLogRecord _ConvertLinetoTransferRegister(const std::string &Line, const std::string &Seperator = "#//#") {
      
    stTrnsferLogRecord TransferRegisterRecord; 
    
    vector<string> TransferRegister = clsString::SplitString(Line, Seperator);
    
    if (TransferRegister.size() < 7) { // ✅ التأكد من أن السطر يحتوي على 7 أجزاء
        cerr << "Error: Invalid line format!" << endl;
        return TransferRegisterRecord;
    }
    
    TransferRegisterRecord.DateTime = TransferRegister[0];
    TransferRegisterRecord.SourceAccountNumber = TransferRegister[1];  // ✅ تعديل الاسم
    TransferRegisterRecord.DestinationAccountNumber = TransferRegister[2];  // ✅ تعديل الاسم
    TransferRegisterRecord.Amount = stof(TransferRegister[3]);  // ✅ تحويل string إلى float
    TransferRegisterRecord.srcBalanceAfter = stof(TransferRegister[4]); // ✅ تعديل الاسم وتحويل string إلى float
    TransferRegisterRecord.destBalanceAfter = stof(TransferRegister[5]); // ✅ تعديل الاسم وتحويل string إلى float
    TransferRegisterRecord.UserName = TransferRegister[6];

    return TransferRegisterRecord;
    
    }
      
    
    
    
    static vector <stTrnsferLogRecord> GetTransfersLogList() {
      
      vector <stTrnsferLogRecord> vTrnsferRegisterRecord;
      
      fstream MyFile;
        MyFile.open("/data/data/com.ikou.cppcoding/files/code_project/Hello_World/src/LogTransfer.txt", ios::in);//read Mode

        if (MyFile.is_open())
        {

            string Line;


            while (getline(MyFile, Line))
            {

                stTrnsferLogRecord Register = _ConvertLinetoTransferRegister(Line);

                vTrnsferRegisterRecord.push_back(Register);
            }

            MyFile.close();

        }
        
        
        return vTrnsferRegisterRecord;
      
      
      
    }
    
    


    friend class clsDeleteClientScreen;
};
