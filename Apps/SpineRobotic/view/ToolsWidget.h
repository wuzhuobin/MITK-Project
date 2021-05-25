#ifndef TOOLS_WIDGET
#define TOOLS_WIDGET

// qt 
#include <QWidget>
namespace Ui {
class ToolsWidget;
}
class ToolsWidget: public QWidget
{
public:
    explicit ToolsWidget(QWidget *parent = nullptr);
    ~ToolsWidget() override;
private:
    Ui::ToolsWidget *ui;
};

#endif // !TOOLS_WIDGET