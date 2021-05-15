#ifndef IO_CONTROLLER_H
#define IO_CONTROLLER_H
// qt
#include <QObject>

class THAStdMultiWidget;
class QLabel;
class IOController: public QObject
{
  Q_OBJECT;
public:
  static IOController * getInstance();
  void loadScene(QString fileName);
  void saveScene(QString fileName);
  Q_SIGNAL void sceneLoaded();

protected:
  // explicit IOController(QObject *parent = nullptr);
;
};

#endif//!IO_CONTROLLER_H