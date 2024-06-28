#ifndef AXESWINDOW_H
#define AXESWINDOW_H

#include <QWidget>

namespace Ui {
class axeswindow;
}

class axeswindow : public QWidget
{
    Q_OBJECT

public:
    explicit axeswindow(QWidget *parent = nullptr);
    ~axeswindow();

private:
    Ui::axeswindow *ui;
};

#endif // AXESWINDOW_H
