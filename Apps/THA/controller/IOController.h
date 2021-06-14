#ifndef IO_CONTROLLER_H
#define IO_CONTROLLER_H
// qt
#include <QObject>

class IOController : public QObject
{
    Q_OBJECT;

public:
    static IOController* getInstance();
    explicit IOController(QObject* parent = nullptr);
    void loadScene(const QString& fileName);
    void addReamerTrajectory();
    void addReamer();
    void addReamerCuter();
    // void SetMultiWidget(THAStdMultiWidget *w) { this->multiWidget = w; }
    // THAStdMultiWidget * GetMultiWidget() const { return this->multiWidget; }
    // void SetImageViewer(QLabel *viewer) { this->imageViewer = viewer; }
    // QLabel *GetImageViewer() const { return this->imageViewer; }

    Q_SIGNAL void sceneLoaded();

private:
    class THAStdMultiWidget* mMultiWidget;
    class QLabel* mImageViewer;
};

#endif  //! IO_CONTROLLER_H