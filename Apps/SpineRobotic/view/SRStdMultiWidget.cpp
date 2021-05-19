#include "SRStdMultiWidget.h"
#include "GroupBoxGadget.h"

// qt
#include <mitkDataStorage.h>
#include <mitkRenderingManager.h>
#include <qgridlayout.h>

// mitk
#include <QmitkRenderWindow.h>
#include <mitkDataNode.h>
#include <mitkPlanarLine.h>

SRStdMultiWidget::SRStdMultiWidget(QWidget *parent, Qt::WindowFlags f,
                                   const QString &name)
    : QmitkStdMultiWidget(parent, f, name),
      groupBoxGadget{
          new GroupBoxGadget(GroupBoxGadget::Orientation::AXIAL, this),
          new GroupBoxGadget(GroupBoxGadget::Orientation::SAGITTAL, this),
          new GroupBoxGadget(GroupBoxGadget::Orientation::CORONAL, this),
          new GroupBoxGadget(GroupBoxGadget::Orientation::CORONAL, this)} {
  this->enableGroupBox(false);
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

void SRStdMultiWidget::enableGroupBox(bool flag) {
  for (int i = 0; i < 4; ++i) {
    this->groupBoxGadget[i]->setVisible(flag);
  }
}

void SRStdMultiWidget::enablePlanarLine(bool flag) {

  mitk::DataNode::Pointer planarLineNode = nullptr;
  mitk::DataStorage *ds =
      mitk::RenderingManager::GetInstance()->GetDataStorage();
  planarLineNode = ds->GetNamedNode("planar_line");
  if (!planarLineNode) {
    planarLineNode = mitk::DataNode::New();
    planarLineNode->SetName("planar_line");
    planarLineNode->SetData(mitk::PlanarLine::New());
  }
  planarLineNode->SetVisibility(flag);
}
