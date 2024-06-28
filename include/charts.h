#ifndef CHARTS_H
#define CHARTS_H

#include <QMainWindow>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QThread>
#include <QStandardPaths>
#include <QDir>
#include "include/data_parser.h"

namespace Ui {
class Charts;
}

class DataParserWorker : public QObject
{
    Q_OBJECT
public:
    explicit DataParserWorker(const QString &filePath, QObject *parent = nullptr);
    ~DataParserWorker();

signals:
    void dataParsed(DataParser *dataParser);

public slots:
    void process();

private:
    QString m_filePath;
};

class Charts : public QMainWindow
{
    Q_OBJECT

public:
    explicit Charts(int argc, char *argv[], QWidget *parent = nullptr);
    ~Charts();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_actionOpen_triggered();
    void on_plotToggled(bool checked);
    void handleDataParsed(DataParser *dataParser);

private:
    void plotData();
    void openFile(const QString &filePath);
    void readSettings();
    void writeSettings();
    void setLoading(bool loading);

    Ui::Charts *ui;
    DataParser *m_dataParser;
    QString m_settingsFilePath;
    QMap<QAction*, QString> actionToDataName;
};

#endif // CHARTS_H
