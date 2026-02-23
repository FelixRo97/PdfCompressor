#include "mainwindow.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow w;
    app.setWindowIcon(QIcon(":/resources/app_icon.png"));
    w.resize(650, 700);
    w.show();

    return app.exec();
}
