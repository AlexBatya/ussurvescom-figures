#include "include/charts.h"
#include "qcustomplot.h"
#include "ui_charts.h"
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDebug>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QCloseEvent>
#include <QStandardPaths>

DataParserWorker::DataParserWorker(const QString &filePath, QObject *parent)
    : QObject(parent), m_filePath(filePath) {}

DataParserWorker::~DataParserWorker() {}

void DataParserWorker::process() {
    DataParser *dataParser = new DataParser(m_filePath.toStdString());
    emit dataParsed(dataParser);
}

Charts::Charts(int argc, char *argv[], QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Charts),
    m_dataParser(nullptr)
{
    this->setWindowTitle("Ussuvescom-figures");
    ui->setupUi(this);

    connect(ui->action, &QAction::triggered, this, &Charts::on_actionOpen_triggered);

    actionToDataName[ui->action_2] = "OSWES";
    actionToDataName[ui->action_1_2] = "WES12";
    actionToDataName[ui->action_3_4] = "WES34";
    actionToDataName[ui->action_5_7] = "WES56";
    actionToDataName[ui->action_7_9] = "WES78";
    actionToDataName[ui->action_9_11] = "WES910";
    actionToDataName[ui->action_11_13] = "WES1112";

    for (auto action : actionToDataName.keys()) {
        connect(action, &QAction::toggled, this, &Charts::on_plotToggled);
    }

    QIcon icon(":/icon.ico");
    setWindowIcon(icon);
    setAcceptDrops(true);

    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(appDataPath); // Ensure the directory exists
    m_settingsFilePath = QDir(appDataPath).filePath("ussurvescom_figures/settings.json");
    QDir().mkpath(QFileInfo(m_settingsFilePath).absolutePath()); // Ensure the settings directory exists
    qDebug() << "Settings file path:" << m_settingsFilePath;
    readSettings();

    if (argc > 1) {
        QString filePath = argv[1];
        if (QFileInfo::exists(filePath) && filePath.endsWith(".xml")) {
            openFile(filePath);
        }
    }
}

Charts::~Charts()
{
    delete ui;
    delete m_dataParser;
}

void Charts::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void Charts::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        if (!urlList.isEmpty()) {
            QString filePath = urlList.first().toLocalFile();
            qDebug() << "Dropped file path:" << filePath;
            if (filePath.endsWith(".xml")) {
                openFile(filePath);
            }
        }
    }
}

void Charts::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open XML File"), "", tr("XML Files (*.xml)"));
    if (!fileName.isEmpty()) {
        openFile(fileName);
    }
}

