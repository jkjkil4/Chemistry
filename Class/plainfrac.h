#ifndef SIMPLEFRAC_H
#define SIMPLEFRAC_H

#include <QString>
#include <Lib/header.h>
#include <Lib/math.h>

struct FracError
{
    FracError(const QString &what) : what(what) {}
    QString what;
};

class PlainFrac
{
public:
    friend class Frac;

    PlainFrac() = default;
    PlainFrac(int value);
    PlainFrac(int a, int b);    //如果b为0，会抛出FracError错误
    explicit PlainFrac(QString str, bool *ok = nullptr);

    void reduct();

    PlainFrac& moveNegativeToTop();

    QString format(bool autoSpace = false) const;

    PlainFrac& abs();

    PlainFrac& sum(int digit);
    PlainFrac& sub(int digit);
    PlainFrac& mul(int digit);
    PlainFrac& div(int digit);  //如果digit为0，会抛出FracError错误

    PlainFrac& sum(const PlainFrac &other);
    PlainFrac& sub(const PlainFrac &other);
    PlainFrac& mul(const PlainFrac &other);
    PlainFrac& div(const PlainFrac &other); //如果other为0，会抛出FracError错误

    int top() const { return a; }
    int bottom() const { return b; }

    //下列重载的运算符会发生拷贝(在某些时候可能降低效率)
    friend inline PlainFrac operator+(const PlainFrac &a, const PlainFrac &b) { return PlainFrac(a).sum(b); }
    friend inline PlainFrac operator-(const PlainFrac &a, const PlainFrac &b) { return PlainFrac(a).sub(b); }
    friend inline PlainFrac operator*(const PlainFrac &a, const PlainFrac &b) { return PlainFrac(a).mul(b); }
    friend inline PlainFrac operator/(const PlainFrac &a, const PlainFrac &b) { return PlainFrac(a).div(b); }

    //对自运算符的重载
    inline PlainFrac& operator+=(const PlainFrac &other) { return sum(other); }
    inline PlainFrac& operator-=(const PlainFrac &other) { return sub(other); }
    inline PlainFrac& operator*=(const PlainFrac &other) { return mul(other); }
    inline PlainFrac& operator/=(const PlainFrac &other) { return div(other); }

    //对逻辑运算符的重载
    inline bool operator==(const PlainFrac &other) const { return (*this - other).a == 0; }
    inline bool operator< (const PlainFrac &other) const { return (*this - other).moveNegativeToTop().a < 0; }
    inline bool operator> (const PlainFrac &other) const { return (*this - other).moveNegativeToTop().a > 0; }
    inline bool operator<=(const PlainFrac &other) const { return !operator>(other); }
    inline bool operator>=(const PlainFrac &other) const { return !operator<(other); }
    inline bool operator!=(const PlainFrac &other) const { return !operator==(other); }

private:
    int a = 0;
    int b = 1;
};

#endif // SIMPLEFRAC_H
