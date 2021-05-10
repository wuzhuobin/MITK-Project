#include "SegmentationWidget.h"
#include "ui_SegmentationWidget.h"

// mitk
#include <mitkDataNode.h>
#include <mitkDataStorage.h>
#include <mitkGeometry3D.h>
#include <mitkGeometryData.h>
#include <mitkImage.h>
#include <mitkLogMacros.h>
#include <mitkRenderingManager.h>
#include <usModuleRegistry.h>

#include <mitkPointSet.h>
#include <mitkPointSetDataInteractor.h>

SegmentationWidget::SegmentationWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::SegmentationWidget),
      boundingShapeInteractor(mitk::BoundingShapeInteractor::New()) {
  this->ui->setupUi(this);

  connect(this->ui->pushButtonMarkerSegmentation, &QPushButton::clicked, this,
          &SegmentationWidget::on_pushButtonMarkerSegmentation_clicked);
  connect(this->ui->pushButtonLabeling, &QPushButton::clicked, this,
          &SegmentationWidget::on_pushButtonLabeling_clicked);
}

SegmentationWidget::~SegmentationWidget() { delete this->ui; }

void SegmentationWidget::on_pushButtonMarkerSegmentation_clicked(bool checked) {

  mitk::DataStorage *ds =
      mitk::RenderingManager::GetInstance()->GetDataStorage();

  // Create PointSet and a node for it
  mitk::PointSet::Pointer pointSet = mitk::PointSet::New();
  mitk::DataNode::Pointer pointSetNode = mitk::DataNode::New();
  // Store the point set in the DataNode
  pointSetNode->SetData(pointSet);

  // Add the node to the tree
  ds->Add(pointSetNode);

  // Create PointSetDataInteractor
  mitk::PointSetDataInteractor::Pointer interactor = mitk::PointSetDataInteractor::New();
  // Set the StateMachine pattern that describes the flow of the interactions
  interactor->LoadStateMachine("PointSet.xml");
  // Set the configuration file, which describes the user interactions that trigger actions
  // in this file SHIFT + LeftClick triggers add Point, but by modifying this file,
  // it could as well be changes to any other user interaction.
  interactor->SetEventConfig("PointSetConfig.xml");

  // Assign the pointSetNode to the interactor,
  // alternatively one could also add the DataInteractor to the pointSetNode using the SetDataInteractor() method.
  interactor->SetDataNode(pointSetNode);
  interactor->EnableInteraction(true);


  // mitk::Image *img2 = ds->GetNamedObject<mitk::Image>("img2");

  // auto geometry = img2->GetGeometry();
  auto boundingGeometry = mitk::Geometry3D::New();
  // boundingGeometry->SetBounds(geometry->GetBounds());
  // boundingGeometry->SetImageGeometry(geometry->GetImageGeometry());
  // boundingGeometry->SetOrigin(geometry->GetOrigin());
  // boundingGeometry->SetSpacing(geometry->GetSpacing());
  // boundingGeometry->SetIndexToWorldTransform(geometry->GetIndexToWorldTransform()->Clone());
  // boundingGeometry->Modified();

  mitk::GeometryData::Pointer geoData = mitk::GeometryData::New();
  // geoData->SetGeometry(img2->GetGeometry()->Clone());
  geoData->SetGeometry(boundingGeometry);

  mitk::DataNode::Pointer geoDataNode = mitk::DataNode::New();
  geoDataNode->SetData(geoData);
  // geoDataNode->SetName("img2Geo");
  // //   geoDataNode->SetProperty("name",
  // //   mitk::StringProperty::New(name.toStdString()));
  // geoDataNode->SetProperty("color", mitk::ColorProperty::New(1.0, 1.0, 1.0));
  // geoDataNode->SetProperty("opacity", mitk::FloatProperty::New(0.6));
  // geoDataNode->SetProperty("layer", mitk::IntProperty::New(99));
  // geoDataNode->AddProperty("handle size factor",
  //                          mitk::DoubleProperty::New(1.0 / 40.0));
  // geoDataNode->SetBoolProperty("pickable", true);
  // geoDataNode->SetVisibility(true);

  ds->Add(geoDataNode);

  this->boundingShapeInteractor->LoadStateMachine(
      "BoundingShapeInteraction.xml",
      us::ModuleRegistry::GetModule("MitkBoundingShape"));
  this->boundingShapeInteractor->SetEventConfig(
      "BoundingShapeMouseConfig.xml",
      us::ModuleRegistry::GetModule("MitkBoundingShape"));
  this->boundingShapeInteractor->EnableInteraction(true);
  this->boundingShapeInteractor->SetDataNode(geoDataNode);

  // mitk::RenderingManager::GetInstance()->RequestUpdateAll();

  MITK_INFO << *geoData;
  // MITK_INFO << *this->boundingShapeInteractor;

  
}

void SegmentationWidget::on_pushButtonLabeling_clicked(bool checked) {
  MITK_INFO << __PRETTY_FUNCTION__;
}