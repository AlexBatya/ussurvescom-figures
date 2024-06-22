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

Charts::Charts(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Charts),
    m_dataParser(nullptr)
{
    this->setWindowTitle("Ussuvescom-figures");
    ui->setupUi(this);
    connect(ui->action, &QAction::triggered, this, &Charts::on_actionOpen_triggered);
    connect(ui->action_2, &QAction::toggled, this, &Charts::on_actionOSWES_toggled);
    connect(ui->action_1_2, &QAction::toggled, this, &Charts::on_actionWES12_toggled);
    connect(ui->action_3_4, &QAction::toggled, this, &Charts::on_actionWES34_toggled);
    connect(ui->action_5_7, &QAction::toggled, this, &Charts::on_actionWES56_toggled); // Connect WES56 action
    connect(ui->action_7_9, &QAction::toggled, this, &Charts::on_actionWES78_toggled); // Connect WES78 action
    connect(ui->action_9_11, &QAction::toggled, this, &Charts::on_actionWES910_toggled); // Connect WES910 action
    connect(ui->action_11_13, &QAction::toggled, this, &Charts::on_actionWES1112_toggled); // Connect WES1112 action

    QIcon icon(":/icon.ico");
    setWindowIcon(icon);

    setAcceptDrops(true);

    // Устанавливаем путь к файлу настроек
    m_settingsFilePath = QDir(QCoreApplication::applicationDirPath()).filePath("settings.json");

    // Читаем настройки при запуске программы
    readSettings();
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
    qDebug() << "Opening file:" << filePath;
    delete m_dataParser;
    m_dataParser = new DataParser(filePath.toStdString());
    plotData();
}

void Charts::on_actionOSWES_toggled(bool checked)
{
    Q_UNUSED(checked);
    plotData();
}

void Charts::on_actionWES12_toggled(bool checked)
{
    Q_UNUSED(checked);
    plotData();
}

void Charts::on_actionWES34_toggled(bool checked)
{
    Q_UNUSED(checked);
    plotData();
}

void Charts::on_actionWES56_toggled(bool checked)
{
    Q_UNUSED(checked);
    plotData();
}

void Charts::on_actionWES78_toggled(bool checked)
{
    Q_UNUSED(checked);
    plotData();
}

void Charts::on_actionWES910_toggled(bool checked)
{
    Q_UNUSED(checked);
    plotData();
}

void Charts::on_actionWES1112_toggled(bool checked)
{
    Q_UNUSED(checked);
    plotData();
}

void Charts::plotData()
{
    if (!m_dataParser) return; // Если нет данных, ничего не делаем

    ui->customPlot->clearGraphs();

    QVector<double> xData = m_dataParser->getXData();
    QVector<double> yDataOSWES = m_dataParser->getYDataOSWES();
    QVector<double> yDataWES12 = m_dataParser->getYDataWES12();
    QVector<double> yDataWES34 = m_dataParser->getYDataWES34();
    QVector<double> yDataWES56 = m_dataParser->getYDataWES56(); // Get WES56 data
    QVector<double> yDataWES78 = m_dataParser->getYDataWES78(); // Get WES78 data
    QVector<double> yDataWES910 = m_dataParser->getYDataWES910(); // Get WES910 data
    QVector<double> yDataWES1112 = m_dataParser->getYDataWES1112(); // Get WES1112 data

    int graphIndex = 0;
    if (ui->action_2->isChecked()) {
        ui->customPlot->addGraph();
        ui->customPlot->graph(graphIndex)->setData(xData, yDataOSWES);
        ui->customPlot->graph(graphIndex)->setName("Вес");
        ui->customPlot->graph(graphIndex)->setPen(QPen(Qt::red));
        ++graphIndex;
    }

    if (ui->action_1_2->isChecked()) {
        ui->customPlot->addGraph();
        ui->customPlot->graph(graphIndex)->setData(xData, yDataWES12);
        ui->customPlot->graph(graphIndex)->setName("1-2");
        ui->customPlot->graph(graphIndex)->setPen(QPen(Qt::black));
        ++graphIndex;
    }

    if (ui->action_3_4->isChecked()) {
        ui->customPlot->addGraph();
        ui->customPlot->graph(graphIndex)->setData(xData, yDataWES34);
        ui->customPlot->graph(graphIndex)->setName("3-4");
        ui->customPlot->graph(graphIndex)->setPen(QPen(Qt::green));
        ++graphIndex;
    }

    if (ui->action_5_7->isChecked()) {
        ui->customPlot->addGraph();
        ui->customPlot->graph(graphIndex)->setData(xData, yDataWES56);
        ui->customPlot->graph(graphIndex)->setName("5-6"); // Adjust name as needed
        ui->customPlot->graph(graphIndex)->setPen(QPen(Qt::cyan));
        ++graphIndex;
    }

    if (ui->action_7_9->isChecked()) {
        ui->customPlot->addGraph();
        ui->customPlot->graph(graphIndex)->setData(xData, yDataWES78);
        ui->customPlot->graph(graphIndex)->setName("7-8"); // Adjust name as needed
        ui->customPlot->graph(graphIndex)->setPen(QPen(Qt::magenta));
        ++graphIndex;
    }

    if (ui->action_9_11->isChecked()) {
        ui->customPlot->addGraph();
        ui->customPlot->graph(graphIndex)->setData(xData, yDataWES910);
        ui->customPlot->graph(graphIndex)->setName("9-10"); // Adjust name as needed
        ui->customPlot->graph(graphIndex)->setPen(QPen(Qt::gray));
        ++graphIndex;
    }

    if (ui->action_11_13->isChecked()) {
        ui->customPlot->addGraph();
        ui->customPlot->graph(graphIndex)->setData(xData, yDataWES1112);
        ui->customPlot->graph(graphIndex)->setName("11-12"); // Adjust name as needed
        ++graphIndex;
    }

    ui->customPlot->rescaleAxes();
    ui->customPlot->replot();
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    // Add legend
    ui->customPlot->legend->setVisible(graphIndex > 0);
    if (graphIndex > 0) {
        ui->customPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));

        QFont legendFont = font();
        legendFont.setPointSize(10); // Adjust the size to your needs
        ui->customPlot->legend->setFont(legendFont);
        ui->customPlot->legend->setRowSpacing(-3); // Adjust the spacing to your needs
    }
}

