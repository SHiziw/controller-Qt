#include "widget.h"
#include "ui_widget.h"

#include "handle.h"
#include "mybutton.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{

    ui->setupUi(this);





        QImage _image;
      _image.load(":/new/image/surface.png");
      setAutoFillBackground(true);   // 这个属性一定要设置
       QPalette pal(palette());
        pal.setBrush(QPalette::Window, QBrush(_image.scaled(size(), Qt::IgnoreAspectRatio,
                          Qt::SmoothTransformation)));
       setPalette(pal);
        //QPalette pal = palette(); //将背景设为透明
        //pal.setColor(QPalette::Background, QColor(0x00,0xff,0x00,0x00));
        //ui->handle1->setPalette(pal);


}

Widget::~Widget()
{
    delete ui;
}





Handle::Handle(QWidget *parent)
    : QWidget(parent)
{
    //setAutoFillBackground(true);
    //setPalette(QPalette(Qt::white));
    //resize(200,200);
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
    handleR=padR/3.5;//摇杆圆半径
    int handleMaxR=padR-handleR;
    QColor handleColor(186, 138, 96);
    //加载底盘图像
    painter.save();

    painter.scale(side / 400.0, side / 400.0);//坐标会随窗口缩放
    painter.drawPixmap(0, 0, QPixmap(":/new/image/pad_only2.png"));
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
    QPixmap sticker = QPixmap(":/new/image/pad_up2.png");
     painter.drawPixmap(QPoint(handleX-75,handleY-75), sticker.scaled(150,150));
    //painter.drawEllipse(QPoint(handleX,handleY),handleR,handleR);//摇杆
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
    x=(int)(handleX*10/(padR*2));
    y=(int)((padR*20-handleY*10)/(padR*2));
    keynum=10*y+x;
    return keynum;
}
