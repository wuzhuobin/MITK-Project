#include "CaseManagementDialog.h"

#include "CaseCreationDialog.h"
#include "CaseModel.h"
#include "ui_CaseManagementDialog.h"
// #include "IOController.h"
#include "DicomLocalStorageWidget.h"

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
    QDialog(parent),
    mUi(new Ui::CaseManagementDialog),
    mCaseModel(new CaseModel(this))
{
  mUi->setupUi(this);
  mUi->tabDicomLocalStorage->setDatabaseDirectory(qApp->applicationDirPath());
  mUi->tableViewCase->setModel(mCaseModel);
  mUi->tableViewCase->setRootIndex(mCaseModel->index(mCaseModel->rootPath()));

  connect(mUi->tabDicomExternalData,
          &QmitkDicomExternalDataWidget::SignalStartDicomImport,
          mUi->tabDicomLocalStorage,
          QOverload<const QStringList&>::of(
              &DicomLocalStorageWidget::onStartDicomImport));
  connect(mUi->tabDicomLocalStorage,
          &DicomLocalStorageWidget::pushButtonCreateCaseClicked,
          this,
          &CaseManagementDialog::createCase);

  connect(
      this, &CaseManagementDialog::finished, this, &CaseManagementDialog::hide);
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
    QMessageBox::warning(
        this, tr("No case selected."), tr("Please select a case."));
    return;
  }

  auto aCase = mCaseModel->rootPath() + "/" + indexes.first().data().toString();
  emit caseImported(aCase);
}

void CaseManagementDialog::on_lineEditSearch_textChanged(const QString& text)
{
  mCaseModel->setNameFilters(QStringList() << "*" + text + "*");
}

void CaseManagementDialog::createCase(const QStringList& dicoms)
{
  auto infoList = QDir(mCaseModel->rootPath()).entryInfoList(QDir::Files);

  auto max = -1;
  for (const auto& info : infoList)
  {
    max = std::max(info.suffix().toInt(), max);
  }

  CaseCreationDialog d(max + 1, this);
  if (d.exec() != QDialog::Accepted)
  {
    return;
  }
  auto fileName = mCaseModel->rootPath() + '/' + d.getFileName();

  Q_EMIT caseCreated(fileName, dicoms);
}