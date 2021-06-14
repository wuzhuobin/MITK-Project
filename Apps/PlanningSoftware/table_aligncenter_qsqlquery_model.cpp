#include "table_aligncenter_qsqlquery_model.h"
#include <QFont>

TableAlignCenterQSqlQueryModel::TableAlignCenterQSqlQueryModel(QObject *parent):QSqlQueryModel(parent)
{
}
TableAlignCenterQSqlQueryModel::~TableAlignCenterQSqlQueryModel()
{
}
QVariant TableAlignCenterQSqlQueryModel::data(const QModelIndex &item, int role) const
{	
	QVariant value = QSqlQueryModel::data(item, role);
	QFont dataFont("Microsoft YaHei", 16, 50);
	if (role == Qt::TextAlignmentRole ){
		value = Qt::AlignHCenter+Qt::AlignVCenter; // set Center ( horizontal and vertical )
	}
	if (role == Qt::FontRole) {
		value = dataFont;
	}
	if (role == Qt::DisplayRole &&item.column() == 2)
	{
		value = value.toString().mid(0, 19);
		return value;
	}
	return value;
}