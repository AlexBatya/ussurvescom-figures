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
    void on_plotToggled(bool checked);

private:
    void plotData();
    void openFile(const QString &filePath);
    void readSettings();
    void writeSettings();

    Ui::Charts *ui;
    DataParser *m_dataParser;
    QString m_settingsFilePath;
    QMap<QAction*, QString> actionToDataName;  // Маппинг действий на имена данных
};

#endif // CHARTS_H
