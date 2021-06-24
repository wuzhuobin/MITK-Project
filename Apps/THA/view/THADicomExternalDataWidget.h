#ifndef THA_DICOM_EXTERNAL_DATA_WIDGET_H
#define THA_DICOM_EXTERNAL_DATA_WIDGET_H

// mitk
#include <QmitkDicomExternalDataWidget.h>

class THADicomExternalDataWidget : public QmitkDicomExternalDataWidget
{
    Q_OBJECT;

public:
    THADicomExternalDataWidget(QWidget* parent = nullptr) : QmitkDicomExternalDataWidget(parent) {}
};

#endif  // ! THA_DICOM_EXTERNAL_DATA_WIDGET_H