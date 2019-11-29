#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle(QObject::tr("Warehouse"));
    w.setWindowIcon(QIcon(":/resources/icons/warehouse.svg"));
    w.show();

    return a.exec();
}
