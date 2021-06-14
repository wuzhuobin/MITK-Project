#ifndef account_manager_dbmanager_h_
#define account_manager_dbmanager_h_
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>

class DbManager
{
public:
    DbManager(const QString& path);
    void openDb();
    QSqlDatabase getDb(){
        return m_database;
    }

private:
    QSqlDatabase m_database;
};

#endif // account_manager_dbmanager_h_