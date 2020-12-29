#include "widget.h"
#include <QApplication>
#include <QDebug>
#include "Parser/formulagroup.h"
#include "Widget/formulawidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    w.show();

    qDebug() << Frac(3, "a").div(2).div(PlainFrac(2, 3)).format();

    return a.exec();
}


