#include "account_manager_impl.h"
#include <QTextCodec>
#include <vector>
#include <QtWidgets/QApplication>
#include <QVariant>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QDir>

AccountManagerImpl::AccountManagerImpl()
{

}

QSqlQuery AccountManagerImpl::queryAll(const std::vector<QueryItems>& requestResults)
{
    QSqlQuery query;
    QString var = "select";
    for (int i = 0; i < requestResults.size(); i++) {
        if (requestResults[i] == QueryItems::kId) {
            var = var + " username,";    
        }
        else if(requestResults[i] == QueryItems::kName)
        {
            var = var + " name,";
        }
        else if (requestResults[i] == QueryItems::kGroup)
        {
            var = var + " role,";
        }
        else if (requestResults[i] == QueryItems::kActive)
        {
            var = var + " status,";
        }
    }
    var = var.left(var.length() - 1);
    var = var + " from user";
    query.exec(var);
    return query;
}

bool AccountManagerImpl::createNewUser(const QString& userName, const QString& name, const QString& password, const Account::Group& group, const Account::Status& status)
{
    QString passwordEncrypted = getEncryptDecrypt(password);

    QDir folder;
    QString curWorkBasePath = QApplication::applicationDirPath();  // this file is in the debug file
    bool exist = folder.exists(QString("%1/user_folder/%2").arg(curWorkBasePath).arg(userName));
    if (exist)
    {
        return false;
    }
    else
    {
        bool ok = folder.mkpath(QString("%1/user_folder/%2").arg(curWorkBasePath).arg(userName));
        if (ok) {
            QString sql = "INSERT INTO user (username, name, password, role, status) VALUES (?, ?, ?, ?, ?)";
            QSqlQuery query;
            query.prepare(sql);
            query.bindValue(0, userName);
            query.bindValue(1, name);
            query.bindValue(2, passwordEncrypted);
            query.bindValue(3, (int)group);
            query.bindValue(4, (int)status);

            if (query.exec())
            {
                qDebug()<<"Saving User!";
                return true;
            }
            else
            {
                qDebug() << "Saving User :" << query.lastError().text();
                return false;
            }
        }
        else {
            return false;
        }
    }
}

Account AccountManagerImpl::getAccount(const QString& userName)
{
    QSqlQuery query;
    Account account;
    query.exec("select * from user");
    while (query.next())
    {
        if (userName == query.value(1).toString())
        {
            account.userIndexId = query.value(0).toInt();
            account.userName = userName;
            account.name = query.value(2).toString();
            account.passwordEncrypted = query.value(3).toString();
            account.group = (Account::Group)query.value(4).toInt();
            account.status = (Account::Status)query.value(5).toInt();
            return account;
        }
    }
    return m_account;
    qDebug() << "Get Account Wrong!";
}

bool AccountManagerImpl::login(const QString& userName, const QString& password, Account* account)
{
    QSqlQuery query;
    QString passwordEncrypted = getEncryptDecrypt(password);
    qDebug() <<  userName << password;

    if (!query.exec("CREATE TABLE IF NOT EXISTS user(uuid VARCHAR(36) primary key, username VARCHAR(50), name VARCHAR(50), password VARCHAR(50), role INTEGER, status INTEGER)"))
    {
        qDebug() << "Error: Fail to create table..." << query.lastError().text();
    }
    else
    {
        qDebug() << "Table created!";
    }
    // login :check userName and passWord if exist in database.
    query.exec("select * from user");
    while (query.next())
    {
        if (userName == query.value(1).toString() && passwordEncrypted == query.value(3).toString())
        {
            if (query.value(5).toInt()== 1) {
                qDebug() << "Login Successful";
                m_account.userName = userName;
                m_encryptKey = 12;
                m_account.passwordEncrypted = passwordEncrypted;
                return true;
            }
            // if ID is not active.
            else {
                qDebug() << "ID is not active";
                return false;
            }
        }
    }
    qDebug() << "Sorry. ID or Password is wrong!";
    return false;
}

Account AccountManagerImpl::account(const QString& user)
{
    return m_account;
}

QString AccountManagerImpl::userName()
{
    return 	m_account.userName;
}

bool AccountManagerImpl::setAccount(const QString& userName, Account::Group group)
{
    QString sql = "UPDATE user SET role = %1 WHERE username = '%2'";
    QString finalQuery = sql.arg((int)group).arg(userName);
    QSqlQuery query;
    if (query.exec(finalQuery)) {
        qDebug() << "Updata User";
        return true;
    }
    else {
        qDebug() << "Updata User:" << query.lastError().text();
        return false;
    }
}

bool AccountManagerImpl::setActive(const QString& userName, Account::Status status)
{
    QString sql = "UPDATE user SET status = %1 WHERE username = '%2'";
    QString finalQuery = sql.arg((int)status).arg(userName);
    QSqlQuery query;
    if (query.exec(finalQuery)) {
        qDebug() << "Updata User";
        return true;
    }
    else {
        qDebug() << query.lastError().text();
        return false;
    }
}

bool AccountManagerImpl::setName(const QString& userName, const QString& name)
{
    QString sql = "UPDATE user SET name = '%1' WHERE username = '%2'";
    QString finalQuery = sql.arg(name).arg(userName);
    QSqlQuery query;
    if (query.exec(finalQuery)) {
        qDebug() << "Updata User";
        return true;
    }
    else {
        qDebug() << "Updata User: " << query.lastError().text();
        return false;
    }
}

bool AccountManagerImpl::setPassword(const QString& userName, const QString& password)
{
    QString passwordEncrypted = getEncryptDecrypt(password);
    QString sql = "UPDATE user SET password = '%1' WHERE username = '%2'";
    QString finalQuery = sql.arg(passwordEncrypted).arg(userName);
    QSqlQuery query;
    if (query.exec(finalQuery)) {
        qDebug() << "Updata User";
        return true;
    }
    else {
        qDebug() << "Updata User {}", query.lastError().text();
        return false;
    }
}

//using Encryption and decryption using XOR + key algorithm
QString AccountManagerImpl::getEncryptDecrypt(const QString &str)
{    
    QString result;
    QByteArray bs = qstringToByte(str);

    for (int i = 0; i < bs.size(); i++) {
        bs[i] = bs[i] ^ m_encryptKey;
    }
    result = byteToQString(bs);
    return result;
}

QString AccountManagerImpl::byteToQString(const QByteArray &byte)
{
    QString result;
    if (byte.size() > 0) {
        QTextCodec *codec = QTextCodec::codecForName("utf-8");
        result = codec->toUnicode(byte);
    }
    return result;
}

QByteArray AccountManagerImpl::qstringToByte(const QString &strInfo)
{
    QByteArray result;
    if (strInfo.length() > 0) {
        QTextCodec *codec = QTextCodec::codecForName("utf-8");
        result = codec->fromUnicode(strInfo);
    }
    return result;
}