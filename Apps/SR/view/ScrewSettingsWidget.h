/**
 * @file ScrewSettingsWidget.h
 * @author wuzhuobin (zhuobin.wu@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-06-11
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef SCREW_SETTINGS_WIDGET_H
#define SCREW_SETTINGS_WIDGET_H

// qt
#include <QWidget>

// std
#include <memory>

namespace Ui
{
class ScrewSettingsWidget;
}
class QRadioButton;
class ScrewSettingsWidget : public QWidget
{
  Q_OBJECT
public:
  explicit ScrewSettingsWidget(const QString& screwName,
                               QWidget* parent = nullptr);
  ~ScrewSettingsWidget() override;
  QRadioButton* getRadioButton() const;

  double getDiameter() const;
  void setDiameter(double diameter);
  int getLength() const;
  void setLength(int length);

private:
  std::unique_ptr<Ui::ScrewSettingsWidget> mUi;
  Q_SLOT void on_toolButtonDelete_clicked(bool checked = false);
};

#endif  //! SCREW_SETTINGS_WIDGET_H