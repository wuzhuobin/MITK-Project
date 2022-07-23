#ifndef DICOM_EXTERNAL_DATA_WIDGET_H
#define DICOM_EXTERNAL_DATA_WIDGET_H

// mitk
#include <QmitkDicomExternalDataWidget.h>

class DicomExternalDataWidget : public QmitkDicomExternalDataWidget
{
  Q_OBJECT;

public:
  DicomExternalDataWidget(QWidget* parent = nullptr);
};

#endif  // ! DICOM_EXTERNAL_DATA_WIDGET_H