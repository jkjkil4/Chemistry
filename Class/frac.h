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
    Frac(int value, char) = delete;
    Frac(int value, QChar) = delete;
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
    Frac& mul(const PlainFrac &other);
    Frac& div(const PlainFrac &other);  //如果other为0，会抛出FracError错误

    //下列重载的运算符会发生拷贝(在某些时候可能降低效率)
    friend inline Frac operator+(const Frac &a, const Frac &b) { return Frac(a).sum(b); }
    friend inline Frac operator-(const Frac &a, const Frac &b) { return Frac(a).sub(b); }
    friend inline Frac operator*(const Frac &a, const PlainFrac &b) { return Frac(a).mul(b); }
    friend inline Frac operator*(const PlainFrac &a, const Frac &b) { return Frac(b).mul(a); }
    friend inline Frac operator/(const Frac &a, const PlainFrac &b) { return Frac(a).div(b); }

    //对自运算符的重载
    inline Frac& operator+=(const Frac &other) { return sum(other); }
    inline Frac& operator-=(const Frac &other) { return sub(other); }
    inline Frac& operator*=(const PlainFrac &other) { return mul(other); }
    inline Frac& operator/=(const PlainFrac &other) { return div(other); }

private:
    QMap<QString, int> mapPoly;
    int b = 1;
};


#endif // FRAC_H
