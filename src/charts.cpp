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
    this->setWindowTitle("Ussuvescom-figures"); // Устанавливаем заголовок окна
    ui->setupUi(this);

    connect(ui->action, &QAction::triggered, this, &Charts::on_actionOpen_triggered); // Подключаем сигнал открытия файла

    // Маппинг действий на имена данных для графиков
    actionToDataName[ui->action_2] = "OSWES";
    actionToDataName[ui->action_1_2] = "WES12";
    actionToDataName[ui->action_3_4] = "WES34";
    actionToDataName[ui->action_5_7] = "WES56";
    actionToDataName[ui->action_7_9] = "WES78";
    actionToDataName[ui->action_9_11] = "WES910";
    actionToDataName[ui->action_11_13] = "WES1112";

    // Подключаем все действия к общему слоту для обработки переключений графиков
    for (auto action : actionToDataName.keys()) {
        connect(action, &QAction::toggled, this, &Charts::on_plotToggled);
    }

    QIcon icon(":/icon.ico");
    setWindowIcon(icon); // Устанавливаем иконку окна

    setAcceptDrops(true); // Разрешаем перетаскивание файлов в окно

    // Устанавливаем путь к файлу настроек
    m_settingsFilePath = QDir(QCoreApplication::applicationDirPath()).filePath("settings.json");

    // Читаем настройки при запуске программы
    readSettings();
}

Charts::~Charts()
{
    delete ui;
    delete m_dataParser; // Удаляем парсер данных при уничтожении объекта
}

void Charts::dragEnterEvent(QDragEnterEvent *event)
{
    // Проверяем, содержит ли перетаскиваемый объект URL
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
    // Открываем диалог для выбора XML файла
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open XML File"), "", tr("XML Files (*.xml)"));
    if (!fileName.isEmpty()) {
        openFile(fileName);
    }
}

void Charts::openFile(const QString &filePath)
{
    // Открываем файл и создаем парсер данных
    qDebug() << "Opening file:" << filePath;
    delete m_dataParser;
    m_dataParser = new DataParser(filePath.toStdString());
    plotData(); // Обновляем графики
}

void Charts::on_plotToggled(bool checked)
{
    Q_UNUSED(checked);
    plotData(); // Обновляем графики при переключении
}

void Charts::plotData()
{
    if (!m_dataParser) return; // Если нет данных, ничего не делаем

    ui->customPlot->clearGraphs(); // Очищаем текущие графики

    QVector<double> xData = m_dataParser->getXData(); // Получаем данные по оси X
    // Маппинг имен данных на соответствующие данные и стили графиков
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

    int graphIndex = 0; // Индекс для добавляемых графиков
    // Добавляем графики для включенных действий
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

    ui->customPlot->rescaleAxes(); // Масштабируем оси
    ui->customPlot->replot(); // Перерисовываем график
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables | QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectItems | QCP::iMultiSelect); // Включаем взаимодействия

    // Добавляем легенду, если есть хотя бы один график
    ui->customPlot->legend->setVisible(graphIndex > 0);
    if (graphIndex > 0) {
        ui->customPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));

        QFont legendFont = font();
        legendFont.setPointSize(10); // Устанавливаем размер шрифта для легенды
        ui->customPlot->legend->setFont(legendFont);
        ui->customPlot->legend->setRowSpacing(-3); // Устанавливаем расстояние между строками легенды
    }
}

void Charts::readSettings()
{
    // Открываем файл настроек
    QFile settingsFile(m_settingsFilePath);
    if (!settingsFile.exists()) {
        // Если файл не существует, создаем его с настройками по умолчанию
        QJsonObject defaultSettings;
        for (auto key : actionToDataName.values()) {
            defaultSettings[key] = false;
        }

        QJsonDocument doc(defaultSettings);
        if (settingsFile.open(QIODevice::WriteOnly)) {
            settingsFile.write(doc.toJson());
            settingsFile.close();
        }

        // Устанавливаем настройки по умолчанию в интерфейсе
        for (auto action : actionToDataName.keys()) {
            action->setChecked(false);
        }
    } else {
        // Если файл существует, читаем его и применяем настройки
        if (settingsFile.open(QIODevice::ReadOnly)) {
            QByteArray saveData = settingsFile.readAll();
            settingsFile.close();
            QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
            QJsonObject settings = loadDoc.object();
            for (auto action : actionToDataName.keys()) {
                QString dataName = actionToDataName[action];
                if (settings.contains(dataName)) {
                    action->setChecked(settings[dataName].toBool());
                }
            }
        }
    }
}

void Charts::writeSettings()
{
    // Создаем объект JSON с текущими настройками
    QJsonObject settings;
    for (auto action : actionToDataName.keys()) {
        QString dataName = actionToDataName[action];
        settings[dataName] = action->isChecked();
    }

    // Записываем настройки в файл
    QJsonDocument doc(settings);
    QFile settingsFile(m_settingsFilePath);
    if (settingsFile.open(QIODevice::WriteOnly)) {
        settingsFile.write(doc.toJson());
        settingsFile.close();
    }
}

void Charts::closeEvent(QCloseEvent *event)
{
    writeSettings(); // Записываем настройки перед закрытием
    QMainWindow::closeEvent(event);
}

