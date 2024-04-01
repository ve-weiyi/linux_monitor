#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDateTime>
#include <QMessageBox>
#include <QDateTime>
#include <QListWidget>
#include <QDebug>
#include <QMessageBox>
#include <QStandardItemModel>  //设置表格所用

#include <QFile>
#include <QTextStream>
#include <QFileDialog> //引用文件浏览对话框类

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void SystemTime();//显示当前系统时间
    void Updata_table();
    /************设置表格**************/
    void Set_table_cpu();//设置表格
    void Set_Table_mem(int mem_row,int mem_line);
    void Set_table_pro(int pro_row,int pro_line);

    QStringList Get_cpuinfo(QString path ="/proc/stat");//获得cpu信息
    QStringList Get_meminfo(QString path ="/proc/meminfo");//获得mem信息
    void Get_proinfo(QString path ="/proc");//获得process信息

    void Printf_cpu(QStringList cpulist);//打印CPU表格
    void Printf_mem(QStringList memlist);//打印memory表格
    void Printf_pro(QString proname,QStringList prolist,int count_pro);//打印process表格


private:
    Ui::MainWindow *ui;
    time_t system_time;
    struct tm *time_now;
    QTimer *timer;

    QString cpufile="/proc/stat";
    QString memfile="/proc/meminfo";
    QString profile="/proc";

    float cpu_usage;
    float mem_usage;

public:
    int showfile=0;
    int count_pro=0;

    int cpu_row=15;//行数
    int cpu_line=11;//列数
    int mem_row=40;//行数
    int mem_line=3;//列数
    int pro_row=300;//行数,缺省值
    int pro_line=14;//列数

    QStringList cpulist;
    QStringList memlist;
    QStringList prolist;
};

#endif // MAINWINDOW_H
