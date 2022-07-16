#ifndef __SCB_Dental_MAIN_WINDOW_H__
#define __SCB_Dental_MAIN_WINDOW_H__

// std
#include <memory>

// qt
#include <QMainWindow>
class QGraphicsView;
class QActionGroup;
class QProgressDialog;
class SCBPlanarViewer;
class SCBNonPlanarViewer;
class SCBOrthogonalViewer;
class SCBViewer;
class PoseIndicatorViewer;
class SCBDentalPlanningPipeline;
class SCBDentalRegistrationPipeline;
class SCBDentalNavigationPipeline;
class SCBDentalValidationPipeline;
class SCBDentalClosingDialog;
class AbstractRobot;
class SCBWelcomeWizard;
class MACKeyConfirm;

// #include "AbstractRobotCommon.h"
namespace Ui
{
class SCBDentalMainWindow;
}
class SCBDentalMainWindow final : public QMainWindow
{
  Q_OBJECT;

public:
  const static int MAX_RECENT_IMAGE = 10;

  explicit SCBDentalMainWindow(QWidget* parent = nullptr);
  virtual ~SCBDentalMainWindow() override;

  SCBDentalPlanningPipeline* getDentalPlanningPipeline()
  {
    return this->DentalPlanningPipeline;
  }
  SCBDentalRegistrationPipeline* getDentalRegistrationPipeline()
  {
    return this->DentalRegistrationPipeline;
  }
  SCBDentalNavigationPipeline* getDentalNavigationPipeline()
  {
    return this->DentalNavigatePipeline;
  }
  SCBDentalValidationPipeline* getDentalValidationPipeline()
  {
    return this->DentalValidatePipeline;
  }

  PoseIndicatorViewer* getPoseIndicatorViewer()
  {
    return this->poseIndicatorViewer;
  }
  SCBOrthogonalViewer* getViewer(int i);

  SCBDentalPlanningPipeline* DentalPlanningPipeline;
  SCBDentalRegistrationPipeline* DentalRegistrationPipeline;
  SCBDentalNavigationPipeline* DentalNavigatePipeline;
  SCBDentalValidationPipeline* DentalValidatePipeline;

  QActionGroup* stylesGroup;
  AbstractRobot* robot;

  SCBDentalClosingDialog* closingdialog;
  SCBWelcomeWizard* welcomeWizard;
  MACKeyConfirm* mac;
  QList<SCBPlanarViewer*> planarViewers;
  SCBNonPlanarViewer* nonPlanarViewer;

  SCBOrthogonalViewer* panoramicViewer;
  QList<SCBPlanarViewer*> panoramicSagitalViewers;
  QGraphicsView* trackerViewer;

  PoseIndicatorViewer* poseIndicatorViewer;
  void setPatientNameIDLabel(QString name, int id, int age);

public slots:
  // menu file
  void slotWelcomeWizard();
  bool slotMacChecking();
  void slotImportProject(QString path = QString(), bool clean = true);
  void slotImportProjectString(QString xml = QString());
  void slotCommitProject(QString path = QString());

  // void showFullScreen();

  void on_toolButtonPlanning_toggled(bool checked);
  void on_toolButtonRegistration_toggled(bool checked);
  void on_toolButtonNavigation_toggled(bool checked);
  void on_toolButtonValidation_toggled(bool checked);

  void on_pushButtonHelp_clicked();
  void on_pushButtonAbout_clicked();
  void on_pushButtonPatientManagement_clicked();
  void on_tabWidgetCentral_currentChanged(int index);

  void on_pushButtonOperation_clicked();
  void on_pushButtonCollapse_clicked();
  // void on_pushButtonFreeDrive_clicked();

  void slotPlanningPipelineConfirm();
  void slotRegistrationPipelineConfirm();
  void slotNavigationPipelineConfirm();
  void slotValidationPipelineConfirm();
  void slotNavigationPipelineToValidation();

  void slotLoadingDialog(bool);
  void slotSetLoadingDialogValue(int);

signals:
  void signalImportImages(QStringList paths);
  void signalImportLabelImages(QStringList paths, QString label);
  void signalImportedProject(QString path, bool clean);
  void signalImportedProjectString(QString string);
  void signalCommitedProject(QString path);

protected:
  virtual void closeEvent(QCloseEvent* event) override;
  QString getCurrentProjectDirectory();

private:
  // Recent File
  std::unique_ptr<Ui::SCBDentalMainWindow> mUi;
  QProgressDialog* m_progressDialog;
  int patientID = 0;
  //   std::vector<Joints_DataMap> m_ref_jnt;
  QString PROJECT_DIR;
  QString currentProjectDirectory;
  QString currentProjectXml = "";
  bool m_firstLoad = true;
};

#endif  // !__SCB_Dental_MAIN_WINDOW_H__
