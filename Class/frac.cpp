#include "frac.h"

void Frac::checkZero() {
    QStringList lNeedRemoveMono;
    for(auto iter = mapPoly.begin(); iter != mapPoly.end(); ++iter)
        if(iter.value() == 0)
            lNeedRemoveMono << iter.key();
    for(QString& mono : lNeedRemoveMono)
        mapPoly.remove(mono);

    if(mapPoly.isEmpty())
        b = 1;
}

int Frac::Gcd(int a, int b) {
    int absA = qMax(qAbs(a), qAbs(b));
    int absB = qMin(qAbs(a), qAbs(b));
    return absB == 0 ? absA : Gcd(b, a % b);
}

int Frac::Gcd(const QVector<int> &vValues, int n) {
    if(n == -1)
        n = vValues.size();
    return (n == 1 ? vValues[0] : Gcd(vValues[n - 1], Gcd(vValues, n - 1)));
}

int Frac::Lcm(int a, int b) {
    if(a == 0 || b == 0)
        return 0;
    return qAbs(a / Gcd(a, b) * b);
}

int Frac::Lcm(const QVector<int> &vValues, int n) {
    if(n == -1)
        n = vValues.size();
    if(vValues[n - 1] == 0)
        return 0;
    return (n == 1 ? vValues[0] : Lcm(vValues[n - 1], Lcm(vValues, n - 1)));
}

void Frac::reduct() {
    checkZero();
    while(true) {
        QVector<int> vValues { b };
        for(int value : mapPoly)
            vValues << value;
        int divNum = Gcd(vValues);
        if(divNum == 1 || divNum == 0)
            break;
        for(int &value : mapPoly)
            value /= divNum;
        b /= divNum;
    }
}

QString Frac::format(bool autoSpace, bool useColor) {
    QString res;

    int size = mapPoly.size();
    if(size == 0) {
        res = "0";
        return res;
    }

    if(size != 1)
        res += "(";

    bool hasPrev = false;
    for(auto iter = mapPoly.begin(); iter != mapPoly.end(); ++iter) {
        QString key = iter.key();
        int value = iter.value();

        if(value == 0)
            continue;
        if(hasPrev) {
            res += value < 0 ? (autoSpace ? " - " : "-") : (autoSpace ? " + " : "+");
        } else hasPrev = true;

        if(qAbs(value) != 1 || key.isEmpty())
            res += QString::number(qAbs(value));
        if(!key.isEmpty())
            res += (useColor ? "\033[33m[\033[0m" : "[") + key + (useColor ? "\033[33m]\033[0m" : "]");
    }
    if(size != 1)
        res += ")";
    res += autoSpace ? " / " : "/";
    res += QString::number(b);

    return res;
}

Frac& Frac::sum(int digit, const QString &key) {
    if(digit != 0) {
        digit *= b;
        mapPoly[key] += digit;
    }
    return *this;
}
Frac& Frac::sub(int digit, const QString &key) {
    if(digit != 0) {
        digit *= b;
        mapPoly[key] -= digit;
    }
    return *this;
}
Frac& Frac::multi(int digit) {
    if(digit == 0) {
        mapPoly.clear();
        b = 1;
    } else {
        for(int &mono : mapPoly)
            mono *= digit;
    }
    return *this;
}
Frac& Frac::div(int digit) {
    if(digit == 0)
        throw Error("cannot div 0");
    b *= digit;
    return *this;
}

//Frac& Frac::sum(const Frac &other) {
//    for(QMap<QString, int>::const_iterator iter = other.mapPoly.begin(); iter != other.mapPoly.end(); ++iter)
//        mapPoly[iter.key()] += iter.value();
//    return *this;
//}
//Frac& Frac::sub(const Frac &other) {
//    for(QMap<QString, int>::const_iterator iter = other.mapPoly.begin(); iter != other.mapPoly.end(); ++iter)
//        mapPoly[iter.key()] -= iter.value();
//    return *this;
//}
