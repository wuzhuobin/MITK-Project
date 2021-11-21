#include "ImageWidget.h"

#include <qglobal.h>

#include "ui_ImageWidget.h"

const struct ImageWidgetInit
{
  ImageWidgetInit() { Q_INIT_RESOURCE(ImageWidget); }
} GInit;

ImageWidget::ImageWidget(QWidget* parent) :
    QWidget(parent), mUi(new Ui::ImageWidget)
{
  this->mUi->setupUi(this);
  this->setMode(Mode::PelvisCheckpoint);
}

void ImageWidget::setMode(Mode mode)
{
  this->mode = mode;
  this->showImage(0);
}

ImageWidget::Mode ImageWidget::getMode() const
{
  return this->mode;
}

void ImageWidget::showImage(size_t index)
{
  size_t num;
  QString path;
  switch (this->mode)
  {
    case Mode::PelvisCheckpoint:
      num = NUM_OF_PELVIS_CHECKPOINT_IMAGES;
      path = QStringLiteral(":/resource/images/pelvis_checkpoint/%1.png");
      this->mUi->progressBarImage->setVisible(false);
      this->mUi->pushButtonLeft->setVisible(false);
      this->mUi->pushButtonRight->setVisible(false);
      break;
    case Mode::FemurLandmark:
      num = NUM_OF_FEMUR_LANDMARK_IMAGES;
      path = QStringLiteral(":/resource/images/femur_landmark/%1.png");
    case Mode::RioRegistration:
      num = NUM_OF_RIO_REGISTRATION_IMAGES;
      path = QStringLiteral(":/resource/images/rio_registration/%1.png");
      this->mUi->progressBarImage->setVisible(true);
      this->mUi->pushButtonLeft->setVisible(true);
      this->mUi->pushButtonRight->setVisible(true);
      break;
    default:
      break;
  }
  if (index > num - 1)
  {
    index = num - 1;
  }
  this->currentIndex = index;
  QString image = path.arg(index);
  this->mUi->imageViewer->setPixmap(QPixmap(image));
  this->mUi->progressBarImage->setValue(
      static_cast<int>(100 * (this->currentIndex + 1) / num));
  if (this->currentIndex == 0)
  {
    this->mUi->pushButtonLeft->setEnabled(false);
    this->mUi->pushButtonRight->setEnabled(true);
  }
  else if (this->currentIndex == num - 1)
  {
    this->mUi->pushButtonLeft->setEnabled(true);
    this->mUi->pushButtonRight->setEnabled(false);
  }
  else
  {
    this->mUi->pushButtonLeft->setEnabled(true);
    this->mUi->pushButtonRight->setEnabled(true);
  }
}

void ImageWidget::on_pushButtonRight_clicked(bool checked)
{
  Q_UNUSED(checked);
  this->showImage(this->currentIndex + 1);
}

void ImageWidget::on_pushButtonLeft_clicked(bool checked)
{
  Q_UNUSED(checked);
  this->showImage(this->currentIndex - 1);
}