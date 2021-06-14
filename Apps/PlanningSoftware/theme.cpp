#include "theme.h"

namespace
{
    class ThemeImpl final : public Theme
    {
    public:
        // Make constructor public
        ThemeImpl() = default;
    };
    Q_GLOBAL_STATIC(ThemeImpl, g_instance);
};  // namespace


void Theme::setFlowViewStyle(QWidget* widget)
{
    widget->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
    widget->setWindowState(Qt::WindowFullScreen);
    QString qss_str =
        "QWidget{"
        "background-color:\"#1A1A1A\";"  //background is black
        "}"

        "QDialog{"
        "background-color:\"#1A1A1A\";"  // QDialog background is black
        "}"

        "QPushButton{"                   // PushButton setting 
        "border-style:hidden;"
        "background-color:\"#505761\";"
        "color:white;"
        "font-size:16px;"
        "font-family:Microsoft YaHei;"
        "border-radius:5px;"
        "}"

        "QToolButton{"// ToolButton setting 
        "border-style:hidden;"
        "}"
        ;
    widget->setStyleSheet(qss_str);   
}

void Theme::setWidgetPanelStyle(QWidget* widget)
{
    QString strWidgetQss = "background-color:\"#3D444F\"";
    widget->setStyleSheet(strWidgetQss);
}

void Theme::setTitleWidgetStyle(QWidget *widget)
{
    QString qss_str = "QWidget{"
        "color:white;"
        "font-size:18px;"
        "font-family:Microsoft YaHei;"
        "background-color:\"#323841\";"
        "}";
    widget->setStyleSheet(qss_str);
}

void Theme::setLineEditStyle(QLineEdit *lineedit)
{
    QFont font("Microsoft YaHei", 14, 50);  //first: format, second: font size, third: bond or not 50(normal)
    lineedit->setFont(font);
    lineedit->setAlignment(Qt::AlignHCenter);  //set horizental center
    QString qss_str = "QLineEdit{"
        "color:white;"
        "background-color:\"#3D444F\";"
        "border-width:0;"
        "border-radius:5px;"
        "}";
    lineedit->setStyleSheet(qss_str);
}

void Theme::setMenuStyle(QMenu *menu, int pointSize)
{
    QString qss_str = "QMenu{"
        "background-color:\"#323841\";"
        "color:white;border:0px;"
        "border-radius:5px;"
        "}"
        "QMenu::item{"
        "color:white;"
        "padding:5px 30px;"
        "margin:5px 8px;"
        "}"
        "QMenu::item:selected{"
        "background-color:\"#3B596D\";"
        "border-radius:5px;"
        "}";
    menu->setStyleSheet(qss_str);

    QFont labelfont("Microsoft YaHei", pointSize, 50);
    menu->setFont(labelfont);
}

void Theme::setUserNameLabelStyle(QLabel *label)
{
    QFont fontLabel("Microsoft YaHei", 14, 50);
    label->setFont(fontLabel);
    label->setStyleSheet("color:white");
}


void Theme::setleftWidgetStyle(QWidget *widget)
{
    QString widget_str = "background-color:\"#20242A\"";
    widget->setStyleSheet(widget_str);
}

void Theme::setFlowPanelTitleQss(QLabel * qLabel)
{
    qLabel->setStyleSheet("color:white;background-color:\"#3D444F\";");
}

void Theme::setQMessageBoxStyle(QMessageBox* messageBox)
{
    messageBox->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Dialog);
    messageBox->setStyleSheet("QLabel{""font-family:Microsoft YaHei;font-size:20px;color:white;""}");
}

Theme* Theme::instance()
{
    return g_instance;
}

void Theme::setDialogBackground(QDialog *dialog) {
    QString str = "QDialog{"
        "background-color:\"#1A1A1A\";"  // QDialog background is black
        "}";
    dialog->setStyleSheet(str);
}

void Theme::setDialogInfoHintBackground(QDialog *dialog) {
    QString str = "QDialog{"
        "background-color:\"#00008B\";"  // QDialog background is DarkBlue
        "}";
    dialog->setStyleSheet(str);
}

void Theme::setButtonStyle(QPushButton *btn) {
    QFont fontBtn("Microsoft YaHei", 16, 50);  //first is font format，second is font size，third is bond or not（50 means normal）
    btn->setStyleSheet("color:white;background-color:\"#505761\";border-radius:5px;border-style:hidden;");
    btn->setFont(fontBtn);
}

void Theme::setBackgroundstyle(QWidget *widgetLeft, QWidget *widgetRight) {
    widgetLeft->setStyleSheet("color:white;background-color:\"#1A1A1A\";");
    widgetRight->setStyleSheet("color:white;background-color:\"#3D444F\";");
}

