#include "widget.h"
#include <QApplication>
#include "Class/frac.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    Frac frac;
//    frac.sum(3).sum(2).sum(-7).sum(3, "a").div(2).sum(2, "b").multi(3);
//    qDebug().noquote() << frac.format();

//    Frac frac;
//    frac.sum(6).sum(12, "a").div(9);
//    qDebug().noquote() << frac.format();
//    frac.reduct();
//    qDebug().noquote() << "约分:                     " << frac.format();
//    frac.sub(Frac(1, "x").div(2));
//    qDebug().noquote() << "-x/2:                    " << frac.format();
//    Frac frac2 = frac.paramSep("x");
//    qDebug().noquote() << "原式=0，化为\"x=\"的形式:      x =" << frac2.format();

    Frac frac(2, "x");
    frac.sum(4, "y").div(3);
    qDebug().noquote() << frac.format();
    frac.substitute("x", Frac(3, "y").sum(4).div(2));
    qDebug().noquote() << frac.format();


    Widget w;
    w.show();

    return a.exec();
}
