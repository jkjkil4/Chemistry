#ifndef SIMPLEFRAC_H
#define SIMPLEFRAC_H

#include <mlibheader.h>

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
    PlainFrac(QString str, bool *ok = nullptr);

    void reduct();

    PlainFrac& moveNegativeToTop();

    QString format(bool autoSpace = false) const;

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

    inline bool operator==(const PlainFrac &other) { return PlainFrac(*this).sub(other).a == 0; }
    inline bool operator<(const PlainFrac &other) { return PlainFrac(*this).sub(other).moveNegativeToTop().a < 0; }
    inline bool operator>(const PlainFrac &other) { return PlainFrac(*this).sub(other).moveNegativeToTop().a > 0; }
    inline bool operator<=(const PlainFrac &other) { return !operator>(other); }
    inline bool operator>=(const PlainFrac &other) { return !operator<(other); }
    inline bool operator!=(const PlainFrac &other) { return !operator==(other); }

private:
    int a = 0;
    int b = 1;
};

#endif // SIMPLEFRAC_H
