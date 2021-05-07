#ifndef account_manager_impl_h_
#define account_manager_impl_h_

#include <QObject>
#include <QtSql/QSqlQuery>
#include <vector>
#include "account_manager.h"
#include "account.h"

class AccountManagerImpl: public AccountManager {

public:
    AccountManagerImpl();
    //using Encryption and decryption using XOR + key algorithm
    QString getEncryptDecrypt(const QString &str);    
    QSqlQuery queryAll(const std::vector<QueryItems>& requestResults) override;
    Account account(const QString& userName) override;
    bool login(const QString& userName, const QString& password, Account* account = nullptr) override;
    QString userName() override;
    Account getAccount(const QString& userName) override;
    bool setAccount(const QString& userName, Account::Group group) override;
    bool setActive(const QString& userName, Account::Status status) override;
    bool createNewUser(const QString& userName, const QString& name, const QString& password, const Account::Group& group, const Account::Status& status) override;
    bool setName(const QString& userName, const QString& name)override;
    bool setPassword(const QString& userName, const QString& password) override;

private:
    QString byteToQString(const QByteArray &byte);
    QByteArray qstringToByte(const QString &strInfo);
    Account m_account;
    char m_encryptKey = 12;
};
#endif // account_manager_account_manager_impl_h_