#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SkyDefendingProjectcpp.h"

class SkyDefendingProjectcpp : public QMainWindow
{
    Q_OBJECT

public:
    SkyDefendingProjectcpp(QWidget *parent = nullptr);
    ~SkyDefendingProjectcpp();

private:
    Ui::SkyDefendingProjectcppClass ui;
};

