#include "PlanningWidget.h"
#include "SegmentationWidget.h"
#include "ui_PlanningWidget.h"

// mitk
#include <mitkBaseGeometry.h>
#include <mitkBoundingShapeCropper.h>
#include <mitkDataNode.h>
#include <mitkDataStorage.h>
#include <mitkGeometry3D.h>
#include <mitkGeometryData.h>
#include <mitkLogMacros.h>
#include <mitkNodePredicateDataType.h>
#include <mitkRenderingManager.h>

// qt
#include <mitkSurface.h>
#include <mitkVector.h>
#include <qbuttongroup.h>
#include <qcombobox.h>
#include <qcoreevent.h>
#include <qtoolbutton.h>
#include <qwidget.h>

PlanningWidget::PlanningWidget(QWidget *parent)
    : QWidget(parent), mUi(new Ui::PlanningWidget) {
  this->mUi->setupUi(this);

  connect(this->mUi->buttonGroup,
          QOverload<QAbstractButton *, bool>::of(&QButtonGroup::buttonToggled),
          this, &PlanningWidget::on_buttonGroup_buttonToggled);

  this->mUi->pushButtonReset->setVisible(false);

  connect(this->mUi->comboBoxLeft, QOverload<int>::of(&QComboBox::activated),
          this, &PlanningWidget::on_comboBoxLeft_activated);
  connect(this->mUi->comboBoxRight, QOverload<int>::of(&QComboBox::activated),
          this, &PlanningWidget::on_comboBoxRight_activated);
}

PlanningWidget::~PlanningWidget() { delete this->mUi; }

void PlanningWidget::changeEvent(QEvent *event) {
  QWidget::changeEvent(event);
  if (event->type() == QEvent::EnabledChange) {
    this->enablePlanningWithBounding(this->isEnabled());
  }
}

void PlanningWidget::enablePlanningWithBounding(bool enable) {

  for (QAbstractButton *button : this->mUi->buttonGroup->buttons()) {
    if (button != this->mUi->pushButtonReset) {
      this->mUi->buttonGroup->removeButton(button);
      delete button;
    }
  }
  this->mUi->comboBoxLeft->setEnabled(false);
  this->mUi->comboBoxRight->setEnabled(false);
  if (!enable) {
    return;
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
    this->mUi->verticalLayoutButtons->addWidget(toolButton);
    this->mUi->buttonGroup->addButton(toolButton);
  }
}

void PlanningWidget::on_buttonGroup_buttonToggled(QAbstractButton *button,
                                                  bool checked) {

  mitk::DataStorage *ds =
      mitk::RenderingManager::GetInstance()->GetDataStorage();
  mitk::DataStorage::SetOfObjects::ConstPointer nodes = ds->GetAll();
  mitk::DataNode *image = nullptr;

  for (mitk::DataNode *node : *nodes) {
    node->SetVisibility(false);
  }

  if (button == this->mUi->pushButtonReset) {

    image = ds->GetNamedNode("image");
    image->SetVisibility(true);
    this->mUi->comboBoxLeft->setEnabled(false);
    this->mUi->comboBoxRight->setEnabled(false);

  } else {

    std::string name = "image" + button->text().toStdString();
    image = ds->GetNamedNode(name);
    image->SetVisibility(true);
    this->mUi->comboBoxLeft->setEnabled(true);
    this->mUi->comboBoxRight->setEnabled(true);
  }

  mitk::RenderingManager::GetInstance()->InitializeViews(
      image->GetData()->GetGeometry());
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void PlanningWidget::on_comboBoxRight_activated(int index) {
  if (index == 0) {
    return;
  }
  QString type = "pin1.";
  type += QString::number(index - 1);

  this->addPin(PIN_RIGHT, type);
}

void PlanningWidget::on_comboBoxLeft_activated(int index) {
  if (index == 0) {
    return;
  }

  QString type = "pin1.";
  type += QString::number(index - 1);
  MITK_INFO << type.toStdString();

  this->addPin(PIN_LEFT, type);
}

void PlanningWidget::addPin(QString base, QString typeName) {

  mitk::DataStorage *ds =
      mitk::RenderingManager::GetInstance()->GetDataStorage();

  mitk::Surface *type =
      ds->GetNamedObject<mitk::Surface>(typeName.toStdString());

  mitk::DataNode::Pointer pinNode = ds->GetNamedNode(base.toStdString());
  mitk::Surface::Pointer pin = type->Clone();

  if (pinNode == nullptr) {

    QString geometryDataName = this->mUi->buttonGroup->checkedButton()->text();
    mitk::GeometryData *geometryData =
        ds->GetNamedObject<mitk::GeometryData>(geometryDataName.toStdString());
    mitk::Point3D point = geometryData->GetGeometry()->GetCenter();
    mitk::Vector3D translate;
    translate[0] = point[0];
    translate[1] = point[1];
    translate[2] = point[2];

    pin->GetGeometry()->Translate(translate);

    pinNode = mitk::DataNode::New();
    pinNode->SetName(base.toStdString());
    ds->Add(pinNode);
  } else {
    pin->SetGeometry(pinNode->GetData()->GetGeometry());
  }

  pinNode->SetData(pin);

  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}