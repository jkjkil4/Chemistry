#include "widget.h"
#include <QApplication>
#include "Parser/formulagroup.h"
#include "Widget/formulawidget.h"

#include "global.h"

#include <QDebug>

FontFamily fontSourceCodePro;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //读取字体
    fontSourceCodePro.load(":/rc/rc/SourceCodePro-Medium.ttf");

    Formula f(Formula::Group, "Ba2(SO4)2CuNaOH");
    qDebug() << f.isVaild();
    Formula::Iter iter(f);
    while(iter.hasNext()) {
        Formula::Data data = iter.next();
        qDebug() << data.isVaild();
        qDebug() << data.count() << data.name();
    }

    Widget w;
    w.show();

    return a.exec();
}


