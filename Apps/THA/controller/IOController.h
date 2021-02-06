#ifndef IO_CONTROLLER_H
#define IO_CONTROLLER_H
// qt
#include <QObject>
class QLabel;
class IOController: public QObject
{
  Q_OBJECT;
public:
  static IOController * GetInstance();
  void LoadScene(QString fileName);
  void AddReamerTrajectory();
  void AddReamerCuter();
  void SetImageViewer(QLabel *viewer) { this->imageViewer = viewer; }
  QLabel *GetImageViewer() const { return this->imageViewer; }

protected:
  explicit IOController(QObject *parent = nullptr);
private:
  QLabel *imageViewer;
};

#endif//!IO_CONTROLLER_H