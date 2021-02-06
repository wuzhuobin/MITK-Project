#include "RIORegistrationWidget.h"
#include "ui_RIORegistrationWidget.h"

RIORegistrationWidget::RIORegistrationWidget(QWidget *parent)
  : QWidget(parent),
  ui(new Ui::RIORegistrationWidget)
{
  this->ui->setupUi(this);
  this->showImage(0);
}

void RIORegistrationWidget::showImage(size_t index)
{
  if (index == this->currentIndex || index > 11)
  {
    return;
  }
  this->currentIndex = index;
  QString image = QString(":/images/robot_registration/%1.png").arg(index);
  this->ui->imageViewer->setPixmap(QPixmap(image));
  this->ui->progressBarRegistration->setValue(static_cast<int>(100 * this->currentIndex / 11));
  if (this->currentIndex == 0)
  {
    this->ui->pushButtonLeft->setEnabled(false);
    this->ui->pushButtonRight->setEnabled(true);
  }
  else if (this->currentIndex == 11)
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

void RIORegistrationWidget::on_pushButtonRight_clicked(bool checked) 
{
  Q_UNUSED(checked);
  this->showImage(this->currentIndex + 1);
}


void RIORegistrationWidget::on_pushButtonLeft_clicked(bool checked)
{
  Q_UNUSED(checked);
  this->showImage(this->currentIndex - 1);
}