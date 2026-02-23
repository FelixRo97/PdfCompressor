#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <QObject>
#include <QStringList>
#include <QProcess>

class Compressor : public QObject
{
    Q_OBJECT

public:
    explicit Compressor(QObject *parent = nullptr);
    void compressFiles(const QStringList &files, const QString &preset);

signals:
    void fileFinished(const QString &file);
    void logMessage(const QString &msg);
    void allFinished();

private slots:
    void processNext();
    void onProcessFinished(int exitCode, QProcess::ExitStatus status);

private:
    QStringList fileQueue;
    QString preset;
    QProcess *process;
};

#endif
