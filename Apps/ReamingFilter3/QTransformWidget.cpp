#include "QTransformWidget.hpp"

#include "ui_QTransformWidget.h"

// vtk
#include <vtkTransform.h>

// mitk
#include <mitkLogMacros.h>
#include <mitkRenderingManager.h>

QTransformWidget::QTransformWidget(vtkSmartPointer<vtkTransform> transform,
                                   std::function<void()> onTransformChanged,
                                   QWidget* parent) :
    QWidget(parent),
    mTransform(transform),
    mOnTransformChanged(std::move(onTransformChanged)),
    mUi(std::make_unique<Ui::QTransformWidget>())
{
  mUi->setupUi(this);
}

QTransformWidget::~QTransformWidget() = default;

void QTransformWidget::on_toolButtonXAdd_clicked(bool checked)
{
  mTransform->Translate(10, 0, 0);
  mOnTransformChanged();
}

void QTransformWidget::on_toolButtonXSubstract_clicked(bool checked)
{
  mTransform->Translate(-10, 0, 0);
  mOnTransformChanged();
}

void QTransformWidget::on_toolButtonYAdd_clicked(bool checked)
{
  mTransform->Translate(0, 10, 0);
  mOnTransformChanged();
}

void QTransformWidget::on_toolButtonYSubstract_clicked(bool checked)
{
  mTransform->Translate(0, -10, 0);
  mOnTransformChanged();
}

void QTransformWidget::on_toolButtonZAdd_clicked(bool checked)
{
  mTransform->Translate(0, 0, 10);
  mOnTransformChanged();
}

void QTransformWidget::on_toolButtonZSubstract_clicked(bool checked)
{
  mTransform->Translate(0, 0, -10);
  mOnTransformChanged();
}