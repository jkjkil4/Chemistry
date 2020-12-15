#include "widget.h"
#include <QApplication>
#include <QDebug>
#include "Parser/formula.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Formula_Group("BaSO4");
    qDebug() << "";
    Formula_Group("NaOH");
    qDebug() << "";
    Formula_Group("HCl");


    Widget w;
    w.show();

    return a.exec();
}


