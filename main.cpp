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

    QList<Frac> lFracs = {
        Frac(2, "a").sum(1, "b").sub(1),
        Frac(1, "a").sum(1, "b").sum(1, "c").sub(3),
        Frac(1, "a").sub(1, "b").sub(2),
        Frac(3, "a").sum(1, "b").sub(1, "c").sum(1)
    };
    QStringList lUnkNumbers = { "a", "b", "c" };
    Frac::solvingEquations(lFracs, lUnkNumbers);


    Widget w;
    w.show();

    return a.exec();
}
