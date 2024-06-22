#ifndef CHARTS_H
#define CHARTS_H

#include <QMainWindow>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include "include/data_parser.h"

namespace Ui {
class Charts;
}

class Charts : public QMainWindow
{
    Q_OBJECT

public:
    explicit Charts(QWidget *parent = nullptr);
    ~Charts();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_actionOpen_triggered();
    void on_actionOSWES_toggled(bool checked);
    void on_actionWES12_toggled(bool checked);
    void on_actionWES34_toggled(bool checked);
    void on_actionWES56_toggled(bool checked); // New slot for WES56
    void on_actionWES78_toggled(bool checked); // New slot for WES78
    void on_actionWES910_toggled(bool checked); // New slot for WES910
    void on_actionWES1112_toggled(bool checked); // New slot for WES1112

private:
    void plotData();
    void openFile(const QString &filePath);
    void readSettings();
    void writeSettings();

    Ui::Charts *ui;
    DataParser *m_dataParser;
    QString m_settingsFilePath;
};

#endif // CHARTS_H
