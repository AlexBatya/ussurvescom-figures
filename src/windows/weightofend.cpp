#include "include/windows/weightofend.h"
#include "ui_weightofend.h"

WeightOfEnd::WeightOfEnd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WeightOfEnd)
{
    ui->setupUi(this);
}

WeightOfEnd::~WeightOfEnd()
{
    delete ui;
}
