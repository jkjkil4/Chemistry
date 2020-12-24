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
    PlainFrac() = default;
    PlainFrac(int value);
    PlainFrac(QString str, bool *ok = nullptr);

    void reduct();

    PlainFrac& moveNegativeToTop();

    QString format(bool autoSpace = false) const;

    PlainFrac& sum(int digit);
    PlainFrac& sub(int digit);
    PlainFrac& mul(int digit);
    PlainFrac& div(int digit);

    PlainFrac& sum(const PlainFrac &other);
    PlainFrac& sub(const PlainFrac &other);

private:
    int a = 0;
    int b = 1;
};

#endif // SIMPLEFRAC_H
