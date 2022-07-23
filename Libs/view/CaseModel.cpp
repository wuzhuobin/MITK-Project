#include "CaseModel.h"

// qt
#include <qabstractitemmodel.h>
#include <qcoreapplication.h>
#include <qdir.h>
#include <qfilesystemmodel.h>
#include <qnamespace.h>

CaseModel::CaseModel(QObject* parent) : QFileSystemModel(parent)
{
    QDir path(qApp->applicationDirPath() + "/cases");
    if (!path.exists())
    {
        path.mkpath(".");
    }
    setRootPath(path.absolutePath());
    setFilter(QDir::Files);
    setReadOnly(true);
    setNameFilterDisables(false);
}

QVariant CaseModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section == 1 && orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return tr("ID");
    }
    if (section == 2 && orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return tr("Import Time");
    }

    return QFileSystemModel::headerData(section, orientation, role);
}

int CaseModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 3;
}

QVariant CaseModel::data(const QModelIndex& index, int role) const
{
    if (index.column() == 0 && role == Qt::DisplayRole)
    {
        return fileInfo(index).completeBaseName();
    }
    if (index.column() == 1 && role == Qt::DisplayRole)
    {
        return fileInfo(index).suffix();
    }
    if (index.column() == 2 && role == Qt::DisplayRole)
    {
        return QFileSystemModel::data(index.siblingAtColumn(3));
    }
    return QFileSystemModel::data(index, role);
}