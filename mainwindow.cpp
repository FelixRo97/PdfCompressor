#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QUrl>
#include <QFileInfo>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    fileList = new QListWidget(this);
    addButton = new QPushButton("Add Files", this);
    removeButton = new QPushButton("Remove Selected", this);
    compressButton = new QPushButton("Compress", this);
    presetCombo = new QComboBox(this);
    progressBar = new QProgressBar(this);
    logOutput = new QTextEdit(this);

    logOutput->setReadOnly(true);
    progressBar->setValue(0);
    setAcceptDrops(true);
    fileList->setSelectionMode(QAbstractItemView::ExtendedSelection);

    presetCombo->addItem("Very Small", "/screen");
    presetCombo->addItem("Small", "/ebook");
    presetCombo->addItem("Medium ", "/printer");
    presetCombo->addItem("High Quality", "/prepress");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(new QLabel("Files:"));
    mainLayout->addWidget(fileList);

    QHBoxLayout *controlsLayout = new QHBoxLayout;
    controlsLayout->addWidget(addButton);
    controlsLayout->addWidget(removeButton);
    controlsLayout->addWidget(presetCombo);
    controlsLayout->addWidget(compressButton);

    mainLayout->addLayout(controlsLayout);
    mainLayout->addWidget(progressBar);
    mainLayout->addWidget(new QLabel("Log:"));
    mainLayout->addWidget(logOutput);

    central->setLayout(mainLayout);

    compressor = new Compressor(this);

    connect(addButton, &QPushButton::clicked,
            this, &MainWindow::addFiles);

    connect(removeButton, &QPushButton::clicked,
            this, &MainWindow::removeSelected);

    connect(compressButton, &QPushButton::clicked,
            this, &MainWindow::startCompression);

    connect(compressor, &Compressor::fileFinished,
            this, &MainWindow::onFileFinished);

    connect(compressor, &Compressor::logMessage,
            this, &MainWindow::onLogMessage);

    connect(compressor, &Compressor::allFinished,
            this, &MainWindow::onAllFinished);
}

void MainWindow::addFiles()
{
    QStringList files = QFileDialog::getOpenFileNames(
        this,
        "Select PDF Files",
        QString(),
        "PDF Files (*.pdf)"
        );

    for (const QString &file : files)
        fileList->addItem(file);
}

void MainWindow::removeSelected()
{
    QList<QListWidgetItem*> selectedItems = fileList->selectedItems();

    for (QListWidgetItem* item : selectedItems)
    {
        delete fileList->takeItem(fileList->row(item));
    }
}

void MainWindow::startCompression()
{
    if (fileList->count() == 0)
        return;

    QStringList files;
    for (int i = 0; i < fileList->count(); ++i)
        files << fileList->item(i)->text();

    QString preset = presetCombo->currentData().toString();

    progressBar->setMaximum(files.size());
    progressBar->setValue(0);
    logOutput->clear();

    compressor->compressFiles(files, preset);
}

void MainWindow::onFileFinished(const QString &)
{
    progressBar->setValue(progressBar->value() + 1);
}

void MainWindow::onLogMessage(const QString &msg)
{
    logOutput->append(msg);
}

void MainWindow::onAllFinished()
{
    logOutput->append("All files processed.");
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();

    for (const QUrl &url : urls)
    {
        QString filePath = url.toLocalFile();
        QFileInfo info(filePath);

        if (info.exists() && info.suffix().toLower() == "pdf")
        {
            fileList->addItem(filePath);
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete)
    {
        removeSelected();
    }
    else
    {
        QMainWindow::keyPressEvent(event);
    }
}
