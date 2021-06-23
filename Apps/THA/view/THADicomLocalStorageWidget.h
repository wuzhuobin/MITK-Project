#ifndef THA_DICOM_LOCAL_STORAGE_WIDGET_H
#define THA_DICOM_LOCAL_STORAGE_WIDGET_H

// mitk
#include <QmitkDicomLocalStorageWidget.h>

class THADicomLocalStorageWidget : public QmitkDicomLocalStorageWidget
{
    Q_OBJECT
public:
    explicit THADicomLocalStorageWidget(QWidget* parent = nullptr);
    void setDatabaseDirectory(QString newDataBaseDirectory);
    Q_SLOT void onStartDicomImport(const QStringList& dicomData);
};

#endif  //! THA_DICOM_LOCAL_STORAGE_WIDGET_H