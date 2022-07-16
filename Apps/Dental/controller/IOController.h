/**
 * @file IOController.h
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-06-12
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef IO_CONTROLLER_H
#define IO_CONTROLLER_H
// qt
#include <QObject>

class IOController : public QObject
{
  Q_OBJECT;

public:
  static IOController* getInstance();
  void loadScene(QString fileName);
  void saveScene(QString fileName);
  Q_SIGNAL void sceneLoaded();
};

#endif  //! IO_CONTROLLER_H