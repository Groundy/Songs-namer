#include "Songsnamer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Songsnamer w;
    w.show();
    return a.exec();
}
