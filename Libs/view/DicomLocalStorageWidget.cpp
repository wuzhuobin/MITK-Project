#include "DicomLocalStorageWidget.h"

// mitk
#include <QmitkDicomLocalStorageWidget.h>

// qt
#include <qdebug.h>
#include <qfileinfo.h>
#include <qglobal.h>
#include <qpushbutton.h>
#include <qtableview.h>
#include <qwidget.h>

// ctk
#include <ctkDICOMThumbnailGenerator.h>
#include <ctkThumbnailLabel.h>
#include <ctkThumbnailListWidget.h>

DicomLocalStorageWidget::DicomLocalStorageWidget(QWidget* parent) :
    QmitkDicomLocalStorageWidget(parent),
    mThumbnailsWidget(new ctkThumbnailListWidget(this)),
    mThumbnailGenerator(new ctkDICOMThumbnailGenerator(this)),
    mPushButtonCreateCase(new QPushButton(tr("Create Case"), this))
{
  m_Controls->localFilesGroupBox->layout()->addWidget(mThumbnailsWidget);
  m_Controls->frame->layout()->addWidget(mPushButtonCreateCase);
  mPushButtonCreateCase->setEnabled(false);

  auto tables = this->findChildren<QTableView*>("tblDicomDatabaseView");
  for (auto* table : tables)
  {
    table->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
  }

  connect(m_Controls->viewInternalDataButton,
          &QPushButton::clicked,
          this,
          &DicomLocalStorageWidget::on_viewInternalDataButton_clicked);

  connect(mPushButtonCreateCase,
          &QPushButton::clicked,
          this,
          &DicomLocalStorageWidget::on_pushButtonCreateCase_clicked);

  connect(m_Controls->ctkDicomBrowser,
          QOverload<const QStringList&>::of(
              &ctkDICOMTableManager::seriesSelectionChanged),
          this,
          &DicomLocalStorageWidget::onSeriesSelectionChanged);
}

void DicomLocalStorageWidget::setDatabaseDirectory(QString newDatabaseDirectory)
{
  SetDatabaseDirectory(newDatabaseDirectory);
  // auto *gen = new ctkDICOMThumbnailGenerator(this);
  mThumbnailGenerator->setHeight(64);
  mThumbnailGenerator->setWidth(64);
  m_LocalDatabase->setThumbnailGenerator(mThumbnailGenerator);
}

void DicomLocalStorageWidget::onStartDicomImport(const QStringList& dicomData)
{
  if (m_LocalDatabase->isOpen())
  {
    m_LocalIndexer->addListOfFiles(dicomData, true);
  }
}
void DicomLocalStorageWidget::on_viewInternalDataButton_clicked(
    bool /*checked*/)
{
  auto uids = m_Controls->ctkDicomBrowser->currentSeriesSelection();
  mThumbnailsWidget->clearThumbnails();
  for (const auto& uid : uids)
  {
    auto filesForSeries = m_LocalDatabase->filesForSeries(uid);
    for (const auto& file : filesForSeries)
    {
      auto dicomPrefix = m_LocalDatabase->databaseDirectory() + "/dicom";
      auto thumbnail = m_LocalDatabase->databaseDirectory() + "/thumb" +
                       file.mid(dicomPrefix.size()) + ".png";

      QFileInfo fileInfo(thumbnail);
      if (!fileInfo.exists())
      {
        QDir().mkpath(QFileInfo(thumbnail).path());
        mThumbnailGenerator->generateThumbnail(file, thumbnail);
      }
      QPixmap pixmap(thumbnail);
      mThumbnailsWidget->addThumbnail(pixmap);
    }
  }
}

void DicomLocalStorageWidget::on_pushButtonCreateCase_clicked(bool /*checked*/)
{
  auto uids = m_Controls->ctkDicomBrowser->currentSeriesSelection();
  QStringList filesForSeries;
  for (const auto& uid : uids)
  {
    filesForSeries << m_LocalDatabase->filesForSeries(uid);
  }

  Q_EMIT pushButtonCreateCaseClicked(filesForSeries);
}

void DicomLocalStorageWidget::onSeriesSelectionChanged(
    const QStringList& selection)
{
  mPushButtonCreateCase->setEnabled(!selection.isEmpty());
}