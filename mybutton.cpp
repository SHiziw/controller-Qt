#include "mybutton.h"
#include "ui_widget.h"
#include <QPixmap>
#include <QBitmap>

MyButton::MyButton(QWidget *parent):
    QPushButton(parent)
{
    QPixmap pixmap(":/new/image/anniu.png");

    resize(pixmap.size());
    /* 设置按钮的有效区域 */
    setMask(QBitmap(pixmap.mask()));
    setStyleSheet("QPushButton{border-image: url(:/new/image/anniu.png);}");
}

void MyButton::enterEvent(QEvent *)
{
    setStyleSheet("QPushButton{border-image: url(:/new/image/anniuB.png);}");
}

void MyButton::leaveEvent(QEvent *)
{
    setStyleSheet("QPushButton{border-image: url(:/new/image/anniu.png);}");
}