void Theme::setLineEditSearchInputStyle(QLineEdit *lineEdit) {
    QFont font("Microsoft YaHei", 14, 50);
    QString lineEditStr = "QLineEdit{"
        "color:white;"
        "background-color:\"#20242A\";"
        "border-width:0;"
        "border-style:outset;"
        "border-radius:20px;"
        "}";
    lineEdit->setStyleSheet(lineEditStr);
    lineEdit->setFont(font);
    lineEdit->setAlignment(Qt::AlignHCenter);
    lineEdit->setPlaceholderText("Support Fuzzy Search");
}

void Theme::setTableViewStyle(QTableView *tableView) {
    //tableView style init
    QString strTableViewStyle = "QTableView {"
        "color: white;"
        "gridline-color: \"#535353\";"
        "background-color: \"#323841\";"
        "alternate-background-color: rgb(64, 64, 64);"
        "selection-color: white;"
        "selection-background-color:\"#3B596D\";"
        "border: none;"
        "border-radius: 5px;"
        "padding: 2px 4px;"
        "}"
        "QTableView::item{"
        "border:none;"
        "background-color:\"#3D444F\";"
        "}"
        "QTableView::item:selected{"
        "background-color:\"#3B596D\";"
        "border-top:1px solid \"#1E99D6\";"
        "border-bottom:1px solid \"#1E99D6\";"
        "}"
        //--QScrollBar-----
        "QScrollBar{"
        "background:gray;"
        "}"
        "QScrollBar:vertical{"
        "background-color:gray;"
        "width:20px;"
        "}"
        //-----Header View:: style
        "QHeaderView::section {"
        "border:none;"
        "font-size:18px;"
        "color: white;"
        "background-color:\"#323841\";"
        "}"

        "QScrollBar::handle:vertical{"
        "width:20px;"
        "background:\"#1E99D6\";"
        "border:0px;"
        "border-radius:20px;"
        "min-height:50px;}";

    tableView->setFocusPolicy(Qt::NoFocus); // delete dash border.
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows); //when clicking, show the whole row
    tableView->setStyleSheet(strTableViewStyle);
    tableView->setShowGrid(false);  // close the grid lines
    tableView->verticalHeader()->hide(); // hide row numbers
    tableView->verticalHeader()->setDefaultSectionSize(40);
    tableView->verticalHeader()->setMinimumSectionSize(40);
    tableView->horizontalHeader()->setDefaultSectionSize(133);
    tableView->horizontalHeader()->setMinimumSectionSize(133);
    tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
}

void Theme::setComboBoxUserManageStyle(QComboBox *comboBox) {
    QString str1 = "QComboBox{"
        "font-size:16px; color:white; background-color:\"#3D444F\";"
        "border-width:0;"
        "border-style:outset;"
        "border-radius:5px;"
        "}"

        "QComboBox::down-arrow{ "
        "image:url(:/svpk_lib/images/case_management/down__blue_arrow.png);"
        "background-color:transparent;"
        "width:40px;"
        "height:40px"
        "}"
        "QComboBox::down-arrow:on{ "
        "image:url(:/svpk_lib/images/case_management/up_blue_arrow.png);"
        "background-color:transparent;"
        "width:40px;"
        "height:40px"
        "}"

        "QComboBox QAbstractItemView {"//Box items 
        "outline:0px;" //no line of  dashes
        "font-size:16px; color:white; background-color:\"#3D444F\";"
        "}"

        "QComboBox::drop-down {"
        "subcontrol-origin: padding;"
        "subcontrol-position: top right;"
        "width: 40px;"
        "height:40px;"
        "border-width:0;"
        "border-radius:5px;"
        "background-color:transparent;"
        "}";
    QFont fontLabel("Microsoft YaHei", 18, 50);
    comboBox->setStyleSheet(str1);
    comboBox->setFont(fontLabel);
}

void Theme::setLabelStyle_18pxWhite(QLabel *label) {
    QFont fontLabel("Microsoft YaHei", 18, 50);
    label->setStyleSheet("color:white");
    label->setFont(fontLabel);
}
void Theme::setLabelStyle_18pxGray(QLabel *label) {
    QFont fontLabel("Microsoft YaHei", 18, 50);
    label->setFont(fontLabel);
    label->setStyleSheet("color:\"#808080\"");
}
void Theme::setLabelStyle_14pxWhite(QLabel *label) {
    QFont fontLabel("Microsoft YaHei", 14, 50);
    label->setFont(fontLabel);
    label->setStyleSheet("color:white");
}
void Theme::setLabelStyle_14pxGray(QLabel *label) {
    QFont fontLabel("Microsoft YaHei", 14, 50);
    label->setFont(fontLabel);
    label->setStyleSheet("color:\"#C2C2C2\"");
}
void Theme::setLabelStyle_12pxWhite(QLabel *label) {
    QFont fontLabel("Microsoft YaHei", 12, 50);
    label->setFont(fontLabel);
    label->setStyleSheet("color:white");
}
void Theme::setLabelEditUserName(QLabel *label) {
    QFont font("Microsoft YaHei", 14, 50);
    QString labelStyle = "color:\"#868686\";"
        "background-color:\"#3D444F\";"
        "border-width:0;"
        "border-radius:5px";
    label->setFont(font);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(labelStyle);
}
void Theme::setLabelPicInfoStyle(QLabel *pictureLabel, QLabel *backgroundLabel, QLabel *categoryLabel, QLabel *slicesNumLabel) {
    pictureLabel->setStyleSheet("background-color:black");
    backgroundLabel->setStyleSheet("background-color:\"#20242A\";border-width:0");
    QFont font1("Microsoft YaHei", 16, 50);
    QFont font2("Microsoft YaHei", 14, 50);
    categoryLabel->setFont(font1);
    categoryLabel->setStyleSheet("color:white;background:transparent");
    slicesNumLabel->setFont(font2);
    slicesNumLabel->setStyleSheet("color:white;background:transparent");
}

