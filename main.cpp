#include "widget.h"
#include <QApplication>
//#include <QDebug>
#include "Parser/formulagroup.h"
#include "Widget/formulawidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    w.show();

    return a.exec();
}


