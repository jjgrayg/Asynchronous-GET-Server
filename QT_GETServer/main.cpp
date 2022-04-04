#include "servercontroller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerController w;
    w.show();
    return a.exec();
}
