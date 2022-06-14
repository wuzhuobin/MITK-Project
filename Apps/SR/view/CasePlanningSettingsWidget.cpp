/**
 * @file CasePlanningSettingsWidget.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-06-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "CasePlanningSettingsWidget.h"

#include "ui_CasePlanningSettingsWidget.h"

// qt
#include <QRadioButton>

// mitk
#include <mitkDataStorage.h>
#include <mitkLogMacros.h>
#include <mitkRenderingManager.h>

CasePlanningSettingsWidget::CasePlanningSettingsWidget(
    const QString& objectName, QWidget* parent) :
    QWidget(parent), mUi(std::make_unique<Ui::CasePlanningSettingsWidget>())
{
  mUi->setupUi(this);
  setObjectName("CasePlanningSettingsWidget_" + objectName);
  mUi->radioButton->setText(objectName);
}

CasePlanningSettingsWidget::~CasePlanningSettingsWidget() = default;

QRadioButton* CasePlanningSettingsWidget::getRadioButton() const
{
  return mUi->radioButton;
}

const QString CasePlanningSettingsWidget::getCasePlanningName() const
{
  return mUi->radioButton->text();
}

void CasePlanningSettingsWidget::on_toolButtonDelete_clicked(bool checked)
{
  Q_UNUSED(checked);
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

  ds->Remove(ds->GetNamedNode(getCasePlanningName().toStdString()));
  mitk::RenderingManager::GetInstance()->InitializeViewsByBoundingObjects(ds);
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
  deleteLater();
}

void CasePlanningSettingsWidget::on_toolButtonHide_toggled(bool checked)
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* caseplanningNode =
      ds->GetNamedNode(getCasePlanningName().toStdString());
  caseplanningNode->SetVisibility(!checked);

  mitk::RenderingManager::GetInstance()->InitializeViewsByBoundingObjects(ds);
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}