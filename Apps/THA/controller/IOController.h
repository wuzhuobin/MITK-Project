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
  static IOController * GetInstance();
  void LoadScene(QString fileName);
  void AddReamerTrajectory();
  void AddReamer();
  void AddReamerCuter();
  // void SetMultiWidget(THAStdMultiWidget *w) { this->multiWidget = w; }
  // THAStdMultiWidget * GetMultiWidget() const { return this->multiWidget; }
  // void SetImageViewer(QLabel *viewer) { this->imageViewer = viewer; }
  // QLabel *GetImageViewer() const { return this->imageViewer; }

Q_SIGNALS:
  void SceneLoaded();

protected:
  explicit IOController(QObject *parent = nullptr);
private:
  THAStdMultiWidget *multiWidget;
  QLabel *imageViewer;
};

#endif//!IO_CONTROLLER_H