#ifndef FEMORAL_PREP_WIDGET_H
#define FEMORAL_PREP_WIDGET_H

// std
#include <memory>

// qt
#include <QStackedWidget>

namespace Ui
{
class FemoralPrepWidget;
}

class FemoralPrepWidget : public QStackedWidget
{
  Q_OBJECT
public:
  explicit FemoralPrepWidget(QWidget* parent = nullptr);
  ~FemoralPrepWidget() override;

  Q_SLOT void action_Femoral_Landmark_triggered(bool checked = false);
  Q_SLOT void action_Femoral_Checkpoint_triggered(bool checked = false);
  Q_SLOT void action_Femoral_Registration_triggered(bool checked = false);
  Q_SLOT void action_Neck_Resection_Guide_triggered(bool checked = false);
  Q_SLOT void action_Broach_Tracking_triggered(bool checked = false);
  Q_SIGNAL void checkBoxProximalCheckpointToggled(bool checked);
  Q_SIGNAL void checkBoxDistalCheckpointToggled(bool checked);

private:
  std::unique_ptr<Ui::FemoralPrepWidget> mUi;
  bool mProximalCheckpointCaptured = false;
  bool mDistalCheckpointCaptured = false;

  Q_SLOT void on_checkBoxProximalCheckpoint_toggled(bool checked);
  Q_SLOT void on_checkBoxDistalCheckpoint_toggled(bool checked);
  Q_SLOT void on_pushButtonFemoralLandmarkCapture_toggled(bool checked);
  Q_SLOT void on_pushButtonFemoralLandmarkClearAll_toggled(bool checked);
};

#endif  //! FEMORAL_PREP_WIDGET_H