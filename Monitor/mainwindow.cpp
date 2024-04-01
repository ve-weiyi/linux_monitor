#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("linux系统监控");
    //Set_Table();
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(SystemTime()));
    connect(timer,SIGNAL(timeout()),this,SLOT(Updata_table()));

    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}
/***************系统时间刷新*****************/
void MainWindow::SystemTime()
{
    //qDebug()<<"刷新系统时间";
    ui->timelabel->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));
    ui->datelabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
}
/***************信息表格刷新*****************/
void MainWindow::Updata_table()
{
    //qDebug()<<"刷新表格信息";
    cpulist=Get_cpuinfo(cpufile);
    if(!cpulist.empty())
    {
        Printf_cpu(cpulist);
    }
    memlist=Get_meminfo(memfile);
    if(!cpulist.empty())
    {
        Printf_mem(memlist);
    }
    Get_proinfo(profile);
}
/***************获得CPU信息****************/
QStringList MainWindow::Get_cpuinfo(QString path)
{
    //获取文件路径及名称
    QStringList cpulist;
    QByteArray array;
    if(path.isEmpty() == false)//判断路径是否为空
    {
        QFile file(path);//指定文件路径
        bool isOk = file.open(QIODevice::ReadOnly);//检查文件打开情况
        if(isOk == true)
        {
            array = file.readAll();//读文件 内容放到字节组
            cpulist = QString(array).split('\n');       //通过“\n”将整个文件内容按字符串进行存储
        }
        else
        {
            qDebug()<<"文件打开错误"<<"\n"<<path;
            return {};
        }
        file.close();//文件关闭
    }
    if(showfile==1){
    QFileInfo  info(path);
    qDebug()<<"获取CPU信息"<<"\n"<<cpulist;
    qDebug()<<"文件创建时间："<<info.created().toString("yyyy-MM-dd hh:mm:ss");//2019-01-01 11：11：00的格式
    }
    return cpulist;
}
/***************获得内存信息*****************/
QStringList MainWindow::Get_meminfo(QString path)
{
    //QString path = QFileDialog::getOpenFileName(this,"打开文件","/proc/meminfo");//获取文件路径及名称
    //QString path ="/proc/meminfo";
    QStringList memlist;
    QByteArray array;
    if(path.isEmpty() == false)//判断路径是否为空
    {
        QFile file(path);//指定文件路径
        bool isOk = file.open(QIODevice::ReadOnly);//检查文件打开情况
        if(isOk == true)
        {
            array = file.readAll();//读文件 内容放到字节组
            memlist = QString(array).split('\n');        //通过“\n”将整个文件内容按字符串进行存储
        }
        file.close();//文件关闭
    }
    //qDebug()<<path<<"memlist"<<memlist;
    return memlist;
}
/***************process信息显示*****************/
void MainWindow::Get_proinfo(QString path)
{
    QString statfile;//暂时文件名
    QStringList prolist;
    QString proname,proinfo;
    QByteArray array;
    ui->tableWidget_pro->clearContents();//每次刷新内容
    count_pro=0;
    QDir *dir=new QDir(path);
    QStringList filter;//文件名表单
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir->entryInfoList(filter));//获取文件夹下所有文件数目及名字
    //设置表格
    for(int i = 0,pro_row=0;i<fileInfo->count(); i++)
    {
        statfile=fileInfo->at(i).filePath()+"/stat";
        //qDebug()<<pro_row<<statfile;
        QFile file(statfile);//指定文件路径
        bool isOk = file.open(QIODevice::ReadOnly);
        if(isOk && statfile.isEmpty()== false && statfile!="/proc/./stat")//判断路径是否为空
            pro_row++;
        if(i==fileInfo->count()-1)
            Set_table_pro(pro_row,pro_line);
    }

    for(int i = 0;i<fileInfo->count(); i++)
    {
        statfile=fileInfo->at(i).filePath()+"/stat";
        //qDebug()<<statfile+"   "<<fileInfo->at(i).path();//路径
        if(statfile.isEmpty() == false&&statfile!="/proc/./stat")//判断路径是否为空
        {
            QFile file(statfile);//指定文件路径
            bool isOk = file.open(QIODevice::ReadOnly);//检查文件打开情况
            if(isOk == true)
            {
                array = file.readAll();//读文件 内容放到字节组
                proinfo=QString(array);
                int start = proinfo.indexOf("(", 0);
                int end = proinfo.lastIndexOf(")");
                QString proname = proinfo.mid(start, end-start+1);//提取进程名字
                proinfo.remove(proname);//提取进程其他信息
                prolist = proinfo.split(QRegExp("\\s+"), QString::SkipEmptyParts);        //通过将整个文件内容按字符串进行存储
                //qDebug()<<count_pro++<<proname<<proinfo;
                if(count_pro<fileInfo->count())
                    Printf_pro(proname,prolist,count_pro++);
            }
            file.close();//文件关闭
        }
    }
}
void MainWindow::Printf_cpu(QStringList cpulist)
{
    //qDebug()<<"获取CPU信息\n"<<cpulist;
    ui->tableWidget_cpu->clearContents();//每次刷新内容
    Set_table_cpu();
    QStringList temlist;
    for(int i=0;i<3;i++)
        {
          temlist = cpulist.at(i).split(QRegExp("\\s+"), QString::SkipEmptyParts);//按一个或多个空格分割
          if(i==0)//计算CPU使用率
          {
            cpu_usage=100*(temlist.at(0).toFloat()+temlist.at(1).toFloat()+temlist.at(2).toFloat())/(temlist.at(0).toFloat()+temlist.at(1).toFloat()+temlist.at(2).toFloat()+temlist.at(3).toFloat());
            //qDebug()<<cpu_usage<<"cpu使用率\n";
            ui->cpu_usage->setText(QString("%1").arg(cpu_usage)+"%");
          }
            for(int j=0;j<cpu_line;j++)
            {
               ui->tableWidget_cpu->setItem(i,j,new QTableWidgetItem(temlist.at(j)));
            }
        }

    ui->tableWidget_cpu->setItem(3,0,new QTableWidgetItem("# 这行给出中断的信息，第一个为自系统启动以来，发生的所有的中断的次数\n# 然后每个数对应一个特定的中断自系统启动以来所发生的次数。"));
    ui->tableWidget_cpu->setItem(4,0,new QTableWidgetItem(cpulist.at(3)));

    for(int i=5;i<10;i++)
        {
        temlist = cpulist.at(i).split(QRegExp("\\s+"), QString::SkipEmptyParts);//按一个或多个空格分割
        //qDebug()<<"获取CPU信息\n"<<temlist;
        for(int j=0;j<2;j++)
            {
            ui->tableWidget_cpu->setItem(i+3,j,new QTableWidgetItem(temlist.at(j)));
            }
        }

    temlist = cpulist.at(9).split(QRegExp("\\s+"), QString::SkipEmptyParts);//按一个或多个空格分割
    for(int j=0;j<cpu_line;j++)//最后一行
        {
        ui->tableWidget_cpu->setItem(12,j,new QTableWidgetItem(temlist.at(j)));
        }
}

