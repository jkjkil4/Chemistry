#include "widget.h"
#include <QApplication>
#include <QDebug>
#include "Parser/formulagroup.h"
#include "Widget/formulawidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FormulaGroup f("Mn[+7]O[-2]4");
    qDebug() << f.isVaild() << f.format() << f.elec().format();

    Widget w;
    w.show();

    return a.exec();
}


