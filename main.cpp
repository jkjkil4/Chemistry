#include "widget.h"
#include <QApplication>
#include "Parser/formulagroup.h"
#include "Widget/formulawidget.h"

#include "global.h"

FontFamily fontSourceCodePro;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //读取字体
    fontSourceCodePro.load(":/rc/rc/SourceCodePro-Medium.ttf");

    Widget w;
    w.show();

    return a.exec();
}


