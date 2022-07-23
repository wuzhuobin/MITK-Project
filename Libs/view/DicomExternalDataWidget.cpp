#include <DicomExternalDataWidget.h>
#include <qwidget.h>

DicomExternalDataWidget::DicomExternalDataWidget(QWidget* parent) :
    QmitkDicomExternalDataWidget(parent)
{
  m_Controls->viewExternalDataButton->setVisible(false);
}