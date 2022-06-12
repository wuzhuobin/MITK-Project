/**
 * @file ScrewSettingsWidget.cpp
 * @author wuzhuobin (zhuobin.wu@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-06-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "ScrewSettingsWidget.h"

#include "ui_ScrewSettingsWidget.h"

// qt
#include <QButtonGroup>

ScrewSettingsWidget::ScrewSettingsWidget(const QString& screwName,
                                         QWidget* parent) :
    QWidget(parent), mUi(std::make_unique<Ui::ScrewSettingsWidget>())
{
  mUi->setupUi(this);
  setObjectName("ScrewSettingsWidget_" + screwName);
  mUi->radioButtonScrew->setText(screwName);
}

ScrewSettingsWidget::~ScrewSettingsWidget() = default;

QRadioButton* ScrewSettingsWidget::getRadioButton() const
{
  return mUi->radioButtonScrew;
}

double ScrewSettingsWidget::getDiameter() const
{
  return mUi->doubleSpinBoxDiameter->value();
}

void ScrewSettingsWidget::setDiameter(double diameter)
{
  mUi->doubleSpinBoxDiameter->setValue(diameter);
}

int ScrewSettingsWidget::getLength() const
{
  return mUi->spinBoxLength->value();
}

void ScrewSettingsWidget::setLength(int length)
{
  mUi->spinBoxLength->setValue(length);
}

void ScrewSettingsWidget::on_toolButtonDelete_clicked(bool checked)
{
  Q_UNUSED(checked);

  delete this;
}