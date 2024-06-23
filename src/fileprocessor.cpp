// fileprocessor.cpp
#include "include/fileprocessor.h"

FileProcessor::FileProcessor(QObject *parent)
    : QThread(parent)
{
}

void FileProcessor::run()
{
    emit fileProcessingStarted();

    // Здесь выполняется загрузка и обработка файла
    // Примерно как в вашем методе openFile()

    emit fileProcessingFinished();
}

void FileProcessor::setFilePath(const QString &filePath)
{
    m_filePath = filePath;
}
