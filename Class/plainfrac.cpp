#include "plainfrac.h"


PlainFrac::PlainFrac(int value) {
    if(value != 0)
        a = value;
}

PlainFrac::PlainFrac(int a, int b) {
    if(b == 0)
        throw FracError("cannot div 0");
    this->a = a;
    this->b = b;
    reduct();
}

PlainFrac::PlainFrac(QString str, bool *ok) {
    str.replace(' ', "");
    int indexOfDiv = str.indexOf("/");
    if(indexOfDiv == -1) {
        bool ok2;
        int value = str.toInt(&ok2);
        if(ok2) {
            a = value;
        } else {
            SET_PTR(ok, false);
            return;
        }
    } else {
        bool ok2;
        int top = str.left(indexOfDiv).toInt(&ok2);
        if(!ok2) {
            SET_PTR(ok, false);
            return;
        }
        int bottom = str.right(str.length() - indexOfDiv - 1).toInt(&ok2);
        if(!ok2 || bottom == 0) {
            SET_PTR(ok, false);
            return;
        }
        a = top;
        b = bottom;
    }

    reduct();
    SET_PTR(ok, true);
}


void PlainFrac::reduct() {
    if(a == 0) {
        b = 1;
        return;
    }
    while(true) {
        int divNum = j::Gcd(a, b);
        if(divNum == 1 || divNum == 0)
            break;
        a /= divNum;
        b /= divNum;
    }
}


PlainFrac& PlainFrac::moveNegativeToTop() {
    if(b < 0) {
        b = -b;
        a = -a;
    }
    return *this;
}

QString PlainFrac::format(bool autoSpace) const {
    if(a == 0)
        return "0";

    return (b == 1 ? QString::number(a) : QString::number(a) + (autoSpace ? " / " : "/") + QString::number(b));
}

PlainFrac& PlainFrac::abs() {
    if(a < 0) a = -a;
    if(b < 0) b = -b;
    return *this;
}

PlainFrac& PlainFrac::sum(int digit) {
    if(digit != 0) {
        digit *= b;
        a += digit;
        reduct();
    }
    return *this;
}
PlainFrac& PlainFrac::sub(int digit) {
    if(digit != 0) {
        digit *= b;
        a += digit;
        reduct();
    }
    return *this;
}
PlainFrac& PlainFrac::mul(int digit) {
    if(digit == 0) {
        a = 0;
        b = 1;
    } else {
        a *= digit;
        reduct();
    }
    return *this;
}
PlainFrac& PlainFrac::div(int digit) {
    if(digit == 0)
        throw FracError("cannot div 0");
    if(a != 0) {
        b *= digit;
        reduct();
    }
    return *this;
}

PlainFrac& PlainFrac::sum(const PlainFrac &other) {
    if(other.a != 0) {
        int commonMulti = j::Lcm(b, other.b);
        int selfMulti = commonMulti / b;
        int otherMulti = commonMulti / other.b;
        b = commonMulti;

        a = a * selfMulti + other.a * otherMulti;
        reduct();
    }
    return *this;
}
PlainFrac& PlainFrac::sub(const PlainFrac &other) {
    if(other.a != 0) {
        int CommonMulti = j::Lcm(b, other.b);
        int selfMulti = CommonMulti / b;
        int otherMulti = CommonMulti / other.b;
        b = CommonMulti;

        a = a * selfMulti - other.a * otherMulti;
        reduct();
    }
    return *this;
}
PlainFrac& PlainFrac::mul(const PlainFrac &other) {
    if(other.a == 0) {
        a = 0;
        b = 1;
    } else {
        a *= other.a;
        b *= other.b;
        reduct();
    }
    return *this;
}
PlainFrac& PlainFrac::div(const PlainFrac &other) {
    if(other.a == 0)
        throw FracError("cannot div 0");
    a *= other.b;
    b *= other.a;
    reduct();
    return *this;
}

