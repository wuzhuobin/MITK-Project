#ifndef DICOM_LOCAL_STORAGE_WIDGET_H
#define DICOM_LOCAL_STORAGE_WIDGET_H

// mitk
#include <QmitkDicomLocalStorageWidget.h>

class DicomLocalStorageWidget : public QmitkDicomLocalStorageWidget
{
  Q_OBJECT
public:
  explicit DicomLocalStorageWidget(QWidget* parent = nullptr);
  void setDatabaseDirectory(QString newDataBaseDirectory);
  Q_SLOT void onStartDicomImport(const QStringList& dicomData);
  Q_SIGNAL void pushButtonCreateCaseClicked(const QStringList& dicomData);

private:
  class ctkThumbnailListWidget* mThumbnailsWidget;
  class ctkDICOMThumbnailGenerator* mThumbnailGenerator;
  class QPushButton* mPushButtonCreateCase;
  Q_SLOT void on_viewInternalDataButton_clicked(bool checked = false);
  Q_SLOT void on_pushButtonCreateCase_clicked(bool checked = false);
  Q_SLOT void onSeriesSelectionChanged(const QStringList& selection);
};

#endif  //! DICOM_LOCAL_STORAGE_WIDGET_H