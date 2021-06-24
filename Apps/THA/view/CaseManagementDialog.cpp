#include "CaseManagementDialog.h"

#include "CaseCreationDialog.h"
#include "CaseModel.h"
#include "IOController.h"
#include "THADicomLocalStorageWidget.h"
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

// ctk
#include <ctkDICOMThumbnailGenerator.h>

static const struct CaseManagementDialogResourceInit
{
    CaseManagementDialogResourceInit() {}
} GInit;

CaseManagementDialog::CaseManagementDialog(QWidget* parent) :
    QDialog(parent), mUi(new Ui::CaseManagementDialog), mCaseModel(new CaseModel(this))
{
    mUi->setupUi(this);

    mUi->tabDicomLocalStorage->setDatabaseDirectory(qApp->applicationDirPath());

    // auto* viewInternalDataButton = internal->findChild<QPushButton*>("viewInternalDataButton");
    // if (viewInternalDataButton)
    // {
    //     viewInternalDataButton->setText(tr("Create Case"));
    // }

    connect(mUi->tabDicomExternalData,
            &QmitkDicomExternalDataWidget::SignalStartDicomImport,
            mUi->tabDicomLocalStorage,
            QOverload<const QStringList&>::of(&THADicomLocalStorageWidget::onStartDicomImport));
    // connect(
    //     mUi->tabDicomLocalStorage, &QmitkDicomLocalStorageWidget::SignalDicomToDataManager, this, &CaseManagementDialog::createCase);
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

void CaseManagementDialog::createCase(QHash<QString, QVariant> map)
{
    auto dicoms = map["FilesForSeries"].toStringList();
    ctkDICOMThumbnailGenerator gen;
    gen.setWidth(64);
    gen.setHeight(64);
    for (auto dicom : dicoms)
    {
        auto dicomPrefix = qApp->applicationDirPath() + "/dicom";
        auto thumbnail = qApp->applicationDirPath() + "/thumb" + dicom.mid(dicomPrefix.size()) + ".png";
        QDir().mkpath(QFileInfo(thumbnail).path());
        gen.generateThumbnail(dicom, thumbnail);
    }
    // CaseCreationDialog(mCaseModel->rowCount(), this).exec();
}