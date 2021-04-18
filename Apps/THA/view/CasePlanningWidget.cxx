#include "CasePlanningWidget.h"
#include "ui_CasePlanningWidget.h"
// qt

// mitk
#include <mitkLogMacros.h>
#include <mitkRenderingManager.h>
#include <mitkDataStorage.h>
#include <mitkSurface.h>

// vtk
#include <vtkPolyData.h>

CasePlanningWidget::CasePlanningWidget(QWidget *parent)
    : QStackedWidget(parent), ui(new Ui::CasePlanningWidget)
{
  this->ui->setupUi(this);

  QComboBox *comboBoxes[] = {
      this->ui->comboBoxCupSize,
      this->ui->comboBoxHeadSize,
      this->ui->comboBoxStemSize,
      this->ui->comboBoxSleeveSize};

  QStringList implants = {"acetabular_shell", "femoral_head", "femoral_stem", "acetabular_liner"};

  QStringList sizes = {"1.0", "1.1", "1.2", "1.3", "1.4", "1.5"};

  for (int i = 0; i < 4; ++i)
  {
    comboBoxes[i]->clear();
    for (int j = 0; j < 6; ++j)
    {
      comboBoxes[i]->addItem(sizes[j]);
    }

    connect(comboBoxes[i], &QComboBox::currentTextChanged, [=](const QString &text) {
      MITK_INFO << (implants[i] + text).toStdString();

      mitk::DataStorage *ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
      mitk::Surface *implantOld = ds->GetNamedObject<mitk::Surface>(implants[i].toStdString());
      mitk::Surface *implantNew = ds->GetNamedObject<mitk::Surface>((implants[i] + text).toStdString());
      implantOld->GetVtkPolyData()->DeepCopy(implantNew->GetVtkPolyData());
      mitk::RenderingManager::GetInstance()->RequestUpdateAll();
    });
  }
}

CasePlanningWidget::~CasePlanningWidget()
{
  delete this->ui;
}

void CasePlanningWidget::on_CasePlanningWidget_currentChanged(int index)
{
  Q_UNUSED(index);
}

void CasePlanningWidget::Action_Pelvis_CT_Landmark_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(this->ui->PelvisCTLandmark);
}

void CasePlanningWidget::Action_Implant_Planning_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(this->ui->ImplantPlanning);
}

void CasePlanningWidget::Action_Broach_Tracking_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(this->ui->BroachTracking);
}