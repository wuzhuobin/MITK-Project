#include "PlanningWidget.h"
#include "SegmentationWidget.h"
#include "ui_PlanningWidget.h"

// mitk
#include <mitkBoundingShapeCropper.h>
#include <mitkDataNode.h>
#include <mitkDataStorage.h>
#include <mitkGeometryData.h>
#include <mitkLogMacros.h>
#include <mitkNodePredicateDataType.h>
#include <mitkRenderingManager.h>

// qt
#include <qbuttongroup.h>
#include <qcoreevent.h>
#include <qtoolbutton.h>
#include <qwidget.h>

PlanningWidget::PlanningWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::PlanningWidget) {
  this->ui->setupUi(this);

  connect(this->ui->buttonGroup,
          QOverload<QAbstractButton *, bool>::of(&QButtonGroup::buttonToggled),
          this, &PlanningWidget::on_buttonGroup_buttonToggled);
}

PlanningWidget::~PlanningWidget() { delete this->ui; }

void PlanningWidget::changeEvent(QEvent *event) {
  QWidget::changeEvent(event);
  if (event->type() == QEvent::EnabledChange) {
    this->enablePlanningWithBounding(this->isEnabled());
  }
}

void PlanningWidget::enablePlanningWithBounding(bool enable) {

  if (!enable) {
    for (QAbstractButton *button : this->ui->buttonGroup->buttons()) {
      if (button != this->ui->pushButtonReset) {
        delete button;
      }
    }
  }

  mitk::NodePredicateDataType::Pointer predicate =
      mitk::NodePredicateDataType::New(
          mitk::GeometryData::GetStaticNameOfClass());

  mitk::DataStorage *ds =
      mitk::RenderingManager::GetInstance()->GetDataStorage();

  mitk::Image *image = ds->GetNamedObject<mitk::Image>("image");

  mitk::DataStorage::SetOfObjects::ConstPointer nodes =
      ds->GetSubset(predicate);

  for (mitk::DataNode *node : *nodes) {
    // node->SetVisibility(enable);
    if (enable) {

      mitk::GeometryData *geoData =
          dynamic_cast<mitk::GeometryData *>(node->GetData());
      if (!geoData) {
        MITK_INFO << "Failed to cast.";
        continue;
      }

      mitk::BoundingShapeCropper::Pointer cropper =
          mitk::BoundingShapeCropper::New();
      cropper->SetInput(image);
      cropper->SetUseWholeInputRegion(false);
      cropper->SetGeometry(geoData);
      cropper->Update();

      std::string name = "image" + node->GetName();

      mitk::DataNode::Pointer croppedImageNode = ds->GetNamedNode(name);
      if (!croppedImageNode) {
        croppedImageNode = mitk::DataNode::New();
        croppedImageNode->SetName(name);
        ds->Add(croppedImageNode);
      }

      croppedImageNode->SetData(cropper->GetOutput());


      QToolButton *toolButton = new QToolButton(this);
      toolButton->setCheckable(true);
      toolButton->setText(node->GetName().c_str());
      this->ui->verticalLayoutButtons->addWidget(toolButton);
      this->ui->buttonGroup->addButton(toolButton);

    }
  }
}

void PlanningWidget::on_buttonGroup_buttonToggled(QAbstractButton *button,
                                                  bool checked) {
  MITK_INFO << button->text().toStdString();
  if (button == this->ui->pushButtonReset) {
  }
}