// #include <AcetabularPrepPanel.h>
#include "AcetabularPrepWidget.h"
#include "AcetabularPrepView.h"
#include "AcetabularPrepNextBackWidget.h"

// qt
#include <QMainWindow>
#include <QApplication>
#include <QHBoxLayout>

// mitk
#include <QmitkRegisterClasses.h>

int main(int argc, char *argv[])
{
  QmitkRegisterClasses();
  QApplication app(argc, argv);

  AcetabularPrepNextBackWidget w;
  w.show();

  AcetabularPrepView v;
  v.show();

  // AcetabularPrepPanel p(&v);
  // p.show();

  // auto& widget = p.widget();
  AcetabularPrepWidget widget(&v);
  widget.show();

  QObject::connect(&w, &AcetabularPrepNextBackWidget::backClicked, [&widget]() {
    widget.setCurrentIndex(widget.currentIndex() - 1);
  });
  QObject::connect(&w, &AcetabularPrepNextBackWidget::nextClicked, [&widget]() {
    widget.setCurrentIndex(widget.currentIndex() + 1);
  });

  return app.exec();
}