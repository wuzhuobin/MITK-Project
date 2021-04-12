#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PlanningSoftware.h"

class PlanningSoftware : public QMainWindow
{
    Q_OBJECT

public:
    PlanningSoftware(QWidget *parent = Q_NULLPTR);

private:
    Ui::PlanningSoftwareClass ui;
};
