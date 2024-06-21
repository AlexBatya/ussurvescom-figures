#ifndef DATA_PARSER_H
#define DATA_PARSER_H

#include <QVector>
#include <QString>
#include <QDomDocument>
#include <QFile>
#include <QDebug>

class DataParser {
public:
    DataParser(const std::string& filePath);
    QVector<double> getXData() const { return m_xData; }
    QVector<double> getYData() const { return m_yData; }

private:
    void parseXML(const QString& filePath);

    QVector<double> m_xData;
    QVector<double> m_yData;
};

#endif // DATA_PARSER_H
