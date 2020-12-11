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

    Frac frac;
    frac.sum(6).sum(12, "a").div(9).div(-1);
    qDebug().noquote() << frac.format();
    frac.reduct();
    qDebug().noquote() << frac.format();

    Widget w;
    w.show();

    return a.exec();
}
