#ifndef FRAC_H
#define FRAC_H

#include <QString>
#include <QMap>

#include <mlibheader.h>

#include "plainfrac.h"

class Frac
{
public:
    Frac() = default;
    Frac(int value, const QString &key = "");
    Frac(int a, int b);     //如果b为0，会抛出FracError错误
    Frac(QString str, bool *ok = nullptr);
    Frac(const PlainFrac &plain);

    PlainFrac toPlain();    //转为PlainFrac，假设Frac中的字母都为0

    static QList<Frac> SolvingEquations(QList<Frac> lFracs, const QStringList &lUnkNumbers, bool *ok = nullptr);    //解方程组

    void reduct();  //约分
    Frac& changeMono(const QString &before, const QString &now);
    Frac paramSep(const QString &param, bool *ok = nullptr);    //参变分离，假设该式等于0来处理

    Frac& substitute(const QString &param, int digit);              //代入数字
    Frac& substitute(const QString &param, const Frac &other);      //代入式子

    Frac& moveNegativeToTop();

    QString format(bool autoSpace = false, bool useColor = false) const;

    Frac& sum(int digit, const QString &key = "");
    Frac& sub(int digit, const QString &key = "");
    Frac& mul(int digit);
    Frac& div(int digit);   //如果digit为0，会抛出FracError错误

    Frac& sum(const Frac &other);
    Frac& sub(const Frac &other);

private:
    QMap<QString, int> mapPoly;
    int b = 1;
};


#endif // FRAC_H
