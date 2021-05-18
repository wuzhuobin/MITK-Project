#include "SRStdMultiWidget.h"
#include "GroupBoxGadget.h"

// qt
#include <qgridlayout.h>

// mitk
#include <QmitkRenderWindow.h>

SRStdMultiWidget::SRStdMultiWidget(QWidget *parent, Qt::WindowFlags f,
                                   const QString &name)
    : QmitkStdMultiWidget(parent, f, name),
      groupBoxGadget{
          new GroupBoxGadget(GroupBoxGadget::Orientation::AXIAL, this),
          new GroupBoxGadget(GroupBoxGadget::Orientation::SAGITTAL, this),
          new GroupBoxGadget(GroupBoxGadget::Orientation::CORONAL, this),
          new GroupBoxGadget(GroupBoxGadget::Orientation::CORONAL, this)} {
              this->setGroupBoxVisible(false);
}

void SRStdMultiWidget::InitializeMultiWidget() {
  this->SetDataStorage(mitk::RenderingManager::GetInstance()->GetDataStorage());
  QmitkStdMultiWidget::InitializeMultiWidget();
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();

  for (int i = 0; i < 4; ++i) {
    QmitkRenderWindow *renderWindow = this->GetRenderWindow(i);
    QGridLayout *pRenderWindow1Layout = new QGridLayout(renderWindow);
    pRenderWindow1Layout->addWidget(this->groupBoxGadget[i], 0, 1,
                                    Qt::AlignRight | Qt::AlignTop);
    renderWindow->setLayout(pRenderWindow1Layout);
  }
}

void SRStdMultiWidget::setGroupBoxVisible(bool flag) {
  for (int i = 0; i < 4; ++i) {
    this->groupBoxGadget[i]->setVisible(flag);
  }
}