#include "widget.h"
#include <QApplication>
#include <QDebug>
#include "Parser/formulagroup.h"
#include "Widget/formulawidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //FormulaGroup f("Cu[+2]S[+6]O[-2]4_5H[+1]2O[-2]");
    FormulaGroup f("Mn[+7]O[-2]4");
    qDebug() << f.isVaild() << f.format() << f.getElec().format();

    Widget w;
    w.show();

    return a.exec();
}


