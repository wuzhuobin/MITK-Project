#include "SegmentationWidget.h"
#include "ui_SegmentationWidget.h"

// mitk
#include <mitkBoundingShapeObjectFactory.h>
#include <mitkDataNode.h>
#include <mitkDataStorage.h>
#include <mitkGeometry3D.h>
#include <mitkGeometryData.h>
#include <mitkImage.h>
#include <mitkLogMacros.h>
#include <mitkRenderingManager.h>
#include <usModuleRegistry.h>

// qt
#include <QButtonGroup>
#include <QToolButton>

SegmentationWidget::SegmentationWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::SegmentationWidget),
      boundingShapeInteractor(mitk::BoundingShapeInteractor::New()) {

  this->ui->setupUi(this);

  this->buttonGroup = new QButtonGroup(this);
  this->buttonGroup->setExclusive(true);

  for (int i = 0; i < 7; ++i) {
    QToolButton *toolButton = new QToolButton;
    toolButton->setObjectName("toolButtonC" + QString::number(i + 1));
    toolButton->setText("C" + QString::number(i + 1));
    toolButton->setCheckable(true);

    this->ui->gridLayoutCervial->addWidget(toolButton, i / 4, i % 4);
    this->buttonGroup->addButton(toolButton);
  }

  for (int i = 0; i < 12; ++i) {
    QToolButton *toolButton = new QToolButton;
    toolButton->setObjectName("toolButtonT" + QString::number(i + 1));
    toolButton->setText("T" + QString::number(i + 1));
    toolButton->setCheckable(true);

    this->ui->gridLayoutThracic->addWidget(toolButton, i / 4, i % 4);
    this->buttonGroup->addButton(toolButton);
  }

  for (int i = 0; i < 5; ++i) {
    QToolButton *toolButton = new QToolButton;
    toolButton->setObjectName("toolButtonL" + QString::number(i + 1));
    toolButton->setText("L" + QString::number(i + 1));
    toolButton->setCheckable(true);

    this->ui->gridLayoutLumbar->addWidget(toolButton, i / 3, i % 3);
    this->buttonGroup->addButton(toolButton);
  }

  for (int i = 0; i < 5; ++i) {
    QToolButton *toolButton = new QToolButton;
    toolButton->setObjectName("toolButtonS" + QString::number(i + 1));
    toolButton->setText("S" + QString::number(i + 1));
    toolButton->setCheckable(true);

    this->ui->gridLayoutSacral->addWidget(toolButton, i / 3, i % 3);
    this->buttonGroup->addButton(toolButton);
  }

  mitk::RegisterBoundingShapeObjectFactory();
  this->boundingShapeInteractor->LoadStateMachine(
      "BoundingShapeInteraction.xml",
      us::ModuleRegistry::GetModule("MitkBoundingShape"));
  this->boundingShapeInteractor->SetEventConfig(
      "BoundingShapeMouseConfig.xml",
      us::ModuleRegistry::GetModule("MitkBoundingShape"));

  connect(this->ui->pushButtonMarkerSegmentation, &QPushButton::clicked, this,
          &SegmentationWidget::on_pushButtonMarkerSegmentation_clicked);
  connect(this->ui->pushButtonLabeling, &QPushButton::clicked, this,
          &SegmentationWidget::on_pushButtonLabeling_clicked);

  connect(this->ui->stackedWidget, &QStackedWidget::currentChanged, this,
          &SegmentationWidget::on_stackedWidget_currentChanged);

  connect(this->buttonGroup,
          static_cast<void (QButtonGroup::*)(QAbstractButton *, bool)>(
              &QButtonGroup::buttonToggled),
          this, &SegmentationWidget::onButtonToggled);
  this->ui->stackedWidget->setCurrentWidget(this->ui->pageMarkerSegmentation);
}

SegmentationWidget::~SegmentationWidget() { delete this->ui; }

void SegmentationWidget::on_pushButtonMarkerSegmentation_clicked(bool checked) {

  this->ui->stackedWidget->setCurrentWidget(this->ui->pageMarkerSegmentation);
}

void SegmentationWidget::on_pushButtonLabeling_clicked(bool checked) {

  this->ui->stackedWidget->setCurrentWidget(this->ui->pageLabel);
}

void SegmentationWidget::on_stackedWidget_currentChanged(int index) {

  this->buttonGroup->setExclusive(false);
  for (auto button : this->buttonGroup->buttons()) {
    button->setChecked(false);
  }

  if (this->ui->stackedWidget->widget(index) ==
      this->ui->pageMarkerSegmentation) {
    this->buttonGroup->setExclusive(true);
  }
}

void SegmentationWidget::onButtonToggled(QAbstractButton *button,
                                         bool checked) {

  std::string name = button->objectName().remove("toolButton").toStdString();
  name = std::string("image") + name;

  mitk::DataStorage *ds =
      mitk::RenderingManager::GetInstance()->GetDataStorage();

  mitk::DataNode::Pointer geoDataNode = ds->GetNamedNode(name);

  if (!geoDataNode) {

    mitk::Image *image = ds->GetNamedObject<mitk::Image>("image");

    mitk::GeometryData::Pointer geoData = mitk::GeometryData::New();
    geoData->SetGeometry(image->GetGeometry());

    geoDataNode = mitk::DataNode::New();
    geoDataNode->SetName(name);
    geoDataNode->SetData(geoData);

    ds->Add(geoDataNode);
  }

  geoDataNode->SetVisibility(checked);
  this->boundingShapeInteractor->EnableInteraction(checked);
  this->boundingShapeInteractor->SetDataNode(checked ? geoDataNode : nullptr);

  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}