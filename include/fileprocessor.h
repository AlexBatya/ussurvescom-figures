// fileprocessor.h
#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#include <QThread>
#include <QString>

class FileProcessor : public QThread
{
    Q_OBJECT

public:
    explicit FileProcessor(QObject *parent = nullptr);
    void run() override;
    void setFilePath(const QString &filePath);

signals:
    void fileProcessingStarted();
    void fileProcessingFinished();

private:
    QString m_filePath;
};

#endif // FILEPROCESSOR_H
