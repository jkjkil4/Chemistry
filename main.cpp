#include "widget.h"
#include <QApplication>
#include <QDebug>
#include "Parser/formula.h"
#include "Widget/formulawidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QStringList list = {
        "Fe[+3](C[+2]N[-3])6", "Mn[+7]O[-2]4", "K[+1]", "To[+2947/114]114Nk[-2]514"
    };

    for(QString &str : list){
        bool ok;
        Formula *f = new Formula(str, 1, &ok);
        if(ok) {
            qDebug().noquote() << "\n解析" << str <<  "成功" << f->elec.format();
            FormulaWidget *w = new FormulaWidget(f);
            w->setAttribute(Qt::WA_DeleteOnClose);
            w->show();
        } else {
            qDebug().noquote() << "\n解析" << str << "失败";
            delete f;
        }
    }



    Widget w;
    w.show();

    return a.exec();
}


