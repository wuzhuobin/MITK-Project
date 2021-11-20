#ifndef IMAGE_WIDGET_H
#define IMAGE_WIDGET_H
// qt
#include <QWidget>
namespace Ui
{
class ImageWidget;
}
class ImageWidget : public QWidget
{
  Q_OBJECT;

public:
  enum class Mode
  {
    PelvisCheckpoint = 0,
    PelvisLandmark,
    PelvisRegistration,
    FemurLandmark,
    RioRegistration
  };
  static constexpr size_t NUM_OF_RIO_REGISTRATION_IMAGES = 12;
  static constexpr size_t NUM_OF_PELVIS_CHECKPOINT_IMAGES = 1;
  static constexpr size_t NUM_OF_FEMUR_LANDMARK_IMAGES = 2;
  explicit ImageWidget(QWidget* parent = nullptr);
  void setMode(Mode mode);
  Mode getMode() const;

private:
  Ui::ImageWidget* mUi;
  size_t currentIndex = NUM_OF_RIO_REGISTRATION_IMAGES;
  Mode mode = Mode::PelvisCheckpoint;
  Q_DISABLE_COPY(ImageWidget);
  void showImage(size_t index);
  Q_SLOT void on_pushButtonRight_clicked(bool checked = false);
  Q_SLOT void on_pushButtonLeft_clicked(bool checked = false);
};

#endif  //! IMAGE_WIDGET_H