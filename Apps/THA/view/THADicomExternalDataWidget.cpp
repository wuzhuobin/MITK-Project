#include <THADicomExternalDataWidget.h>
#include <qwidget.h>

THADicomExternalDataWidget::THADicomExternalDataWidget(QWidget* parent) : QmitkDicomExternalDataWidget(parent)
{
    m_Controls->viewExternalDataButton->setVisible(false);
}