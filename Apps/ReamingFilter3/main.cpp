// qt
#include <QApplication>

// mitk
#include <QmitkRegisterClasses.h>
#include <QmitkRenderWindow.h>

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  QmitkRegisterClasses();

  QmitkRenderWindow renderWindow;
  renderWindow.show();

  return app.exec();
}