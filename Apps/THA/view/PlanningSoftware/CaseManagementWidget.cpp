#include "CaseManagementWidget.h"

#include "ui_CaseManagementWidget.h"
// #include "theme.h"
#include "AdminMenu.h"

// mitk
#include <mitkLogMacros.h>
#include <QmitkDicomExternalDataWidget.h>
#include <QmitkDicomLocalStorageWidget.h>

// qt
#include <qglobal.h>
#include <qvariant.h>
#include <qdebug.h>

struct CaseManagementWidgetResourceInit
{
    CaseManagementWidgetResourceInit() { Q_INIT_RESOURCE(resource); }
} gInit;

CaseManagementWidget::CaseManagementWidget(QWidget* parent) :
    QWidget(parent), mUi(new Ui::CaseManagementWidget), mPatientTableModel(1, 3)
{
    mUi->setupUi(this);
    // mUi->patientTableView->setModel(&mPatientTableModel);
    mUi->toolButtonAdmin->setMenu(new AdminMenu);
    auto* external = new QmitkDicomExternalDataWidget(this);
    external->setObjectName("dicomExternalDataWidget");

    auto* internal = new QmitkDicomLocalStorageWidget(this);
    internal->SetDatabaseDirectory(qApp->applicationDirPath());
    internal->setObjectName("dicomLocalStorageWidget");

    mUi->tabDicomExternalData->layout()->addWidget(external);
    mUi->tabDicomLocalStorage->layout()->addWidget(internal);
    connect(external,
            &QmitkDicomExternalDataWidget::SignalStartDicomImport,
            internal,
            QOverload<const QStringList&>::of(&QmitkDicomLocalStorageWidget::OnStartDicomImport));
    connect(internal, &QmitkDicomLocalStorageWidget::SignalDicomToDataManager, [](QHash<QString, QVariant> map) {
        for(auto cit = map.cbegin(); cit !=  map.cend(); ++ cit)
        {
            // MITK_INFO << cit.key().toStdString();
            // MITK_INFO << cit.value().toString().toStdString();
            qDebug() << cit.key();
            qDebug() << cit.value();
        }
    });
    // connect(external, &QmitkDicomExternalDataWidget::SignalStartDicomImport, [](const QStringList& list) {
    //     for (const auto& path : list)
    //     {
    //         MITK_INFO << path.toStdString();
    //     }
    // });
    connect(this, &CaseManagementWidget::finished, this, &CaseManagementWidget::hide);
}

CaseManagementWidget::~CaseManagementWidget()
{
    delete mUi;
}

void CaseManagementWidget::on_nextStepBtn_clicked(bool checked)
{
    emit finished();
}
