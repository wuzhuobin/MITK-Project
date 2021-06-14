#ifndef case_manager_ctdata_h_
#define case_manager_ctdata_h_
#include <QString>
#include <QDateTime>
#include <vector>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QUuid>
#include <QDir>
// #include "utils/logger/logger_qt_type.h"
// #include "utils/logger/logger.h"

struct DiconFile
{
    QString uuid;
    QString name;
    DiconFile(QString name)
    {
        this->name = name;
    }

    bool save(const int pCT_data_uui)
    {
        QString sql = "INSERT INTO dicom_file (uuid, name, ct_data_uuid) VALUES (?, ?, ?)";
        QSqlQuery query;
        query.prepare(sql);
        QString uuid = QUuid::createUuid().toString();
        uuid = uuid.mid(1, 36);
        query.bindValue(0, uuid);
        query.bindValue(1, name);
        query.bindValue(2, pCT_data_uui);
        if (query.exec())
        {
			// LOG_DEBUG("Saving DiconFile");
            return true;
        }
        else
        {
			// LOG_DEBUG("Saving DiconFile...{}", query.lastError().text());
            return false;
        }
    }

    bool deleteFromDB(const QString pPath)
    {
        QString sql = "DELETE FROM dicom_file WHERE uuid = %1";
        QString finalQuery = sql.arg(this->uuid);
        QSqlQuery query;
        if (query.exec(finalQuery))
        {
			// LOG_DEBUG("Delete DiconFile.");
            QDir dir(pPath);
            dir.remove(name);
            return true;
        }
        else
        {
			// LOG_DEBUG("Delete DiconFile...{}", query.lastError().text());
            return false;
        }
    }

    static std::vector<DiconFile*> select(QString pCT_data_uuid)
    {
        QString sql = "SELECT name FROM dicom_file WHERE ct_data_uuid = '%1'";
        QString finalQuery = sql.arg(pCT_data_uuid);
        QSqlQuery query;
        std::vector<DiconFile*> results;
        if (query.exec(finalQuery))
        {
			// LOG_DEBUG("Selecting DiconFile");
                while (query.next())
                {
                   QString tName = query.value(0).toString();
                   results.push_back(new DiconFile(tName));
                }
        }
        else
        {
			// LOG_DEBUG("Selecting DiconFile...{}",query.lastError().text());
        }
        return results;
    }
};


class CTData
{
    friend class PersonCase;
public:
	CTData();
    CTData(QString id, QString path, QString category, QDateTime checkTime, int sections, QString hospitalInfo, std::vector<DiconFile*>& diconFiles);
    ~CTData();
    bool save(const QString pCaseUuid);
	bool saveDB(const QString pCaseUuid);
	//void Import(std::vector<std::string> &files);
	void Import(std::vector<std::string> &files);
    static std::vector<CTData*> select(const QString pCaseUuid, bool pFull = true);
    bool deleteFromDB();
    bool deleteAllCTDataForThisCaseFromDB(const QString pCaseUuid);
    void updateSections();
    QString getId() const;
    void setId(const QString &value);

    QString getPath() const;
    void setPath(const QString &value);

    QString getCategory() const;
    void setCategory(const QString &value);

    QDateTime getCheckTime() const;
    void setCheckTime(const QDateTime &value);

    int getSections() const;
    void setSections(int value);

    QString getHospitalInfo() const;
    void setHospitalInfo(const QString &value);

    std::vector<DiconFile *> getDiconFiles() const;

    CTData(QString uuid, QString id, QString path, QString category, QDateTime checkTime, int sections, QString hospitalInfo, std::vector<DiconFile*>& diconFiles);
	int indexId;
	QString id;
    QString path;
    QString category;
    QDateTime checkTime;
    int sections;
    QString hospitalInfo;
    std::vector<DiconFile*> diconFiles;
    bool isNew;
};
#endif // case_manager_ctdata_h_