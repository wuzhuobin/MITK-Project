#include "ImageWidget.h"
#include <qglobal.h>
#include "ui_ImageWidget.h"

const struct ImageWidgetInit
{
  ImageWidgetInit() {
    Q_INIT_RESOURCE(ImageWidget);
  }
} GInit;

ImageWidget::ImageWidget(QWidget *parent)
  : QWidget(parent),
  ui(new Ui::ImageWidget)
{
  this->ui->setupUi(this);
  this->SetMode(MODE::PELVIS_CHECKPOINT);
}

void ImageWidget::SetMode(MODE mode)
{
  this->mode = mode;
  this->showImage(0);
}

ImageWidget::MODE ImageWidget::GetMode() const
{
  return this->mode;
}

void ImageWidget::showImage(size_t index)
{
  size_t num;
  QString path;
  switch (this->mode)
  {
  case MODE::PELVIS_CHECKPOINT:
    num = NUM_OF_PELVIS_CHECKPOINT_IMAGES;
    path = QStringLiteral(":/resource/images/pelvis_checkpoint/%1.png");
    this->ui->progressBarImage->setVisible(false);
    this->ui->pushButtonLeft->setVisible(false);
    this->ui->pushButtonRight->setVisible(false);
    break;
  case MODE::RIO_REGISTRATION:
    num = NUM_OF_RIO_REGISTRATION_IMAGES;
    path = QStringLiteral(":/resource/images/rio_registration/%1.png");
    this->ui->progressBarImage->setVisible(true);
    this->ui->pushButtonLeft->setVisible(true);
    this->ui->pushButtonRight->setVisible(true);
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
  this->ui->imageViewer->setPixmap(QPixmap(image));
  this->ui->progressBarImage->setValue(static_cast<int>(100 * (this->currentIndex + 1) / num));
  if (this->currentIndex == 0)
  {
    this->ui->pushButtonLeft->setEnabled(false);
    this->ui->pushButtonRight->setEnabled(true);
  }
  else if (this->currentIndex == num - 1)
  {
    this->ui->pushButtonLeft->setEnabled(true);
    this->ui->pushButtonRight->setEnabled(false);
  }
  else
  {
    this->ui->pushButtonLeft->setEnabled(true);
    this->ui->pushButtonRight->setEnabled(true);
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