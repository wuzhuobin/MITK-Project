#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QString>
#include "dbmanager.h"
#include <QDebug>

DbManager::DbManager(const QString& path)
{
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(path);
    if (!m_database.open())
    {
        qDebug() << "Error: Connection with database fail..." << m_database.lastError().text();
    }
    else
    {
        qDebug() << "Database connection OK";
    }
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS user(index_id  INTEGER PRIMARY KEY AUTOINCREMENT, username VARCHAR(50), name VARCHAR(50), password VARCHAR(50), role INTEGER, status INTEGER)");
    m_database.commit();
}

void DbManager::openDb(){
    //create and open m_database called "MyDataBase.db"
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName("sovajoDatabase.db");
    if (!m_database.open())
    {
        qDebug() << "Database connection wrong!";
    }
    else
    {
        qDebug() << "Database connection OK";
    }
}