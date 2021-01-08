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

    Formula f(Formula::Group, "Ba2(SO4)2Cu[+2:a,b]Na2O[-2:c]H");
    qDebug() << f.isVaild();
    Formula::Iter iter(f);
    while(iter.hasNext()) {
        int count = iter.currentCount();
        Formula::Data data = iter.next();
        QDebug de = qDebug();
        de << count << data.name();
        if(data.hasGL()) {
            de << data.glValue().format() << data.glKeys();
        }
    }

    Widget w;
    w.show();

    return a.exec();
}


