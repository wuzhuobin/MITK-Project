#ifndef IMAGE_WIDGET_H
#define IMAGE_WIDGET_H
// qt
#include <QWidget>
namespace Ui {
  class ImageWidget;
}
class ImageWidget: public QWidget
{
  Q_OBJECT;
public:
  enum class MODE
  {
	PELVIS_CHECKPOINT = 0,
	PELVIS_LANDMARK,
	PELVIS_REGISTRATION,
	RIO_REGISTRATION
  };
  static constexpr size_t NUM_OF_RIO_REGISTRATION_IMAGES = 12;
  static constexpr size_t NUM_OF_PELVIS_CHECKPOINT_IMAGES = 1;
  explicit ImageWidget(QWidget *parent = nullptr);
  void setMode(MODE mode);
  MODE getMode() const;

private:
  Ui::ImageWidget *ui; 
  size_t currentIndex = NUM_OF_RIO_REGISTRATION_IMAGES;
  MODE mode = MODE::PELVIS_CHECKPOINT;
  Q_DISABLE_COPY(ImageWidget);
  void showImage(size_t index);
private Q_SLOTS:
  void on_pushButtonRight_clicked(bool checked = false); 
  void on_pushButtonLeft_clicked(bool checked = false);
};

#endif //!IMAGE_WIDGET_H