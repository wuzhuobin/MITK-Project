#ifndef TOOLS_WIDGET
#define TOOLS_WIDGET

// qt
#include <QWidget>
namespace Ui {
class ToolsWidget;
}
namespace itk{
    class Object;
    class EventObject;
}
class ToolsWidget : public QWidget {
public:
  enum class Page { Dummy = 0, Line = 1, Angle = 2 };
  explicit ToolsWidget(QWidget *parent = nullptr);
  ~ToolsWidget() override;
  void setCurrentPage(Page page);

protected:
  void changeEvent(QEvent *event) override;

private:
  Ui::ToolsWidget *mUi;
  void updateDistance(itk::Object* object, const itk::EventObject& event);
  void updateAngle(itk::Object* object, const itk::EventObject& event);
  Q_SLOT void on_stackedWidget_currentChanged(int index);
};

#endif // !TOOLS_WIDGET