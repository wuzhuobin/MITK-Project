#ifndef case_manager_operationplan_h_
#define case_manager_operationplan_h_
#include <QString>
#include <QDateTime>
#include <vector>

class OperationPlan
{
public:
    OperationPlan(QString id, QDateTime endingTime, QString category, QString planName, QString path, QString operationSide,
                  QString prosthesisName, QString prosthesisBrand);
    bool save(const QString pCaseUuid);
    static std::vector<OperationPlan*> select(const QString pCaseUuid);
    bool deleteFromDB();
    bool deleteAllPlanForThisCaseFromDB(const QString pCaseUuid);
    QString getId() const;
    void setId(const QString &value);

    QDateTime getEndingTime() const;
    void setEndingTime(const QDateTime &value);

    QString getCategory() const;
    void setCategory(const QString &value);

    QString getPlanName() const;
    void setPlanName(const QString &value);

    QString getPath() const;
    void setPath(const QString &value);

    QString getOperationSide() const;
    void setOperationSide(const QString &value);

    QString getProsthesisName() const;
    void setProsthesisName(const QString &value);

    QString getProsthesisBrand() const;
    void setProsthesisBrand(const QString &value);

private:
    OperationPlan(QString uuid, QString id, QDateTime endingTime, QString category, QString planName, QString path, QString operationSide,
                  QString prosthesisName, QString prosthesisBrand);
    QString id;
    QDateTime endingTime;
    QString category;
    QString planName;
    QString path;
    QString operationSide;
    QString prosthesisName;
    QString prosthesisBrand;
    QString uuid;
    bool isNew;
};
#endif //case_manager_operationplan_h_