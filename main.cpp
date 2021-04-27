#include "widget.h"
#include "handle.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    //w.showFullScreen(); //fullscreen
    w.show();
    return a.exec();
}
