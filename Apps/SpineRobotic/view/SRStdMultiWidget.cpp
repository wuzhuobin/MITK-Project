#include "SRStdMultiWidget.h"

SRStdMultiWidget::SRStdMultiWidget(QWidget *parent, Qt::WindowFlags f, const QString &name)
    : QmitkStdMultiWidget(parent, f, name)
{ 
}

void SRStdMultiWidget::InitializeMultiWidget()
{
    this->SetDataStorage(mitk::RenderingManager::GetInstance()->GetDataStorage());
    QmitkStdMultiWidget::InitializeMultiWidget();
    mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}
