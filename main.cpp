#include "widget.h"
#include <QApplication>
#include <QDebug>
#include "Parser/formula.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Formula_Group("Ba[+2]S[+6]O[-2]4");

    qDebug() << QString(" -  2").replace(' ', "").toInt();


    Widget w;
    w.show();

    return a.exec();
}


