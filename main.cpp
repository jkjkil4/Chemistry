#include "widget.h"
#include <QApplication>
#include <QDebug>
#include "Parser/formula.h"
#include "Widget/formulawidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Formula f(Formula::Group, "A[1](B[1]2C[1])2");
    qDebug() << f.isVaild();

    Widget w;
    w.show();

    return a.exec();
}


