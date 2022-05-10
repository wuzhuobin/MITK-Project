/**
 * @file QTransformWidget.hpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-05-08
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef Q_TRANSFORM_WIDGET_HPP
#define Q_TRANSFORM_WIDGET_HPP

// qt
#include <QWidget>

// std
#include <functional>
#include <memory>

// vtk
#include <vtkSmartPointer.h>

namespace Ui
{
class QTransformWidget;
}
class vtkTransform;
class QTransformWidget : public QWidget
{
  Q_OBJECT
public:
  explicit QTransformWidget(vtkSmartPointer<vtkTransform> transform,
                            std::function<void()> onTransformChanged,
                            QWidget* parent = nullptr);
  ~QTransformWidget() override;

private:
  std::unique_ptr<Ui::QTransformWidget> mUi;
  vtkSmartPointer<vtkTransform> mTransform;
  std::function<void()> mOnTransformChanged;

private Q_SLOTS:
  void on_toolButtonXAdd_clicked(bool checked = false);
  void on_toolButtonXSubstract_clicked(bool checked = false);
  void on_toolButtonYAdd_clicked(bool checked = false);
  void on_toolButtonYSubstract_clicked(bool checked = false);
  void on_toolButtonZAdd_clicked(bool checked = false);
  void on_toolButtonZSubstract_clicked(bool checked = false);
};

#endif  // Q_TRANSFORM_WIDGET_HPP