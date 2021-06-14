#ifndef account_manager_account_manager_h_
#define account_manager_account_manager_h_

#include <QObject>
#include <QtSql/QSqlQuery>
#include <vector>
#include "account.h"

class AccountManager : public QObject
{
    Q_OBJECT
public:
    enum QueryItems {
        kId,      // QString
        kName,    // QString
        kGroup,   // quint
        kActive,  // int
        kLastLoginTime,  // QDateTime
    };
        
    virtual QString getEncryptDecrypt(const QString &str) = 0;    
    virtual bool login(const QString& userName, const QString& password, Account* account = nullptr) = 0;
    virtual Account account(const QString& userName) = 0;
    virtual QString userName() = 0;
    // Here we expose QSqlQuery to use QSqlQueryModel for GUI as a tradeoff for develope time
    virtual QSqlQuery queryAll(const std::vector<QueryItems>& requestResults) = 0;

    virtual Account getAccount(const QString& userName) = 0;
    virtual bool setAccount(const QString& userName, Account::Group group) = 0;
    virtual bool setActive(const QString& userName, Account::Status status) = 0;

    virtual bool setName(const QString& userName, const QString& name) = 0;
    virtual bool setPassword(const QString& userName, const QString& password) = 0;

    static void initialize(AccountManager* manager);
    static AccountManager* instance();

    virtual bool createNewUser(const QString& userName, const QString& name, const QString& password, const Account::Group& group , const Account::Status& status) = 0;

};
#endif // account_manager_account_manager_h_