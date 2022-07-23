#ifndef IO_CONTROLLER_H
#define IO_CONTROLLER_H
// qt
#include <QObject>

class IOController : public QObject
{
  Q_OBJECT;

public:
  static QString getBaseProject();
  static IOController* getInstance();
  explicit IOController(QObject* parent = nullptr);

  void addReamerTrajectory() const;
  void addReamer() const;
  void addReamingPelvis() const;
  void addImpactingAcetabularShell() const;
  void addImpactingAcetabularShellCor() const;

  Q_SLOT void loadScene(const QString& fileName) const;
  Q_SLOT bool createScene(const QString& filename,
                          const QStringList& dicoms) const;
  Q_SIGNAL void sceneLoaded() const;

private:
  void initCaseDataBase() const;
};

#endif  //! IO_CONTROLLER_H