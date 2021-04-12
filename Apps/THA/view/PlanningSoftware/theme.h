#ifndef apps_svpk_lib_view_theme_h_
#define apps_svpk_lib_view_theme_h_
#include <QtWidgets/QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include<QToolButton>
#include <QMessagebox>
#include <QObject>
#include <QDialog>
#include <QPushButton>
#include <QToolButton>
#include <QToolBar>
#include <QTableView>
#include <QHeaderView>
#include <QMenu>
#include <QComboBox>
#include <QTableWidget>
#include <QTabWidget>
#include <QStackedWidget>
#include <QApplication>
#include <QListWidget>
#include <QTreeWidget>

class Theme
{
public:
    //QWidget style
    void setFlowViewStyle(QWidget* widget);
    void setWidgetPanelStyle(QWidget* widget);
    void setTitleWidgetStyle(QWidget *widget);
    void setleftWidgetStyle(QWidget *widget);
    //QlineEdit style
    void setLineEditStyle(QLineEdit *lineedit);
    //QMenu style	
    void setMenuStyle(QMenu *menu, int pointSize);
    void setUserNameLabelStyle(QLabel *label);
    void setFlowPanelTitleQss(QLabel * qLabel);

    // Get Icons(none ,doing&done) of each group
    void setQMessageBoxStyle(QMessageBox* messageBox);

    static Theme* instance();

    void setDialogBackground(QDialog *dialog);
    void setDialogInfoHintBackground(QDialog *dialog);
    void setBackgroundstyle(QWidget *widgetLeft, QWidget *widgetRight);

    //Button style
    void setButtonStyle(QPushButton *btn);

    //QWidget style
    void setleftWidget(QWidget *widget);

    //QlineEdit style
    void setLineEditSearchInputStyle(QLineEdit *lineEdit);

    //QTableView style
    void setTableViewStyle(QTableView *tableView);

    //QTableWidget style
    void setTableWidgetStyle(QTableWidget *tableWidget);

    // QTabWidget
    void setTabWidgetStyle(QTabWidget *tabWidget);

    //Label style
    void setLabelStyle_18pxWhite(QLabel *label);
    void setLabelStyle_18pxGray(QLabel *label);
    void setLabelStyle_14pxWhite(QLabel *label);
    void setLabelStyle_14pxGray(QLabel *label);
    void setLabelStyle_12pxWhite(QLabel *label);
    void setLabelBonePicStyle(QLabel *label);
    void setLabelScreenshotPicStyle(QLabel *label);
    void setLabelEditUserName(QLabel *label);
    void setLabelPicInfoStyle(QLabel *pictureLabel, QLabel *backgroundLabel, QLabel *categoryLabel, QLabel *slicesNumLabel);

    //ComboBox style
    void setComboBoxUserManageStyle(QComboBox *comboBox);

    //stackedWidget
    void setStackedWidgetStyle(QStackedWidget *stackedWidget);

    //treeWidget style (FileInputPage)
    void setTreeWidgetStyle(QTreeWidget *treeWidget);




protected:
    Theme() = default;
};

#endif // apps_svpk_lib_view_theme_h_