void Theme::setleftWidget(QWidget *widget) {
    widget->setStyleSheet("background-color:\"#20242A\"");
}

void Theme::setLabelBonePicStyle(QLabel *label) {
    label->setStyleSheet("background-color:\"#20242A\";border-width:1");
}
void Theme::setLabelScreenshotPicStyle(QLabel *label) {
    label->setStyleSheet("background-color:\"#505761\";border-width:1");
}
void Theme::setTableWidgetStyle(QTableWidget *tableWidget) {
    QString tableWdigetStr = "QTableWidget{"
        "color:white;"
        "font-size:16px;"
        "background-color:\"#3D444F\";"
        "border:none;"
        "}"
        "QTableWidget::indicator{"
        "width: 30px; height: 30px;"
        "border-radius:5px;"
        "}"
        "QTableWidget::indicator:checked {"
        "image: url(:/others/icon_images/others/onchecked30x30.png);"
        "border-radius:5px;"
        "}"
        "QTableWidget::indicator:unchecked {"
        "image: url(:/others/icon_images/others/unchecked30x30.png);"
        "border-radius:5px;"
        "}"
        "QHeaderView::section{"
        "color: \"#C2C2C2\";"
        "font-size:16px;"
        "background:transparent;"
        "}"
        "QTableWidget::item{"
        "border:none;"
        "background-color:\"#3D444F\";"
        "}"
        "QTableWidget::item:selected{"
        "color:white;"
        "background-color:\"#3B596D\";"
        "border-top:1px solid \"#1E99D6\";"
        "border-bottom:1px solid \"#1E99D6\";"
        "}";
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //when clicking, show the whole row
    tableWidget->setFocusPolicy(Qt::NoFocus);
    tableWidget->setFrameShape(QFrame::NoFrame);
    tableWidget->verticalHeader()->setHidden(true);
    tableWidget->setShowGrid(false);  //no grid lane
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  //set no editable
    tableWidget->horizontalHeader()->setFixedHeight(50); // header height is fixed
    tableWidget->setSortingEnabled(true); //sorting is ok
    tableWidget->setStyleSheet(tableWdigetStr);
}

void Theme::setTabWidgetStyle(QTabWidget *tabWidget) {
    QString tabWidgetStr = "QTabWidget:pane {"
        "border:none;"
        "border-top:0px solid #e8f3f9;"
        "background:transparent;"
        "}"
        "QTabWidget::tab-bar{"
        "border-width:0px;"
        "}"
        "QTabBar::tab{"
        "color:white;"
        "background:transparent;"
        "border-top-left-radius: 0px;"
        "border-top-right-radius: 0px;"
        "min-width:290px;"
        "min-height:70px;"
        "padding:0px;"
        "}"
        "QTabBar::tab:!selected{"
        "color:gray;"
        "background-color:transparent;"
        "}"
        "QTabBar::tab:selected{"
        "color: white;"
        "background-color:\"#3D444F\";"
        "}";
    QFont font("Microsoft YaHei", 18, 50);
    tabWidget->setFont(font);
    tabWidget->setStyleSheet(tabWidgetStr);
}

void Theme::setStackedWidgetStyle(QStackedWidget *stackedWidget) {
    stackedWidget->setStyleSheet("background-color:\"#1A1A1A\";");
}

void Theme::setTreeWidgetStyle(QTreeWidget *treeWidget) {
    QString treeWidgetStr = "QTreeView{"
        "border:none;"
        "background-color:\"#323841\";"
        "}"
        "QTreeView::item:hover{"
        "background-color:gray;"
        "}"
        "QTreeView::item:selected{"
        "background-color:\"#1E99D6\";"
        "}";
    treeWidget->setStyleSheet(treeWidgetStr);
    treeWidget->header()->setVisible(false);  // do not show header.
}