void MainWindow::Printf_mem(QStringList memlist)
{
    ui->tableWidget_mem->clearContents();//每次刷新内容
    mem_row=0;
    while(memlist.at(mem_row).isEmpty() == false)
        mem_row++;//获取内存信息行数
    Set_Table_mem(mem_row,mem_line);
    for(int i=0;i<mem_row;i++)
        {
        ui->tableWidget_mem->setItem(i,0,new QTableWidgetItem(memlist.at(i)));
        }
     /***************计算内存使用率*****************/
     QStringList tem1,tem2;
     tem1 = memlist.at(0).split(QRegExp("\\s+"), QString::SkipEmptyParts);//按一个或多个空格分割
     tem2 = memlist.at(1).split(QRegExp("\\s+"), QString::SkipEmptyParts);//按一个或多个空格分割
     mem_usage=(1-(tem2.at(1).toFloat()/tem1.at(1).toFloat()))*100;
     //qDebug()<<tem1<<"获取CPU信息\n"<<tem2<<"获取CPU信息\n"<<mem_usage;
     ui->mem_usage->setText(QString("%1").arg(mem_usage)+"%");
}

void MainWindow::Printf_pro(QString proname,QStringList prolist,int count_pro)
{
    //qDebug()<<count_pro<<"获取进程信息\n"<<prolist;
    for(int j=0;j<pro_line;j++)
     {
          //qDebug()<<prolist.at(j);
        if(j==0)
            ui->tableWidget_pro->setItem(count_pro,j,new QTableWidgetItem(proname));
        else
            ui->tableWidget_pro->setItem(count_pro,j,new QTableWidgetItem(prolist.at(j-1)));
      }
}

void MainWindow::Set_table_cpu()
{
    /**********设置CPU信息**********/
    ui->tableWidget_cpu->setRowCount(cpu_row); //设置行数为10
    ui->tableWidget_cpu->setColumnCount(cpu_line); //设置列数为5
    ui->tableWidget_cpu->setSpan(3, 0, 1, 11);
    ui->tableWidget_cpu->setSpan(4, 0, 3, 11);
    ui->tableWidget_cpu->setShowGrid(false);//显示表格线
    ui->tableWidget_cpu->verticalHeader()->setVisible(false);//隐藏列名
    ui->tableWidget_cpu->setColumnWidth(0,120);//设置第一列宽度

    QStringList headers1 = {"name", "user", "nice","system", "idle", "iowait","irrq", "softirq", "steal","guest" ,"guest_nice"};
    ui->tableWidget_cpu->setHorizontalHeaderLabels(headers1);
    //qDebug()<<"设置表格标题cpu"<<headers1;
}
void MainWindow::Set_Table_mem(int mem_row,int mem_line)
{
    /**********设置内存信息**********/
    ui->tableWidget_mem->setRowCount(mem_row);
    ui->tableWidget_mem->setColumnCount(mem_line);
    ui->tableWidget_mem->horizontalHeader()->setVisible(false);//隐藏行名
    ui->tableWidget_mem->setColumnWidth(0,800);
    ui->tableWidget_mem->setShowGrid(false);//显示表格线
}
void MainWindow::Set_table_pro(int pro_row,int pro_line)
{
    /**********设置进程信息**********/
    ui->tableWidget_pro->setRowCount(pro_row);
    ui->tableWidget_pro->setColumnCount(pro_line);
    //ui->tableWidget_pro->resizeColumnsToContents(); //行列宽度匹配
    //ui->tableWidget_pro->resizeRowsToContents();
    QStringList headers2 = {"进程名称", "进程号","状态", "父进程ID","线程组ID", "会话", "终端设备号","进程组号", "进程标志位", "累计缺页","子进程缺页" ,"主缺页数","用户态运行时间" ,"核心态运行时间"};
    ui->tableWidget_pro->setHorizontalHeaderLabels(headers2);
    //qDebug()<<"设置表格标题pro"<<headers2;
}
