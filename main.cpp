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

//    QList<Frac> lFracs = {      // a = 1, b = -1, c = 3
//        Frac(2, "a").sum(1, "b").sub(1),                // 2a + b = 1
//        Frac(1, "a").sum(1, "b").sum(1, "c").sub(3),    // a + b + c = 3
//        Frac(1, "a").sub(1, "b").sub(2),                // a - b = 2
//        Frac(3, "a").sum(1, "b").sub(1, "c").sum(1)     // 3a + b - c = -1
//    };
//    QStringList lUnkNumbers = { "a", "b", "c" };
//    Frac::solvingEquations(lFracs, lUnkNumbers);

//    QList<Frac> lFracs = {  // a = 3, b = 2/3, c = 1
//        Frac(1, "a").sum(1, "b").sub(Frac(11).div(3)),  // a + b = 11/3
//        Frac(1, "a").sum(3, "b").sub(2, "c").sub(3),    // a + 3b - 2c = 3
//        Frac(3, "b").sub(2, "c")                        // 3b - 2c = 0
//    };
//    QStringList lUnkNumbers = { "a", "b", "c" };
//    Frac::solvingEquations(lFracs, lUnkNumbers);

    QList<Frac> lFracs = {  // a = m + 2n, b = m - 2, c = n + 1
        Frac(1, "b").sub(2, "c").sub(Frac(1, "m").sub(2, "n").sub(4)), // b - 2c       = m - 2n - 4
        Frac(1, "a").sum(3, "c").sub(Frac(1, "m").sum(5, "n").sum(3)), // a + 3c       = m + 5n + 3
        Frac(1, "a").sub(1, "b").sum(2, "c").sub(Frac(4, "n").sum(4)), // a - b + 2c   = 4n + 4
        Frac(1, "a").sub(1, "c").sub(Frac(1, "m").sum(1, "n").sub(1)), // a - c        = m + n - 1
        Frac(1, "b").sum(1, "c").sub(Frac(1, "m").sum(1, "n").sub(1))  // b + c        = m + n - 1
    };
    QStringList lUnkNumbers = { "a", "b", "c" };
    Frac::solvingEquations(lFracs, lUnkNumbers);


    Widget w;
    w.show();

    return a.exec();
}

















