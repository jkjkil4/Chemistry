#include "widget.h"
#include <QApplication>
#include <QDebug>
#include "Parser/formula.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStringList list = { "Ba[+2]2", "O[0]2", "BaB" };

    for(QString element : list) {
        bool ok;
        Formula_Element b(element, &ok);
        qDebug() << "";
        if(ok) qDebug().noquote() << b.formatInfo();
        qDebug() << ok;
    }



    Widget w;
    w.show();

    return a.exec();
}


