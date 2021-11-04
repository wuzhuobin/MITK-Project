#ifndef CUP_PARAMETER_GADGET_H
#define CUP_PARAMETER_GADGET_H

#include <QWidget>

namespace Ui
{
  class CupParameterGadget;
}
class CupParameterGadgetCommand;
class CupParameterGadgetPositionCommand;

class CupParameterGadget : public QWidget
{
  Q_OBJECT

public:
  explicit CupParameterGadget(
    const QString &acetabularShell = QStringLiteral("acetabular_shell"),
    const QString &acetabularLiner = QStringLiteral("acetabular_liner"),
    const QString &cupCor = QStringLiteral("cup_cor"),
    const QString &nativeCor = QStringLiteral("native_cor"),
    QWidget *parent = 0);
  explicit CupParameterGadget(QWidget *parent):
    CupParameterGadget(
      QStringLiteral("acetabular_shell"),
      QStringLiteral("acetabular_liner"),
      QStringLiteral("cup_cor"),
      QStringLiteral("native_cor"),
      parent
    ) {}
  virtual ~CupParameterGadget() override;
  void ObserverCup() const;


private:
  QString acetabularShell;
  QString acetabularLiner;
  QString cupCor;
  QString nativeCor;

  friend class CupParameterGadgetCommand;

private:
  Ui::CupParameterGadget *ui;
};

#endif //!CUP_PARAMETER_GADGET_H
