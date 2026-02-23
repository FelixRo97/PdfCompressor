#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QComboBox>
#include <QProgressBar>
#include <QTextEdit>
#include <QStringList>

#include "pdfcompressor.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void addFiles();
    void removeSelected();
    void startCompression();
    void onFileFinished(const QString &file);
    void onLogMessage(const QString &msg);
    void onAllFinished();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QListWidget *fileList;
    QPushButton *addButton;
    QPushButton *removeButton;
    QPushButton *compressButton;
    QComboBox *presetCombo;
    QProgressBar *progressBar;
    QTextEdit *logOutput;

    Compressor *compressor;
};

#endif