void Charts::openFile(const QString &filePath)
{
    setLoading(true);
    QThread *thread = new QThread;
    DataParserWorker *worker = new DataParserWorker(filePath);
    worker->moveToThread(thread);

    connect(thread, &QThread::started, worker, &DataParserWorker::process);
    connect(worker, &DataParserWorker::dataParsed, this, &Charts::handleDataParsed);
    connect(worker, &DataParserWorker::dataParsed, worker, &DataParserWorker::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    thread->start();
}

void Charts::handleDataParsed(DataParser *dataParser)
{
    delete m_dataParser;
    m_dataParser = dataParser;
    plotData();
    setLoading(false);
}

void Charts::on_plotToggled(bool checked)
{
    Q_UNUSED(checked);
    plotData();
}

void Charts::plotData()
{
    if (!m_dataParser) return;

    ui->customPlot->clearGraphs();

    QVector<double> xData = m_dataParser->getXData();
    QMap<QString, QVector<double>> dataMap = {
        {"OSWES", m_dataParser->getYDataOSWES()},
        {"WES12", m_dataParser->getYDataWES12()},
        {"WES34", m_dataParser->getYDataWES34()},
        {"WES56", m_dataParser->getYDataWES56()},
        {"WES78", m_dataParser->getYDataWES78()},
        {"WES910", m_dataParser->getYDataWES910()},
        {"WES1112", m_dataParser->getYDataWES1112()}
    };

    QMap<QString, QPen> penMap = {
        {"OSWES", QPen(Qt::red)},
        {"WES12", QPen(Qt::black)},
        {"WES34", QPen(Qt::green)},
        {"WES56", QPen(Qt::cyan)},
        {"WES78", QPen(Qt::magenta)},
        {"WES910", QPen(Qt::gray)},
        {"WES1112", QPen(Qt::blue)}
    };

    QMap<QString, QString> nameMap = {
        {"OSWES", "Вес"},
        {"WES12", "1-2"},
        {"WES34", "3-4"},
        {"WES56", "5-6"},
        {"WES78", "7-8"},
        {"WES910", "9-10"},
        {"WES1112", "11-12"}
    };

    int graphIndex = 0;
    for (auto action : actionToDataName.keys()) {
        if (action->isChecked()) {
            QString dataName = actionToDataName[action];
            ui->customPlot->addGraph();
            ui->customPlot->graph(graphIndex)->setData(xData, dataMap[dataName]);
            ui->customPlot->graph(graphIndex)->setName(nameMap[dataName]);
            ui->customPlot->graph(graphIndex)->setPen(penMap[dataName]);
            ++graphIndex;
        }
    }

    ui->customPlot->rescaleAxes();
    ui->customPlot->replot();
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables | QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectItems | QCP::iMultiSelect);

    ui->customPlot->legend->setVisible(graphIndex > 0);
    if (graphIndex > 0) {
        ui->customPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));

        QFont legendFont = font();
        legendFont.setPointSize(10);
        ui->customPlot->legend->setFont(legendFont);
        ui->customPlot->legend->setRowSpacing(-3);
    }
}

void Charts::readSettings()
{
    QFile settingsFile(m_settingsFilePath);
    if (!settingsFile.exists()) {
        qDebug() << "Settings file does not exist. Creating default settings.";
        QJsonObject defaultSettings;
        for (auto key : actionToDataName.values()) {
            defaultSettings[key] = false;
        }

        QJsonDocument doc(defaultSettings);
        if (settingsFile.open(QIODevice::WriteOnly)) {
            settingsFile.write(doc.toJson());
            settingsFile.close();
            qDebug() << "Default settings written to" << m_settingsFilePath;
        } else {
            qDebug() << "Failed to open settings file for writing:" << settingsFile.errorString();
        }

        for (auto action : actionToDataName.keys()) {
            action->setChecked(false);
        }
    } else {
        qDebug() << "Settings file exists. Reading settings from" << m_settingsFilePath;
        if (settingsFile.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(settingsFile.readAll());
            settingsFile.close();
            QJsonObject settings = doc.object();
            for (auto action : actionToDataName.keys()) {
                QString dataName = actionToDataName[action];
                if (settings.contains(dataName)) {
                    action->setChecked(settings[dataName].toBool());
                } else {
                    action->setChecked(false);
                }
            }
        } else {
            qDebug() << "Failed to open settings file for reading:" << settingsFile.errorString();
        }
    }
}

void Charts::writeSettings()
{
    qDebug() << "Writing settings to" << m_settingsFilePath;
    QFile settingsFile(m_settingsFilePath);
    QJsonObject settings;
    for (auto action : actionToDataName.keys()) {
        QString dataName = actionToDataName[action];
        settings[dataName] = action->isChecked();
    }

    QJsonDocument doc(settings);
    if (settingsFile.open(QIODevice::WriteOnly)) {
        settingsFile.write(doc.toJson());
        settingsFile.close();
        qDebug() << "Settings successfully written to" << m_settingsFilePath;
    } else {
        qDebug() << "Failed to open settings file for writing:" << settingsFile.errorString();
    }
}

void Charts::setLoading(bool loading)
{
    if (loading) {
        statusBar()->showMessage("Загрузка...");
    } else {
        statusBar()->clearMessage();
    }
}

void Charts::closeEvent(QCloseEvent *event)
{
    writeSettings();
    QMainWindow::closeEvent(event);
}
