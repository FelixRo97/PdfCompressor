#include "pdfcompressor.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>

Compressor::Compressor(QObject *parent)
    : QObject(parent),
    process(new QProcess(this))
{
    connect(process,
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this,
            &Compressor::onProcessFinished);
}

void Compressor::compressFiles(const QStringList &files,
                               const QString &presetSetting)
{
    fileQueue = files;
    preset = presetSetting;
    processNext();
}

void Compressor::processNext()
{
    if (fileQueue.isEmpty())
    {
        emit allFinished();
        return;
    }

    QString inputFile = fileQueue.takeFirst();
    QFileInfo info(inputFile);

    QString outputFile = info.absolutePath() + "/"
                         + info.completeBaseName()
                         + "_compressed.pdf";

#ifdef Q_OS_WIN
    QString gsExecutable = QCoreApplication::applicationDirPath()
                           + "/ghostscript/gswin64.exe";
#else
    QString gsExecutable = QCoreApplication::applicationDirPath()
                           + "/ghostscript/gs";
#endif

    QStringList args;
    args << "-sDEVICE=pdfwrite"
         << "-dCompatibilityLevel=1.4"
         << "-dPDFSETTINGS=" + preset
         << "-dNOPAUSE"
         << "-dQUIET"
         << "-dBATCH"
         << "-sOutputFile=" + outputFile
         << inputFile;

    emit logMessage("Compressing: " + inputFile);

    process->start(gsExecutable, args);
}

void Compressor::onProcessFinished(int exitCode,
                                   QProcess::ExitStatus status)
{
    Q_UNUSED(status)

    if (exitCode == 0)
        emit logMessage("Success.");
    else
        emit logMessage("Error during compression.");

    emit fileFinished("");

    processNext();
}
