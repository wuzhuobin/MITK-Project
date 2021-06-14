#ifndef apps_svpk_lib_table_aligncenter_qsqlquery_model_h_
#define apps_svpk_lib_table_aligncenter_qsqlquery_model_h_

#include <QObject>
#include <QSqlQueryModel>

class TableAlignCenterQSqlQueryModel :	public QSqlQueryModel
{
public:	TableAlignCenterQSqlQueryModel(QObject *parent=nullptr );
	~TableAlignCenterQSqlQueryModel();
	 QVariant data(const QModelIndex &item, int role1 = Qt::DisplayRole) const override;

};
#endif // apps_svpk_lib_table_aligncenter_qsqlquery_model_h_