#include "widget.h"
#include <QApplication>
#include <QDebug>
#include "Parser/formula.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    QStringList list = {
        "Mn[+7]O[-2]4", "O[0]2", "NaN", "BaB", "",
        "S[+6]O[-2]4", "Fe[+8/3]", "Fe[+3](C[+2]N[-3])6", "K[+1]3(Fe[+3](C[+2]N[-3])6)"
    };

    for(QString str : list) {
        bool ok;
        Formula f(str, 1, &ok);
        if(ok) {
            qDebug().noquote() << "\n解析" << str << "成功\n电荷数:" << f.elec.format();
        } else {
            qDebug().noquote() << "\n解析" << str << "失败";
        }
    }



    Widget w;
    w.show();

    return a.exec();
}


