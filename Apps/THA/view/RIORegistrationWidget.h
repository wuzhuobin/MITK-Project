#ifndef RIO_REGISTRATION_WIDGET_H
#define RIO_REGISTRATION_WIDGET_H
// qt
#include <QWidget>
namespace Ui {
  class RIORegistrationWidget;
}
class RIORegistrationWidget: public QWidget
{
  Q_OBJECT;
public:
  explicit RIORegistrationWidget(QWidget *parent = nullptr);

private:
  Ui::RIORegistrationWidget *ui; 
  size_t currentIndex = 11;
  Q_DISABLE_COPY(RIORegistrationWidget);
  void showImage(size_t index);
private Q_SLOTS:
  void on_pushButtonRight_clicked(bool checked = false); 
  void on_pushButtonLeft_clicked(bool checked = false);
};

#endif //!RIO_REGISTRATION_WIDGET_H