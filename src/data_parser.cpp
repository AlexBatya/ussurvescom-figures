#include "include/data_parser.h"

DataParser::DataParser(const std::string& filePath)
{
    parseXML(QString::fromStdString(filePath));
}

void DataParser::parseXML(const QString& filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			qWarning() << "Failed to open file" << filePath;
			return;
	}

	QDomDocument doc;
	if (!doc.setContent(&file)) {
			qWarning() << "Failed to parse XML file" << filePath;
			file.close();
			return;
	}
	file.close();

	QDomElement root = doc.documentElement();
	QDomNodeList rows = root.elementsByTagName("ROW");
	for (int i = 0; i < rows.count(); ++i) {
		QDomNode rowNode = rows.at(i);
		if (rowNode.isElement()) {
			QDomElement rowElement = rowNode.toElement();
			double xValue = rowElement.attribute("OSWES").toDouble();
			m_xData.append(static_cast<double>(m_xData.size()));
			m_yData.append(xValue);  // y value as the index
		}
	}
}
