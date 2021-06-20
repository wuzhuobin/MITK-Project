#include "CaseManagementDialog.h"

#include "CaseModel.h"
#include "IOController.h"
#include "ui_CaseManagementDialog.h"

// mitk
#include <QmitkDicomExternalDataWidget.h>
#include <QmitkDicomLocalStorageWidget.h>
#include <mitkLogMacros.h>

// qt
#include <qabstractitemmodel.h>
#include <qdebug.h>
#include <qdialog.h>
#include <qglobal.h>
#include <qitemselectionmodel.h>
#include <qmessagebox.h>
#include <qnamespace.h>
#include <qtableview.h>
#include <qvariant.h>

#include <QTableWidgetItem>

CaseManagementDialog::CaseManagementDialog(QWidget* parent) :
    QDialog(parent), mUi(new Ui::CaseManagementDialog), mCaseModel(new CaseModel(this))
{
    mUi->setupUi(this);
    auto* external = new QmitkDicomExternalDataWidget(this);
    external->setObjectName("dicomExternalDataWidget");

    auto* internal = new QmitkDicomLocalStorageWidget(this);
    internal->SetDatabaseDirectory(qApp->applicationDirPath());
    internal->setObjectName("dicomLocalStorageWidget");
    auto *viewInternalDataButton = internal->findChild<QPushButton*>("viewInternalDataButton");
    if (viewInternalDataButton)
    {
        viewInternalDataButton->setText(tr("Create Case"));
    }

    mUi->tabDicomExternalData->layout()->addWidget(external);
    mUi->tabDicomLocalStorage->layout()->addWidget(internal);
    connect(external,
            &QmitkDicomExternalDataWidget::SignalStartDicomImport,
            internal,
            QOverload<const QStringList&>::of(&QmitkDicomLocalStorageWidget::OnStartDicomImport));
    connect(internal, &QmitkDicomLocalStorageWidget::SignalDicomToDataManager, [](QHash<QString, QVariant> map) {
        for (auto cit = map.cbegin(); cit != map.cend(); ++cit)
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
    mUi->tableViewCase->setModel(mCaseModel);
    mUi->tableViewCase->setRootIndex(mCaseModel->index(mCaseModel->rootPath()));

    connect(this, &CaseManagementDialog::finished, this, &CaseManagementDialog::hide);
}

CaseManagementDialog::~CaseManagementDialog()
{
    delete mUi;
}

void CaseManagementDialog::on_pushButtonImport_clicked(bool /*checked*/)
{
    auto indexes = mUi->tableViewCase->selectionModel()->selectedIndexes();
    if (indexes.isEmpty())
    {
        QMessageBox::warning(this, tr("No case selected."), tr("Please select a case."));
        return;
    }

    // auto aCase = mCaseModel->rootPath() + "/" + indexes.first().data().toString();
    auto aCase = mCaseModel->rootPath() + "/" + "THA.mitk";
    MITK_INFO << aCase.toStdString();
    IOController::getInstance()->loadScene(aCase);
}