#include "operationplan.h"
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QtSql/QSqlError>
#include <QUuid>
// #include "utils/logger/logger_qt_type.h"
// #include "utils/logger/logger.h"

OperationPlan::OperationPlan(QString uuid, QString id, QDateTime endingTime, QString category, QString planName, QString path, QString operationSide,
                             QString prosthesisName, QString prosthesisBrand)
{
    this->uuid = uuid;
    this->id = id;
    this->endingTime = endingTime;
    this->category = category;
    this->planName = planName;
    this->path = path;
    this->operationSide = operationSide;
    this->prosthesisName = prosthesisName;
    this->prosthesisBrand = prosthesisBrand;
    isNew = false;
}

QString OperationPlan::getProsthesisBrand() const
{
    return prosthesisBrand;
}

void OperationPlan::setProsthesisBrand(const QString &value)
{
    prosthesisBrand = value;
}

QString OperationPlan::getProsthesisName() const
{
    return prosthesisName;
}

void OperationPlan::setProsthesisName(const QString &value)
{
    prosthesisName = value;
}

QString OperationPlan::getOperationSide() const
{
    return operationSide;
}

void OperationPlan::setOperationSide(const QString &value)
{
    operationSide = value;
}

QString OperationPlan::getPath() const
{
    return path;
}

void OperationPlan::setPath(const QString &value)
{
    path = value;
}

QString OperationPlan::getPlanName() const
{
    return planName;
}

void OperationPlan::setPlanName(const QString &value)
{
    planName = value;
}

QString OperationPlan::getCategory() const
{
    return category;
}

void OperationPlan::setCategory(const QString &value)
{
    category = value;
}

QDateTime OperationPlan::getEndingTime() const
{
    return endingTime;
}

void OperationPlan::setEndingTime(const QDateTime &value)
{
    endingTime = value;
}

QString OperationPlan::getId() const
{
    return id;
}

void OperationPlan::setId(const QString &value)
{
    id = value;
}

OperationPlan::OperationPlan(QString id, QDateTime endingTime, QString category, QString planName, QString path, QString operationSide,
                             QString prosthesisName, QString prosthesisBrand)
{
    QString uuid = QUuid::createUuid().toString();
    this->uuid = uuid.mid(1, 36);
    this->id = id;
    this->endingTime = endingTime;
    this->category = category;
    this->planName = planName;
    this->path = path;
    this->operationSide = operationSide;
    this->prosthesisName = prosthesisName;
    this->prosthesisBrand = prosthesisBrand;
    isNew = true;
}


bool OperationPlan::save(const QString pCaseUuid)
{
    if ( isNew == false )
    {
        deleteFromDB();
    }

    QString sql = "INSERT INTO operation_plan (uuid, id, ending_time, category, plan_name, path, operation_side, prosthesis_name, prosthesis_brand, person_case_uuid) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(0, this->uuid);
    query.bindValue(1, this->id);
    query.bindValue(2, this->endingTime);
    query.bindValue(3, this->category);
    query.bindValue(4, this->planName);
    query.bindValue(5, this->path);
    query.bindValue(6, this->operationSide);
    query.bindValue(7, this->prosthesisName);
    query.bindValue(8, this->prosthesisBrand);
    query.bindValue(9, pCaseUuid);
    if (query.exec())
    {
		// LOG_DEBUG("Saving OperationPlan");
        return true;
    }
    else
    {
		// LOG_DEBUG("Saving OperationPlan...{}", query.lastError().text());
        return false;
    }
}

std::vector<OperationPlan*> OperationPlan::select(const QString pCaseUuid)
{
    QString sql = "SELECT id, ending_time, category, plan_name, path, operation_side, prosthesis_name, prosthesis_brand, uuid FROM operation_plan WHERE person_case_uuid = '%1'";
    QString finalQuery = sql.arg(pCaseUuid);
    QSqlQuery query;
    std::vector<OperationPlan*> results;
    if (query.exec(finalQuery))
    {
		// LOG_DEBUG("Selecting OperationPlan");
        while (query.next())
        {
            QString tId = query.value(0).toString();
            QDateTime tEndingTime = query.value(1).toDateTime();
            QString tCategory = query.value(2).toString();
            QString tPlanName = query.value(3).toString();
            QString tPath = query.value(4).toString();
            QString tOperationSide = query.value(5).toString();
            QString tProsthesisName = query.value(6).toString();
            QString tProsthesisBrand = query.value(7).toString();
            QString tUuid = query.value(8).toString();
            results.push_back(new OperationPlan(tUuid, tId, tEndingTime, tCategory, tPlanName, tPath,
                                                tOperationSide, tProsthesisName, tProsthesisBrand));
        }
    }
    else
    {
		// LOG_DEBUG("Selecting OperationPlan...{}",query.lastError().text());
    }
    return results;
}

bool OperationPlan::deleteFromDB()
{
	QString finalQuery = "DELETE FROM operation_plan WHERE uuid = '"+ this->uuid +"'";
    QSqlQuery query;
    if (query.exec(finalQuery))
    {
		// LOG_DEBUG("Delete OperationPlan");
        return true;
    }
    else
    {
		// LOG_DEBUG("Delete OperationPlan...{}", query.lastError().text());
        return false;
    }
}

bool OperationPlan::deleteAllPlanForThisCaseFromDB(const QString pCaseUuid)
{
    QString sql = "DELETE FROM operation_plan WHERE person_case_uuid = %1";
    QString finalQuery = sql.arg(pCaseUuid);
    QSqlQuery query;
    if (query.exec(finalQuery))
    {
		// LOG_DEBUG("Delete OperationPlan");
        return true;
    }
    else
    {
		// LOG_DEBUG("Delete OperationPlan ", query.lastError().text());
        return false;
    }
}