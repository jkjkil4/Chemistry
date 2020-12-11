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

QVector<int> Frac::DigitFactor(int value) {
    QVector<int> vFactors;
    int absValue = abs(value);
    if(absValue == 1 || absValue == 0)
        return vFactors;

    for(int i = 2; i <= absValue; i++)
        if(absValue % i == 0)
            vFactors << i;
    return vFactors;
}
#include <QDebug>
QVector<int> Frac::findCommonFactor() {
    QVector<int> vCommonFactors;

    checkZero();
    if(mapPoly.isEmpty() || b == 1)    //对可以直接返回无公因数的情况进行判断
        return vCommonFactors;

    //得到分母的所有因数
    vCommonFactors = DigitFactor(b);

    //遍历分子的每个项，对公因数进行判断
    for(int value : mapPoly) {
        QVector<int> tmpCommonFactor = vCommonFactors;
        vCommonFactors.clear();
        QVector<int> vFactors = DigitFactor(value);
        for(int factor : vFactors)
            if(tmpCommonFactor.contains(factor))
                vCommonFactors << factor;
        if(vCommonFactors.isEmpty())
            break;
    }

    return vCommonFactors;
}

void Frac::reduct() {
    while(true) {
        QVector<int> vCommonFactors = findCommonFactor();
        if(vCommonFactors.isEmpty())
            break;
        int divNum = *vCommonFactors.rbegin();
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
