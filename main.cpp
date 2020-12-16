#include "widget.h"
#include <QApplication>
#include <QDebug>
#include "Parser/formula.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    bool ok;
    Formula_Group("(BaCr2)11(SO2)45(BrCl4)14", 1, &ok);
    qDebug() << ok;


    Widget w;
    w.show();

    return a.exec();
}


