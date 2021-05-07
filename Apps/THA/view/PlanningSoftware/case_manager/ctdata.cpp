#include "ctdata.h"

CTData::CTData(QString uuid, QString id, QString path, QString category, QDateTime checkTime, int sections, QString hospitalInfo, std::vector<DiconFile*>& diconFiles)
{
    this->indexId = uuid.toInt();
    this->id = id;
    this->path = path;
    this->category = category;
    this->checkTime = checkTime;
    this->sections = sections;
    this->hospitalInfo = hospitalInfo;
    this->diconFiles = diconFiles;
    isNew = false;
}
CTData::CTData()
{
}
std::vector<DiconFile *> CTData::getDiconFiles() const
{
    return diconFiles;
}

QString CTData::getHospitalInfo() const
{
    return hospitalInfo;
}

void CTData::setHospitalInfo(const QString &value)
{
    hospitalInfo = value;
}

int CTData::getSections() const
{
    return sections;
}

void CTData::setSections(int value)
{
    sections = value;
}

QDateTime CTData::getCheckTime() const
{
    return checkTime;
}

void CTData::setCheckTime(const QDateTime &value)
{
    checkTime = value;
}

QString CTData::getCategory() const
{
    return category;
}

void CTData::setCategory(const QString &value)
{
    category = value;
}

QString CTData::getPath() const
{
    return path;
}

void CTData::setPath(const QString &value)
{
    path = value;
}

QString CTData::getId() const
{
    return id;
}

void CTData::setId(const QString &value)
{
    id = value;
}

CTData::CTData(QString id, QString path, QString category, QDateTime checkTime, int sections, QString hospitalInfo, std::vector<DiconFile*>& diconFiles)
{
    this->id = id;
    this->path = path;
    this->category = category;
    this->checkTime = checkTime;
    this->sections = sections;
    this->hospitalInfo = hospitalInfo;
    this->diconFiles = diconFiles;
    isNew = true;
}

CTData::~CTData()
{
    for ( auto p : diconFiles )
    {
        delete p;
    }
}

bool CTData::save(const QString pCaseUuid)
{
    std::vector<DiconFile*>::iterator it1 = diconFiles.begin();
    std::vector<DiconFile*>::iterator it2 = diconFiles.end();
    for (;it1 != it2; ++it1)
    {
        bool result = (*it1)->save(this->indexId);
        if (result == false)
        {
			// LOG_DEBUG("Do not save CTData");
            return false;
        }
    }

    if ( isNew == false )
    {
        deleteFromDB();
    }

    QString sql = "INSERT INTO ct_data (id, path, category, check_time, sections, hospital_info, person_case_uuid) VALUES (?, ?, ?, ?, ?, ?, ?)";
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(0, this->id);
    query.bindValue(1, this->path);
    query.bindValue(2, this->category);
    query.bindValue(3, this->checkTime);
    query.bindValue(4, this->sections);
    query.bindValue(5, this->hospitalInfo);
    query.bindValue(6, pCaseUuid);
    if (query.exec())
    {
		// LOG_DEBUG("Saving CTData");
        return true;
    }
    else
    {
		// LOG_DEBUG("Saving CTData ...{}",query.lastError().text());
        return false;
    }
}

bool CTData::saveDB(const QString pCaseUuid)
{
	QString sql = "INSERT INTO ct_data (index_id, id, path, category, check_time, sections, hospital_info, person_case_uuid) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
	QSqlQuery query;
	query.prepare(sql);
	query.bindValue(0, this->indexId);
	query.bindValue(1, this->id);
	query.bindValue(2, this->path);
	query.bindValue(3, this->category);
	query.bindValue(4, this->checkTime);
	query.bindValue(5, this->sections);
	query.bindValue(6, this->hospitalInfo);
	query.bindValue(7, pCaseUuid);
	if (query.exec())
	{
		// LOG_DEBUG("Saving CTData");
		return true;
	}
	else
	{
		// LOG_DEBUG("Saving CTData...{}", query.lastError().text());
		return false;
	}
}

void CTData::Import(std::vector<std::string> &files)
{
	QDir copyToDir(this->path);

	for (auto file_full : files)
	{
		QString sourceFilePath = QString::fromStdString(file_full);
		QFileInfo fileinfo(sourceFilePath);
		QString filename = fileinfo.fileName();
		QString destFilePath = this->path + "./" + filename;
		QFile mFile(destFilePath);
		if (mFile.exists())
		{
			QFile::remove(destFilePath);
		}
		QFile::copy(sourceFilePath, destFilePath);
	}
}

std::vector<CTData*> CTData::select(const QString pCaseUuid, bool pFull)
{
    std::vector<DiconFile*> tDicomFiles;
    if ( pFull == true )
    {
        tDicomFiles = DiconFile::select(pCaseUuid);
    }


    QString sql = "SELECT id, path, category, check_time, sections, hospital_info, uuid FROM ct_data WHERE person_case_uuid = '%1'";
    QString finalQuery = sql.arg(pCaseUuid);
    QSqlQuery query;
    std::vector<CTData*> results;
    if (query.exec(finalQuery))
    {
		// LOG_DEBUG("Selecting CTData");
        while (query.next())
        {
            QString tId = query.value(0).toString();
            QString tPath = query.value(1).toString();
            QString tCategory = query.value(2).toString();
            QDateTime tCheckTime = query.value(3).toDateTime();
            int tSections = query.value(4).toInt();
            QString tHospitalInfo = query.value(5).toString();
            QString tUuid = query.value(6).toString();
            results.push_back(new CTData(tUuid, tId, tPath, tCategory, tCheckTime, tSections, tHospitalInfo, tDicomFiles));
        }
    }
    else
    {
		// LOG_DEBUG("Selecting CTData...{}",query.lastError().text());
    }
    return results;
}

bool CTData::deleteFromDB()
{
	QString finalQuery = "DELETE FROM ct_data WHERE uuid = '"+this->indexId;
    QSqlQuery query;
    if (query.exec(finalQuery))
    {
		// LOG_DEBUG("Delete CTData");
        return true;
    }
    else
    {
		// LOG_DEBUG("Delete CTData...{}", query.lastError().text());
        return false;
    }
}

bool CTData::deleteAllCTDataForThisCaseFromDB(const QString pCaseUuid)
{
    QString sql = "DELETE FROM ct_data WHERE person_case_uuid = %1";
    QString finalQuery = sql.arg(pCaseUuid);
    QSqlQuery query;
    if (query.exec(finalQuery))
    {
		// LOG_DEBUG("Delete CTData");
        QDir dir(path);
        dir.removeRecursively();
        return true;
    }
    else
    {
		// LOG_DEBUG("Delete CTData...{}", query.lastError().text());
        return false;
    }
}

void CTData::updateSections()
{
    sections = diconFiles.size();
}
