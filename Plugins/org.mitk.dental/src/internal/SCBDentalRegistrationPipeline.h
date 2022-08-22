#ifndef SCBDENTALREGISTRATIONPIPELINE_H
#define SCBDENTALREGISTRATIONPIPELINE_H

#include <QWidget>

#include "ui_SCBDentalRegistrationPipeline.h"

class SCBImage;
class SCBPolyData;
class vtkMatrix4x4;
class SCBMicronTrackerThread;
class SCBOrthogonalViewer;
class QProgressDialog;

namespace Ui
{
class SCBDentalRegistrationPipeline;
}

class SCBDentalRegistrationPipeline : public QWidget,
                                      public Ui::SCBDentalRegistrationPipeline
{
  Q_OBJECT

public:
  explicit SCBDentalRegistrationPipeline(QWidget* parent = 0);
  ~SCBDentalRegistrationPipeline();
  vtkMatrix4x4* m_trackerInImageMatrix;
  vtkMatrix4x4* m_trackerInRobotMatrix;
  SCBMicronTrackerThread* m_trackerThread;

signals:
  void trackerInImageRMSUpdated(double RMS);
  void trackerInRobotRMSUpdated(double RMS);
  void signalConfirmRegistration();

public slots:
  void UpdatePCA_ICP_Transform();
  void UpdateTrackerRobotRegistration();
  void slotUpdateUI(int);
  void slotCalibrateOrientation();
  void slotCalibratePosition();
  void slotConfirmCalibration();
  void slotReferenceChanged(const QString& text);

protected:
  SCBPolyData* m_registrationTool;
  QString m_registrationToolName;
  QString m_referenceName;
  QString m_toolName;
  static const QString DIR;
};

#endif  // SCBDENTALREGISTRATIONPIPELINE_H
