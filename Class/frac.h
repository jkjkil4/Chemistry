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

    void checkZero() {
        QStringList lNeedRemoveMono;
        for(auto iter = mapPoly.begin(); iter != mapPoly.end(); ++iter)
            if(iter.value() == 0)
                lNeedRemoveMono << iter.key();
        for(QString& mono : lNeedRemoveMono)
            mapPoly.remove(mono);

        if(mapPoly.isEmpty())
            b = 1;
    }

    void reduct() {

    }

    QString format(bool autoSpace = true, bool useColor = true) {
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
                if(hasPrev) {
                    res += autoSpace ? " + " : "+";
                } else hasPrev = true;
            QString key = iter.key();
            int value = iter.value();
            if(value != 1 || key.isEmpty())
                res += QString::number(value);
            if(!key.isEmpty())
                res += (useColor ? "\033[33m[\033[0m" : "[") + key + (useColor ? "\033[33m]\033[0m" : "]");
        }
        if(size != 1)
            res += ")";
        res += autoSpace ? " / " : "/";
        res += QString::number(b);

        return res;
    }

    Frac& sum(int digit, const QString &key = "") {
        if(digit != 0) {
            digit *= b;
            mapPoly[key] += digit;
        }
        return *this;
    }
    Frac& sub(int digit, const QString &key = "") {
        if(digit != 0) {
            digit *= b;
            mapPoly[key] -= digit;
        }
        return *this;
    }
    Frac& multi(int digit) {
        if(digit == 0) {
            mapPoly.clear();
            b = 1;
        } else {
            for(int &mono : mapPoly)
                mono *= digit;
        }
        return *this;
    }
    Frac& div(int digit) {
        if(digit == 0)
            throw Error("cannot div 0");
        b *= digit;
        return *this;
    }

    Frac& sum(const Frac &other) {
        for(QMap<QString, int>::const_iterator iter = other.mapPoly.begin(); iter != other.mapPoly.end(); ++iter)
            mapPoly[iter.key()] += iter.value();
        return *this;
    }
    Frac& sub(const Frac &other) {
        for(QMap<QString, int>::const_iterator iter = other.mapPoly.begin(); iter != other.mapPoly.end(); ++iter)
            mapPoly[iter.key()] -= iter.value();
        return *this;
    }

    friend inline Frac operator+(Frac frac, int digit) {
        frac.sum(digit);
        return frac;
    }
};


#endif // FRAC_H
