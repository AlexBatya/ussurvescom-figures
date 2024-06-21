#ifndef CHARTS_H
#define CHARTS_H

#include <QMainWindow>
#include <QFileDialog>
#include "include/data_parser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Charts; }
QT_END_NAMESPACE

class Charts : public QMainWindow
{
    Q_OBJECT

public:
    explicit Charts(QWidget *parent = nullptr);
    ~Charts();

private slots:
    void on_actionOpen_triggered();
    void plotData();

private:
    Ui::Charts *ui;
    DataParser *m_dataParser;

    void setupInteractions();
};

#endif // CHARTS_H
