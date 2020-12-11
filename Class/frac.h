#ifndef FRAC_H
#define FRAC_H

#include <QString>
#include <QMap>

class Frac
{
public:
    struct Error
    {
        Error(const QString &what) : what(what) {}
        QString what;
    };

    QMap<QString, int> mapPoly;
    int b = 1;

    void checkZero();

    static int Gcd(int a, int b);   //得到两个数的最大公因数
    static int Gcd(const QVector<int>& vValues, int n = -1);   //得到许多数的最大公因数

    static int Lcm(int a, int b);   //得到两个数的最小公倍数
    static int Lcm(const QVector<int>& vValues, int n = -1);   //得到许多数的最小公倍数

    void reduct();

    QString format(bool autoSpace = true, bool useColor = true);

    Frac& sum(int digit, const QString &key = "");
    Frac& sub(int digit, const QString &key = "");
    Frac& multi(int digit);
    Frac& div(int digit);

//    Frac& sum(const Frac &other);
//    Frac& sub(const Frac &other);

    friend inline Frac operator+(Frac frac, int digit) {
        frac.sum(digit);
        return frac;
    }
};


#endif // FRAC_H
