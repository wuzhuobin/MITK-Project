#ifndef SCBDENTALCLOSINGDIALOG_H
#define SCBDENTALCLOSINGDIALOG_H

#include <QDialog>

#include "SCBWindowsUtil.h"
#include "ui_SCBDentalClosingDialog.h"

// #include "URDashBoard.h"

class AbstractRobot;
class SCBDentalClosingDialog : public QDialog, public Ui::SCBDentalClosingDialog
{
  Q_OBJECT

public:
  explicit SCBDentalClosingDialog(QWidget* parent = 0);
  ~SCBDentalClosingDialog();

private:
  AbstractRobot* robot;
  // std::vector<Joints_DataMap> m_ref_jnt;

public slots:
  void pushbtn_end_surgery_pressed();
  void pushbtn_continue_validation_pressed();
  void pushbtn_start_surgery_pressed();
  void pushbtn_cancel_pressed();
  void pushbtn_shutdown_pressed();
  void pushbtn_collapse_pressed();
  void pushbtn_cancel2_pressed();
};

#endif  // SCBDENTALCLOSINGWIDGET_H
