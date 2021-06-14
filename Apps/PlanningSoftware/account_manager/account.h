#ifndef account_manager_account_h_
#define account_manager_account_h_

#include <QString>
#include <QDateTime>

struct Account {
    enum class Group {
        kAdmin = 0x01,     // edit accounts
        kSurgon = 0x02,    // create/import/edit case
    };
    enum class Status {
        kActive = 0x01,
        kInactive = 0x00,
    };

    QString userName;
    //QString password;
    QString passwordEncrypted;
    QDateTime lastLoginTime;
    Group group;
    Status status;
    int userIndexId;
    QString name;

};
#endif  // account_manager_account_h_