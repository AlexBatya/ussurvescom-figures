#include "include/axeswindow.h"
#include "ui_axeswindow.h"

axeswindow::axeswindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::axeswindow)
{
    ui->setupUi(this);
}

axeswindow::~axeswindow()
{
    delete ui;
}
