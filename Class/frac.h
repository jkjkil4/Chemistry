#ifndef FRAC_H
#define FRAC_H

#include <QString>
#include <QMap>

#include "mheader.h"

class Frac
{
public:
    struct Error
    {
        Error(const QString &what) : what(what) {}
        QString what;
    };

    Frac() = default;
    Frac(int value, const QString &key = "");
    Frac(QString str, bool *ok = nullptr);

    QMap<QString, int> mapPoly;
    int b = 1;

//    void checkZero();

    static int Gcd(int a, int b);   //得到两个数的最大公因数
    static int Gcd(const QVector<int>& vValues, int n = -1);   //得到许多数的最大公因数

    static int Lcm(int a, int b);   //得到两个数的最小公倍数
    static int Lcm(const QVector<int>& vValues, int n = -1);   //得到许多数的最小公倍数

    static QList<Frac> SolvingEquations(QList<Frac> lFracs, const QStringList &lUnkNumbers, bool *ok = nullptr);

    void reduct();
    Frac paramSep(const QString &param, bool *ok = nullptr);    //参变分离，假设该式等于0来处理

    Frac& substitute(const QString &param, int digit);              //代入数字
    Frac& substitute(const QString &param, const Frac &other);      //代入式子

    Frac& moveNegativeToTop();

    QString format(bool autoSpace = true, bool useColor = true) const;

    Frac& sum(int digit, const QString &key = "");
    Frac& sub(int digit, const QString &key = "");
    Frac& mul(int digit);
    Frac& div(int digit);

    Frac& sum(const Frac &other);
    Frac& sub(const Frac &other);
};


#endif // FRAC_H
