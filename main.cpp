#include "widget.h"
#include "handle.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    Handle h;
    h.show();
    //w.showFullScreen(); //fullscreen
    w.show();
    return a.exec();
}
