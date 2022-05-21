/**
 * @file CupImpactionParameterGadget.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-05-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "CupImpactionParameterGadget.h"

#include "ui_CupImpactionParameterGadget.h"

// mitk
#include <mitkDataStorage.h>
#include <mitkPointSet.h>
#include <mitkRenderingManager.h>
#include <mitkSurface.h>

// itk
#include <itkCommand.h>
#include <itkEuler3DTransform.h>

CupImpactionParameterGadget::CupImpactionParameterGadget(
    const QString& acetabularShell,
    const QString& impactingAcetabularShell,
    const QString& cupCor,
    const QString& impactingAcetabularShellCor,
    QWidget* parent) :
    mAcetabularShell(acetabularShell),
    mImpactingAcetabularShell(impactingAcetabularShell),
    mCupCor(cupCor),
    mImpactingAcetabularShellCor(impactingAcetabularShellCor),
    QWidget(parent),
    mUi(new Ui::CupImpactionParameterGadget)
{
  mUi->setupUi(this);
}

CupImpactionParameterGadget::~CupImpactionParameterGadget() = default;

void CupImpactionParameterGadget::observerCupImpaction()
{
  using CupImpactionParameterGadgetCommand =
      itk::SimpleMemberCommand<CupImpactionParameterGadget>;
  auto impactingAcetabularShellCommand =
      CupImpactionParameterGadgetCommand::New();
  impactingAcetabularShellCommand->SetCallbackFunction(
      this, &CupImpactionParameterGadget::updateActualSpinBoxes);

  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* impactingAcetabularShell = ds->GetNamedObject<mitk::Surface>(
      mImpactingAcetabularShell.toStdString());
  impactingAcetabularShell->GetGeometry()
      ->GetIndexToWorldTransform()
      ->AddObserver(itk::ModifiedEvent(), impactingAcetabularShellCommand);
  impactingAcetabularShell->Modified();

  auto acetabularShellCommand = CupImpactionParameterGadgetCommand::New();
  acetabularShellCommand->SetCallbackFunction(
      this, &CupImpactionParameterGadget::updatePlannedSpinBoxes);
  auto* acetabularShell =
      ds->GetNamedObject<mitk::Surface>(mAcetabularShell.toStdString());
  acetabularShell->GetGeometry()->GetIndexToWorldTransform()->AddObserver(
      itk::ModifiedEvent(), acetabularShellCommand);
  acetabularShell->Modified();
}

void CupImpactionParameterGadget::updateActualSpinBoxes()
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* impactingAcetabularShell = ds->GetNamedObject<mitk::Surface>(
      mImpactingAcetabularShell.toStdString());
  auto matrix = impactingAcetabularShell->GetGeometry()
                    ->GetIndexToWorldTransform()
                    ->GetMatrix();

  using Euler3DTransform = itk::Euler3DTransform<mitk::ScalarType>;
  auto euler3DTransform = Euler3DTransform::New();
  euler3DTransform->SetMatrix(matrix);
  auto angleY = euler3DTransform->GetAngleY() * itk::Math::deg_per_rad;
  auto angleZ = euler3DTransform->GetAngleZ() * itk::Math::deg_per_rad;

  mUi->spinBoxActualCupInclination->setValue(angleY);
  mUi->spinBoxActualCupVersion->setValue(angleZ);

  auto* cupCor = ds->GetNamedObject<mitk::PointSet>(mCupCor.toStdString());
  auto cupCorPoint = cupCor->GetPointSet()->GetPoint(0);
  auto* impactingAcetabularShellCor = ds->GetNamedObject<mitk::PointSet>(
      mImpactingAcetabularShellCor.toStdString());
  auto impactingAcetabularShellCorPoint =
      impactingAcetabularShellCor->GetPointSet()->GetPoint(0);
  mUi->spinBoxActualDistanceRemaining->setValue(
      cupCorPoint.EuclideanDistanceTo(impactingAcetabularShellCorPoint));
}

void CupImpactionParameterGadget::updatePlannedSpinBoxes()
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* acetabularShell =
      ds->GetNamedObject<mitk::Surface>(mAcetabularShell.toStdString());
  auto matrix =
      acetabularShell->GetGeometry()->GetIndexToWorldTransform()->GetMatrix();

  using Euler3DTransform = itk::Euler3DTransform<mitk::ScalarType>;
  auto euler3DTransform = Euler3DTransform::New();
  euler3DTransform->SetMatrix(matrix);
  auto angleY = euler3DTransform->GetAngleY() * itk::Math::deg_per_rad;
  auto angleZ = euler3DTransform->GetAngleZ() * itk::Math::deg_per_rad;

  mUi->spinBoxPlannedCupInclination->setValue(angleY);
  mUi->spinBoxPlannedCupVersion->setValue(angleZ);
}