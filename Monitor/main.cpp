#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qDebug()<<"linux系统监控器";
    QApplication a(argc, argv);
    MainWindow w;
    w.move(200,200);
    w.show();
    qDebug()<<"运行结束";
    a.exec();

    return 0;
}
