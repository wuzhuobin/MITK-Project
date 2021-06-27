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
#include <qfileinfo.h>
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
    mUi->tableViewCase->setModel(mCaseModel);
    mUi->tableViewCase->setRootIndex(mCaseModel->index(mCaseModel->rootPath()));

    connect(mUi->tabDicomExternalData,
            &QmitkDicomExternalDataWidget::SignalStartDicomImport,
            mUi->tabDicomLocalStorage,
            QOverload<const QStringList&>::of(&THADicomLocalStorageWidget::onStartDicomImport));
    connect(mUi->tabDicomLocalStorage,
            &THADicomLocalStorageWidget::pushButtonCreateCaseClicked,
            this,
            &CaseManagementDialog::createCase);

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
    auto aCase = IOController::getBaseProject();
    MITK_INFO << aCase.toStdString();
    IOController::getInstance()->loadScene(aCase);
}

void CaseManagementDialog::on_lineEditSearch_textChanged(const QString& text)
{
    mCaseModel->setNameFilters(QStringList() << "*" + text + "*");
}

void CaseManagementDialog::createCase(const QStringList& dicoms)
{
    for (const auto& dicom : dicoms)
    {
        MITK_INFO << dicom.toStdString();
    }
    auto infoList = QDir(mCaseModel->rootPath()).entryInfoList(QDir::Files);
    
    auto max = -1;
    for (const auto& info: infoList)
    {
        max = std::max(info.suffix().toInt(), max);
    }

    CaseCreationDialog(max + 1).exec();
}