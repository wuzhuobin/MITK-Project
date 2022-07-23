#ifndef __SCB_Dental_MAIN_WINDOW_H__
#define __SCB_Dental_MAIN_WINDOW_H__

// std
#include <memory>

// qt
#include <QMainWindow>
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
  ~SCBDentalMainWindow() override;
  void test();

  Q_SLOT void onSceneLoaded();

private:
  SCBDentalPlanningPipeline* DentalPlanningPipeline;
  SCBDentalRegistrationPipeline* DentalRegistrationPipeline;
  SCBDentalNavigationPipeline* DentalNavigatePipeline;
  SCBDentalValidationPipeline* DentalValidatePipeline;

  QActionGroup* stylesGroup;
  AbstractRobot* robot;

  SCBDentalClosingDialog* closingdialog;
  QList<SCBPlanarViewer*> planarViewers;
  SCBNonPlanarViewer* nonPlanarViewer;

  SCBOrthogonalViewer* panoramicViewer;
  QList<SCBPlanarViewer*> panoramicSagitalViewers;

  PoseIndicatorViewer* poseIndicatorViewer;
  void setPatientNameIDLabel(QString name, int id, int age);

public slots:

  void on_toolButtonPlanning_toggled(bool checked);
  void on_toolButtonRegistration_toggled(bool checked);
  void on_toolButtonNavigation_toggled(bool checked);
  void on_toolButtonValidation_toggled(bool checked);

  void on_pushButtonHelp_clicked();
  void on_pushButtonAbout_clicked();
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