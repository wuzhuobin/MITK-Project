#ifndef CASE_MODEL_H
#define CASE_MODEL_H

#include <qfilesystemmodel.h>
#include <qglobal.h>
#include <qobject.h>
class CaseModel : public QFileSystemModel
{
    Q_OBJECT
public:
    CaseModel(QObject* parent = nullptr);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
    Q_DISABLE_COPY(CaseModel);
};

#endif  //! CASE_MODEL_H