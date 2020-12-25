#ifndef CUP_PARAMETER_GADGET_H
#define CUP_PARAMETER_GADGET_H

#include <QWidget>

namespace Ui
{
  class CupParameterGadget;
}
class CupParameterGadgetOrientationCommand;
class CupParameterGadgetPositionCommand;

class CupParameterGadget : public QWidget
{
  Q_OBJECT

public:
  explicit CupParameterGadget(
    const QString &acetabularShell = QStringLiteral("acetabular_shell"),
    const QString &acetabularLiner = QStringLiteral("acetabular_liner"),
    const QString &cor = QStringLiteral("COR"),
    QWidget *parent = 0);
  explicit CupParameterGadget(QWidget *parent):
    CupParameterGadget(
      QStringLiteral("acetabular_shell"),
      QStringLiteral("acetabular_liner"),
      QStringLiteral("COR"),
      parent
    ) {}
  virtual ~CupParameterGadget() override;
  void observerCup() const;


private:
  QString acetabularShell;
  QString acetabularLiner;
  QString cor;

  friend class CupParameterGadgetOrientationCommand;
  friend class CupParameterGadgetPositionCommand;
  //   void SetCupInclination(int value);
  //   void SetCupVersion(int value);
  //   void SetCupSuperior(int value);
  //   void SetCupMedial(int value);
  //   void SetCupPosterior(int value);

  //   int GetCupInclination();
  //   int GetCupVersion();
  //   int GetCupSuperior();
  //   int GetCupMedial();
  //   int GetCupPosterior();

  // private slots:
  //   void on_spinBoxCupInclination_editingFinished();
  //   void on_spinBoxCupVersion_editingFinished();
  //   void on_spinBoxSuperior_editingFinished();
  //   void on_spinBoxMedial_editingFinished();
  //   void on_spinBoxPosterior_editingFinished();

private:
  Ui::CupParameterGadget *ui;
};

#endif //!CUP_PARAMETER_GADGET_H
