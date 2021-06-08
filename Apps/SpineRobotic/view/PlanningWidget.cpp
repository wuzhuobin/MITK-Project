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
#include <qcombobox.h>
#include <qcoreevent.h>
#include <qtoolbutton.h>
#include <qwidget.h>

PlanningWidget::PlanningWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::PlanningWidget) {
  this->ui->setupUi(this);

  connect(this->ui->buttonGroup,
          QOverload<QAbstractButton *, bool>::of(&QButtonGroup::buttonToggled),
          this, &PlanningWidget::on_buttonGroup_buttonToggled);

  this->ui->pushButtonReset->setVisible(false);

  connect(this->ui->comboBoxLeft, QOverload<int>::of(&QComboBox::activated),
          this, &PlanningWidget::on_comboBoxLeft_activated);
  connect(this->ui->comboBoxRight, QOverload<int>::of(&QComboBox::activated),
          this, &PlanningWidget::on_comboBoxRight_activated);
}

PlanningWidget::~PlanningWidget() { delete this->ui; }

void PlanningWidget::changeEvent(QEvent *event) {
  QWidget::changeEvent(event);
  if (event->type() == QEvent::EnabledChange) {
    this->enablePlanningWithBounding(this->isEnabled());
  }
}

void PlanningWidget::enablePlanningWithBounding(bool enable) {

  for (QAbstractButton *button : this->ui->buttonGroup->buttons()) {
    if (button != this->ui->pushButtonReset) {
      this->ui->buttonGroup->removeButton(button);
      delete button;
    }
  }
  this->ui->comboBoxLeft->setEnabled(false);
  this->ui->comboBoxRight->setEnabled(false);
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
    this->ui->verticalLayoutButtons->addWidget(toolButton);
    this->ui->buttonGroup->addButton(toolButton);
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

  if (button == this->ui->pushButtonReset) {

    image = ds->GetNamedNode("image");
    image->SetVisibility(true);
    this->ui->comboBoxLeft->setEnabled(false);
    this->ui->comboBoxRight->setEnabled(false);

  } else {

    std::string name = "image" + button->text().toStdString();
    image = ds->GetNamedNode(name);
    image->SetVisibility(true);
    this->ui->comboBoxLeft->setEnabled(true);
    this->ui->comboBoxRight->setEnabled(true);
  }

  mitk::RenderingManager::GetInstance()->InitializeViews(
      image->GetData()->GetGeometry());
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void PlanningWidget::on_comboBoxRight_activated(int index) {
  if (index == 0) {
    return;
  }

  QAbstractButton *button = this->ui->buttonGroup->checkedButton();
  QString base = PIN_RIGHT + button->text();

  QString type = this->ui->comboBoxRight->itemText(index);
  this->addPin(base, type);
}

void PlanningWidget::on_comboBoxLeft_activated(int index) {}

void PlanningWidget::addPin(QString base, QString type)
{
  MITK_INFO << "Base: " << base.toStdString();
  MITK_INFO << "Type: " << type.toStdString();

  mitk::DataStorage *ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  
  
  QString name = base + "_" + type;
}