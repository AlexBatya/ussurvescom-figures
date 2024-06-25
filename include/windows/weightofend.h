#ifndef DIAGNOSTIC_WINDOW_H
#define DIAGNOSTIC_WINDOW_H

#include <QMainWindow>

namespace Ui {
class WeightOfEnd;
}

class WeightOfEnd : public QMainWindow
{
    Q_OBJECT

public:
    explicit WeightOfEnd(QWidget *parent = nullptr);
    ~WeightOfEnd();

private:
    Ui::WeightOfEnd *ui;
};

#endif // DIAGNOSTIC_WINDOW_H
