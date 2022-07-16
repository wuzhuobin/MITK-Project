#ifndef __SCB_DENTAL_HUB_H__
#define __SCB_DENTAL_HUB_H__

// me
class SCBDentalMainWindow;
class SCBStylesSwitchOrthogonalViewer;
class SCBWidgetCollection;
class PolarisVicraConfiguration;
class LandmarkRegistrationFilter;
class SCBWelcomeWizard;
// qt
#include <QObject>
// #include "SCBPlanarViewer.h"
// #include "SCBViewer.h"
#include <vtkImageActor.h>
#include <vtkImageMapper.h>
#include <vtkImageResliceMapper.h>

#include "vtkImageData.h"

class SCBDentalHub final : public QObject
{
  Q_OBJECT;

public:
  static const int NUM_OF_ORTHOGONAL_VIEWER = 4;
  static const int NUM_OF_2D_VIEWERS = 3;

  static SCBDentalHub* self;
  explicit SCBDentalHub(QObject* parent = nullptr);
  virtual ~SCBDentalHub() override;

  SCBDentalMainWindow* mainWindow;
  SCBStylesSwitchOrthogonalViewer* styles[4];
  SCBWidgetCollection* widgets[4];

  // keyConfirm
public slots:
  // void slotWelcoming();
  void slotInitializationImages(QStringList imagePaths);
  void slotInitializationLabeledImages(QStringList imagePaths, QString label);
  void slotInitializationImport();

  void slotInitializationMainWindow();

  void slotInitializationParanomicView();

  void slotCommit(QString path);
  void slotImport(QString path, bool clean);
  void slotImportString(QString string);
  void slotClean();
  void slotExit();

  // interactorstyle
  void slotInteractorStyleWindowLevel(bool flag);
  void slotInteractorStyleNavigation(bool flag);

signals:
  void signalLoading(bool);
  void signalSetLoadingValue(int);
  void signalInitializationParanomicView();
};

#endif  // !__SCB_DENTAL_HUB_H__
