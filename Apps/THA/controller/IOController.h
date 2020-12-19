#ifndef IO_CONTROLLER_H
#define IO_CONTROLLER_H
// qt
#include <QObject>
class IOController: public QObject
{
  Q_OBJECT;
public:
  static void LoadScene(QString fileName);
  explicit IOController(QObject *parent = nullptr);
};

#endif//!IO_CONTROLLER_H