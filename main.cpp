#include "widget.h"
#include <QApplication>
#include <QDebug>
#include "Parser/formulagroup.h"
#include "Widget/formulawidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Frac f(3);
    f.div(4).sum(PlainFrac(4).div(3)).moveNegativeToTop();
    qDebug() << f.format();

    Widget w;
    w.show();

    return a.exec();
}


