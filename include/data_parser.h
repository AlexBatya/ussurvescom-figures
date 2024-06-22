#ifndef DATA_PARSER_H
#define DATA_PARSER_H

#include <QVector>
#include <QString>
#include <QDomDocument>
#include <QFile>
#include <QDebug>

class DataParser
{
public:
    DataParser(const std::string& filePath);
    QVector<double> getXData() const;
    QVector<double> getYDataOSWES() const;
    QVector<double> getYDataWES12() const;
    QVector<double> getYDataWES34() const;
    QVector<double> getYDataWES56() const;
    QVector<double> getYDataWES78() const;
    QVector<double> getYDataWES910() const;
    QVector<double> getYDataWES1112() const;

private:
    void parseXML(const QString& filePath);
    QVector<double> m_xData;
    QVector<double> m_yDataOSWES;
    QVector<double> m_yDataWES12;
    QVector<double> m_yDataWES34;
    QVector<double> m_yDataWES56;
    QVector<double> m_yDataWES78;
    QVector<double> m_yDataWES910;
    QVector<double> m_yDataWES1112;
};

#endif // DATA_PARSER_H
