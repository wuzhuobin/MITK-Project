#ifndef SCBDentalNavigationPipeline_H
#define SCBDentalNavigationPipeline_H

#include <QTimer>
#include <QWidget>

#include "ui_SCBDentalNavigationPipeline.h"
// #include "SCBScene.h"
// #include "SCBDentalFixture.h"
// #include "SCBMicronTrackerThread.h"
// #include "qtimer.h"
// #include "SCBDentalClosingDialog.h"

class vtkMatrix4x4;
class AbstractRobot;
// Note: Ui->pushButtonEmergencyStop does not mean emergency stop

class SCBAbstractProp;
class SCBPolyData;

class SCBDentalHub;
class RoboticOrientationCalculator;
class SCBDentalNavigationPipeline : public QWidget,
                                    public Ui::SCBDentalNavigationPipeline
{
  Q_OBJECT

public:
  explicit SCBDentalNavigationPipeline(QWidget* parent = 0);
  ~SCBDentalNavigationPipeline();

  // SCBMicronTrackerThread* m_trackerThread;
  AbstractRobot* m_robot;
  QString m_currentImplant;
  bool m_quitDrill;
  QTimer checktimer;

  // SCBDentalHub* hub;
public slots:
  void addImplant(QString uniqueName);
  void removeImplant(QString unqiueName);
  int MoveToImplant();
  void CalibrateNewTooltip();
  void DrillMode();
  void FreedriveCheck();
  void setCurrentImplant(QString index);
  void Finished();
  int ResetRobot();
  int Stop();

  void tracking(vtkMatrix4x4* matrix);
  void startTracking();
  void stopTracking();

protected:
  SCBAbstractProp* m_toolProp;
  SCBPolyData* m_tool;
  double m_toolTipLength = 0;
};

class vtkTransform;
class vtkPlane;
class vtkMatrix3x3;
class vtkMatrix4x4;
class RoboticOrientationCalculator
{
public:
  RoboticOrientationCalculator();
  ~RoboticOrientationCalculator();
  void SetUpDirection(double, double, double);
  void SetUpDirection(double*);
  void SetDirectionVector(double, double, double);
  void SetDirectionVector(double*);
  void Update();
  void GetRotationVector(double*);
  double* GetRotationVector();
  vtkTransform* GetTransform();

private:
  bool checksame(double* a, double* b);
  int checkorientation(double* a, double* b);
  double* m_direction;
  double* m_up_direction;
  double* m_output;
  double* m_intermediateOutput;
  vtkTransform* m_transform;
  vtkTransform* m_allign;
  vtkPlane* m_orthogonalplane;
  vtkTransform* m_angular_correction;
  vtkMatrix3x3* m_rotation_matrix;
  vtkMatrix4x4* m_cachematrix;
  QTimer checktimer;
};

#endif  // SCBDentalNavigationPipeline_H
