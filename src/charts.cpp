#include "include/charts.h"
#include "qcustomplot.h"
#include "ui_charts.h"

Charts::Charts(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::Charts),
	m_dataParser(nullptr)
{
    this->setWindowTitle("Ussuvescom-figures");
	ui->setupUi(this);
	connect(ui->action, &QAction::triggered, this, &Charts::on_actionOpen_triggered);


	QIcon icon(":/icon.ico"); // Замените ":/icon.ico" на путь к вашей иконке
	setWindowIcon(icon);
}

Charts::~Charts()
{
	delete ui;
	delete m_dataParser;
}

void Charts::on_actionOpen_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open XML File"), "", tr("XML Files (*.xml)"));
	if (!fileName.isEmpty()) {
		delete m_dataParser;
		m_dataParser = new DataParser(fileName.toStdString());
		plotData();
	}
}

void Charts::plotData()
{
	QVector<double> xData = m_dataParser->getXData();
	QVector<double> yData = m_dataParser->getYData();

	ui->customPlot->addGraph();
	ui->customPlot->graph(0)->setData(xData, yData);
	ui->customPlot->rescaleAxes();
	ui->customPlot->replot();
	ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}
