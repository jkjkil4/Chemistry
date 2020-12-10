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

    static QVector<int> DigitFactor(int value);     //找指定数值的所有因数，不包括1
    QVector<int> findCommonFactor();    //找分母和分子的公因数，不包括1，结果为升序

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
