#include "CaseManagementDialog.h"

#include "ui_CaseManagementDialog.h"
// #include "theme.h"
// #include "AdminMenu.h"

// mitk
#include <mitkLogMacros.h>
#include <QmitkDicomExternalDataWidget.h>
#include <QmitkDicomLocalStorageWidget.h>

// qt
#include <qdialog.h>
#include <qglobal.h>
#include <qvariant.h>
#include <qdebug.h>

const static struct CaseManagementDialogResourceInit
{
    CaseManagementDialogResourceInit() { Q_INIT_RESOURCE(resource); }
} GInit;

CaseManagementDialog::CaseManagementDialog(QWidget* parent) :
    QDialog(parent), mUi(new Ui::CaseManagementDialog)
{
    mUi->setupUi(this);
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
    connect(this, &CaseManagementDialog::finished, this, &CaseManagementDialog::hide);
}

CaseManagementDialog::~CaseManagementDialog()
{
    delete mUi;
}

void CaseManagementDialog::on_nextStepBtn_clicked(bool checked)
{
    emit accept();
}
