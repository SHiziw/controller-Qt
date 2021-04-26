#include "widget.h"
#include "ui_widget.h"

#include "handle.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{

    ui->setupUi(this);
        model=new QFileSystemModel(this); //QFileSystemModel提供单独线程，推荐使用
        model->setRootPath(QDir::currentPath()); //设置根目录
        ui->treeView->setModel(model); //设置数据模型
        ui->listView->setModel(model); //设置数据模型
        ui->tableView->setModel(model); //设置数据模型
        //信号与槽关联，treeView单击时，其目录设置为listView和tableView的根节点
        QStringList theStrList; //保存初始 StringList
        theStrList<<"北京"<<"上海"<<"天津"<<"河北"<<"山东"<<"四川"<<"重庆"<<"广东"<<"河南"; //初始化 StringList
        theModel=new QStringListModel(this); //创建数据模型
        theModel->setStringList(theStrList); //为模型设置StringList，会导入StringList的内容
        ui->listView->setModel(theModel); //为listView设置数据模型
        ui->listView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
        connect(ui->treeView,SIGNAL(clicked(QModelIndex)),
                ui->listView,SLOT(setRootIndex(QModelIndex)));
        connect(ui->treeView,SIGNAL(clicked(QModelIndex)),
                ui->tableView,SLOT(setRootIndex(QModelIndex)));
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_calculatorBottom_clicked()
{
    QString str=ui->NumberTextEdit->toPlainText();//读取n数量n
        int num=str.toInt();
        str=ui->PriceTextEdit->toPlainText(); //读取"单价"
        float price=str.toFloat();
        float total=num*price;
        str=str.sprintf ("%.2f",total);
        ui->TotalLineEdit->setText(str);

}
void Widget::on_btnOpen_clicked()
{ //选择单个文件
    QString curPath=QDir::currentPath();//获取系统当前目录
    //获取应用程序的路径
    QString dlgTitle="选择一个文件"; //对话框标题
    QString filter="文本文件(*.txt);;图片文件(*.jpg *.gif *.png);;所有文件(*.*)"; //文件过滤器
    QString aFileName=QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);
    if (!aFileName.isEmpty())
        ui->plainTextEdit->appendPlainText(aFileName);
}


Handle::Handle(QWidget *parent)
    : QWidget(parent)
{
    setAutoFillBackground(true);
    setPalette(QPalette(Qt::white));
    resize(200,200);
    setMinimumSize(100,100);
    mouseX=width()/2;
    mouseY=height()/2;
    tim=new QTimer(this);
    connect(tim,&QTimer::timeout,this,[=]{
       emit keyNumchanged(getKeyNum());
   });
    connect(this,&Handle::keyNumchanged,this,[=](int num){
        qDebug()<<num<<endl;
    });
}

Handle::~Handle()
{

}
void Handle::paintEvent(QPaintEvent *){

    QPainter painter(this);

    int side = qMin(width(), height());

    padR=side/2; //底盘半径
    padX=padR;//底盘圆心
    padY=padR;//底盘圆心
    handleR=padR/4;//摇杆圆半径
    int handleMaxR=padR-handleR;
    QColor handleColor(Qt::gray);
    //加载底盘图像
    painter.save();

    painter.scale(side / 400.0, side / 400.0);//坐标会随窗口缩放
    painter.drawPixmap(0, 0, QPixmap(":/image/pad.png"));
    painter.restore();

    //自绘底盘
//    painter.save();
//    QRadialGradient RadialGradient(padR,padR,padR*3,padR,padR);//圆心2，半径1，焦点2
//    RadialGradient.setColorAt(0,QColor(90,90,90,127));//渐变
//    RadialGradient.setColorAt(1,QColor(255,255,255,190));//渐变
//    painter.setBrush(RadialGradient);
//    painter.setPen(Qt::NoPen);
//    painter.drawEllipse(QPoint(padR,padR),side/2,side/2);//大圆盘
//    painter.restore();

 //   painter.drawText(20,20,tr("%1,%2,%3").arg(mouseX).arg(mouseY).arg(handPaddis));

    if(!mousePressed){//鼠标没按下则摇杆恢复到底盘中心
        mouseX=padX;
        mouseY=padY;
    }
    handPadDis=Pointdis(padR,padR,mouseX,mouseY);
    if(handPadDis<=handleMaxR){
        handleX=mouseX;
        handleY=mouseY;
    }
    else {
        handleX=(int)(handleMaxR*(mouseX-padX)/handPadDis+padX);
        handleY=(int)(handleMaxR*(mouseY-padY)/handPadDis+padY);
    }
   // painter.drawText(200,200,tr("%1,%2,%3").arg(handleX).arg(handleY).arg(handPaddis));
    painter.setPen(Qt::NoPen);
    painter.setBrush(handleColor);
    painter.drawEllipse(QPoint(handleX,handleY),handleR,handleR);//摇杆
}
void Handle::mouseMoveEvent(QMouseEvent* event){
    static bool r=false;
    mouseX=event->pos().x();
    mouseY=event->pos().y();
    if(r==true){
       update();
       r=false;
    }
    else{
        r=true;
    }
}
void Handle::mouseReleaseEvent(QMouseEvent* event){
    mouseX=width()/2;
    mouseY=height()/2;
    tim->stop();
    mousePressed=false;
    emit keyNumchanged(Handle::stop);
    update();
}
void Handle::mousePressEvent(QMouseEvent* event){
    mouseX=event->pos().x();
    mouseY=event->pos().y();
    tim->start(100);
    mousePressed=true;
    update();
}

double Handle::Pointdis(int a,int b,int x,int y){
    return sqrt((double)((x-a)*(x-a)+(y-b)*(y-b)));
}
int Handle::getKeyNum(){
    int x,y;
    int keynum;
    x=(int)(handleX*3.0/(padR*2));
    y=(int)(handleY*3.0/(padR*2));
    keynum=3*y+x;
    return keynum;
}