void Charts::readSettings()
{
    QFile settingsFile(m_settingsFilePath);
    if (!settingsFile.exists()) {
        // Если файл не существует, создаем его с настройками по умолчанию
        QJsonObject defaultSettings;
        defaultSettings["OSWES"] = false;
        defaultSettings["WES12"] = false;
        defaultSettings["WES34"] = false;
        defaultSettings["WES56"] = false; // New setting for WES56
        defaultSettings["WES78"] = false; // New setting for WES78
        defaultSettings["WES910"] = false; // New setting for WES910
        defaultSettings["WES1112"] = false; // New setting for WES1112

        QJsonDocument doc(defaultSettings);
        if (settingsFile.open(QIODevice::WriteOnly)) {
            settingsFile.write(doc.toJson());
            settingsFile.close();
        }

        // Устанавливаем настройки по умолчанию в интерфейсе
        ui->action_2->setChecked(false);
        ui->action_1_2->setChecked(false);
        ui->action_3_4->setChecked(false);
        ui->action_5_7->setChecked(false); // Initialize WES56 action
        ui->action_7_9->setChecked(false); // Initialize WES78 action
        ui->action_9_11->setChecked(false); // Initialize WES910 action
        ui->action_11_13->setChecked(false); // Initialize WES1112 action
    } else {
        // Если файл существует, читаем его и применяем настройки
        if (settingsFile.open(QIODevice::ReadOnly)) {
            QByteArray settingsData = settingsFile.readAll();
            settingsFile.close();

            QJsonDocument doc = QJsonDocument::fromJson(settingsData);
            QJsonObject settings = doc.object();

            ui->action_2->setChecked(settings["OSWES"].toBool());
            ui->action_1_2->setChecked(settings["WES12"].toBool());
            ui->action_3_4->setChecked(settings["WES34"].toBool());
            ui->action_5_7->setChecked(settings["WES56"].toBool()); // Apply WES56 setting
            ui->action_7_9->setChecked(settings["WES78"].toBool()); // Apply WES78 setting
            ui->action_9_11->setChecked(settings["WES910"].toBool()); // Apply WES910 setting
            ui->action_11_13->setChecked(settings["WES1112"].toBool()); // Apply WES1112 setting
        }
    }
}

void Charts::writeSettings()
{
    QJsonObject settings;
    settings["OSWES"] = ui->action_2->isChecked();
    settings["WES12"] = ui->action_1_2->isChecked();
    settings["WES34"] = ui->action_3_4->isChecked();
    settings["WES56"] = ui->action_5_7->isChecked(); // Save WES56 setting
    settings["WES78"] = ui->action_7_9->isChecked(); // Save WES78 setting
    settings["WES910"] = ui->action_9_11->isChecked(); // Save WES910 setting
    settings["WES1112"] = ui->action_11_13->isChecked(); // Save WES1112 setting

    QJsonDocument doc(settings);
    QFile settingsFile(m_settingsFilePath);
    if (settingsFile.open(QIODevice::WriteOnly)) {
        settingsFile.write(doc.toJson());
        settingsFile.close();
    }
}

void Charts::closeEvent(QCloseEvent *event)
{
    writeSettings();
    QMainWindow::closeEvent(event);
}
