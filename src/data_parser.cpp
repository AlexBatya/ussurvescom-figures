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
            double yValue1 = rowElement.attribute("WES12").toDouble();
            double yValue2 = rowElement.attribute("WES34").toDouble();
            double yValue3 = rowElement.attribute("WES56").toDouble(); // New line for WES56
            double yValue4 = rowElement.attribute("WES78").toDouble(); // New line for WES78
            double yValue5 = rowElement.attribute("WES910").toDouble(); // New line for WES910
            double yValue6 = rowElement.attribute("WES1112").toDouble(); // New line for WES1112

            m_xData.append(static_cast<double>(m_xData.size()));
            m_yDataOSWES.append(xValue);
            m_yDataWES12.append(yValue1);
            m_yDataWES34.append(yValue2);
            m_yDataWES56.append(yValue3);
            m_yDataWES78.append(yValue4);
            m_yDataWES910.append(yValue5);
            m_yDataWES1112.append(yValue6);
        }
    }
}

QVector<double> DataParser::getXData() const {
    return m_xData;
}

QVector<double> DataParser::getYDataOSWES() const {
    return m_yDataOSWES;
}

QVector<double> DataParser::getYDataWES12() const {
    return m_yDataWES12;
}

QVector<double> DataParser::getYDataWES34() const {
    return m_yDataWES34;
}

QVector<double> DataParser::getYDataWES56() const {
    return m_yDataWES56;
}

QVector<double> DataParser::getYDataWES78() const {
    return m_yDataWES78;
}

QVector<double> DataParser::getYDataWES910() const {
    return m_yDataWES910;
}

QVector<double> DataParser::getYDataWES1112() const {
    return m_yDataWES1112;
}
