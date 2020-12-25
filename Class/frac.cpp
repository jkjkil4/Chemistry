#include "frac.h"

Frac::Frac(int value, const QString &key) {
    if(value != 0)
        mapPoly[key] = value;
}

Frac::Frac(int a, int b) {
    if(b == 0)
        throw FracError("cannot div 0");
    if(a != 0)
        mapPoly[""] = a;
    this->b = b;
    reduct();
}

Frac::Frac(QString str, bool *ok) {
    str.replace(' ', "");
    int indexOfDiv = str.indexOf("/");
    if(indexOfDiv == -1) {
        bool ok2;
        int value = str.toInt(&ok2);
        if(ok2) {
            mapPoly[""] = value;
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
        mapPoly[""] = top;
        b = bottom;
    }

    reduct();
    SET_PTR(ok, true);
}

Frac::Frac(const PlainFrac &plain) {
    if(plain.a == 0)
        return;
    mapPoly[""] = plain.a;
    b = plain.b;
}

PlainFrac Frac::toPlain() {
    int a = mapPoly.value("", 0);
    return PlainFrac(a, b);
}

//void Frac::checkZero() {
//    QStringList lNeedRemoveMono;
//    for(auto iter = mapPoly.begin(); iter != mapPoly.end(); ++iter)
//        if(iter.value() == 0)
//            lNeedRemoveMono << iter.key();
//    for(QString& mono : lNeedRemoveMono)
//        mapPoly.remove(mono);

//    if(mapPoly.isEmpty())
//        b = 1;
//}


//#define DEBUG_FRAC_SOLVINGRQUATIONS
#ifdef DEBUG_FRAC_SOLVINGRQUATIONS
#include <QDebug>
#endif

QList<Frac> Frac::SolvingEquations(QList<Frac> lFracs, const QStringList &lUnkNumbers, bool *ok) {
    //对于可以直接判断无解的情况，就结束该函数
    if(lUnkNumbers.isEmpty() || lFracs.size() < lUnkNumbers.size()) {
        SET_PTR(ok, false);
        return QList<Frac>();
    }

#ifdef DEBUG_FRAC_SOLVINGRQUATIONS
    qDebug().noquote() << "\033[35m初始:\033[0m";
    for(Frac &frac : lFracs)
        qDebug().noquote() << frac.format() << "= 0";
#endif

    QList<Frac> lRes;
    for(const QString &unkNumber : lUnkNumbers) {
        auto iter = lFracs.end();
        //查找第一个该未知数出现的式子
        for(auto listIter = lFracs.begin(); listIter != lFracs.end(); ++listIter) {
            if((*listIter).mapPoly.contains(unkNumber)) {
                iter = listIter;
                break;
            }
        }
        //如果无法从现有的式子中找到该未知数，就结束该函数
        if(iter == lFracs.end()) {
            SET_PTR(ok, false);
            return lRes;
        }
        Frac paramFrac = (*iter).paramSep(unkNumber);   //参变分离(我不确定是不是这么说)
        lRes << paramFrac;      //将其加入到结果list中
        lFracs.erase(iter);     //从lFracs中移除该iter

#ifdef DEBUG_FRAC_SOLVINGRQUATIONS
        qDebug().noquote() << "\n\033[35m将\033[0m " + unkNumber + " = " + paramFrac.format() + " \033[35m代入得:\033[0m";
#endif

        for(Frac &otherFrac : lFracs) {   //将参变分离的结果代入到其他式子中
#ifdef DEBUG_FRAC_SOLVINGRQUATIONS
            qDebug().noquote() << otherFrac.substitute(unkNumber, paramFrac).format() << "= 0";
#else
            otherFrac.substitute(unkNumber, paramFrac);
#endif
        }
    }

#ifdef DEBUG_FRAC_SOLVINGRQUATIONS
    qDebug() << "\n\033[35m多余项:\033[0m";
    for(Frac &frac : lFracs) {
        qDebug().noquote() << frac.format() << "= 0";
    }
#endif

    for(Frac &frac : lFracs) {  //判断是否无解(若有任何一个多余项不为0，就无解)
        if(!frac.mapPoly.isEmpty()) {
            SET_PTR(ok, false);
            return QList<Frac>();
        }
    }

    {//往回代入，求出所有的未知数
        auto iterUnkNum = lUnkNumbers.rbegin();
        for(auto iterGet = lRes.rbegin(); iterGet != lRes.rend(); ++iterGet) {
            Frac &cur = *iterGet;
            for(auto iterSet = iterGet + 1; iterSet != lRes.rend(); ++iterSet)
                (*iterSet).substitute(*iterUnkNum, cur);
            ++iterUnkNum;
        }
    }

    for(Frac &frac : lRes)  //将分母的负号移至分子
        frac.moveNegativeToTop();

#ifdef DEBUG_FRAC_SOLVINGRQUATIONS
    qDebug().noquote() << "\n\033[35m结果:\033[0m";
    {
        auto iterUnkNum = lUnkNumbers.begin();
        for(auto iter = lRes.begin(); iter != lRes.end(); ++iter) {
            qDebug().noquote() << *iterUnkNum << "=" << (*iter).format();
            ++iterUnkNum;
        }
    }
#endif

    SET_PTR(ok, true);
    return lRes;
}

void Frac::reduct() {
//    checkZero();
    if(mapPoly.isEmpty()) {
        b = 1;
        return;
    }
    while(true) {
        QVector<int> vValues { b };
        for(int value : mapPoly)
            vValues << value;
        int divNum = j::Gcd(vValues);
        if(divNum == 1 || divNum == 0)
            break;
        for(int &value : mapPoly)
            value /= divNum;
        b /= divNum;
    }
}

Frac& Frac::changeMono(const QString &before, const QString &now) {
    if(before != now) {
        auto iter = mapPoly.find(before);
        if(iter != mapPoly.end()) {
            mapPoly[now] += *iter;
            mapPoly.erase(iter);
        }
    }
    return *this;
}

Frac Frac::paramSep(const QString &param, bool *ok) {
//    checkZero();
    if(!mapPoly.contains(param)) {
        SET_PTR(ok, false);
        return Frac();
    }

    Frac res = *this;
    res.b = 1;
    res.mapPoly.remove(param);
    res.div(-mapPoly[param]);
    res.reduct();

    SET_PTR(ok, true);
    return res;
}

Frac& Frac::substitute(const QString &param, int digit) {
    auto iter = mapPoly.find(param);
    if(iter != mapPoly.end()) {
        int value = iter.value();
        if((mapPoly[""] += value * digit) == 0)
            mapPoly.remove("");
        mapPoly.erase(iter);
        reduct();
    }
    return *this;
}

Frac& Frac::substitute(const QString &param, const Frac &other) {
    auto iter = mapPoly.find(param);
    if(iter != mapPoly.end()) {
        int valueMul = iter.value();
        mapPoly.erase(iter);
        b *= other.b;
        for(int &mono : mapPoly)
            mono *= other.b;
        for(auto otherIter = other.mapPoly.begin(); otherIter != other.mapPoly.end(); ++otherIter)
            if((mapPoly[otherIter.key()] += otherIter.value() * valueMul) == 0)
                mapPoly.remove(otherIter.key());
        reduct();
    }
    return *this;
}

Frac& Frac::moveNegativeToTop() {
    if(b < 0) {
        b = -b;
        for(int &mono : mapPoly)
            mono = -mono;
    }
    return *this;
}

QString Frac::format(bool autoSpace, bool useColor) const {
    QString res;

    int size = mapPoly.size();
    if(size == 0) {
        res = "0";
        return res;
    }

    if(b != 1 && size != 1)
        res += "(";

    bool hasPrev = false;
    for(auto iter = mapPoly.begin(); iter != mapPoly.end(); ++iter) {
        const QString& key = iter.key();
        int value = iter.value();

        if(hasPrev) {
            res += value < 0 ? (autoSpace ? " - " : "-") : (autoSpace ? " + " : "+");
        } else {
            if(value < 0) res += '-';
            hasPrev = true;
        }

        if(qAbs(value) != 1 || key.isEmpty())
            res += QString::number(qAbs(value));
        if(!key.isEmpty())
            res += (useColor ? "\033[33m[\033[0m" : "[") + key + (useColor ? "\033[33m]\033[0m" : "]");
    }
    if(b != 1) {
        if(size != 1)
            res += ")";
        res += autoSpace ? " / " : "/";
        res += QString::number(b);
    }

    return res;
}

Frac& Frac::sum(int digit, const QString &key) {
    if(digit != 0) {
        digit *= b;
        if((mapPoly[key] += digit ) == 0)
            mapPoly.remove(key);
        reduct();
    }
    return *this;
}
Frac& Frac::sub(int digit, const QString &key) {
    if(digit != 0) {
        digit *= b;
        if((mapPoly[key] -= digit) == 0)
            mapPoly.remove(key);
        reduct();
    }
    return *this;
}
Frac& Frac::mul(int digit) {
    if(digit == 0) {
        mapPoly.clear();
        b = 1;
    } else {
        for(int &mono : mapPoly)
            mono *= digit;
        reduct();
    }
    return *this;
}
Frac& Frac::div(int digit) {
    if(digit == 0)
        throw FracError("cannot div 0");
    if(!mapPoly.isEmpty()) {
        b *= digit;
        reduct();
    }
    return *this;
}

Frac& Frac::sum(const Frac &other) {
    if(!other.mapPoly.isEmpty()) {
        int commonMulti = j::Lcm(b, other.b);
        int selfMulti = commonMulti / b;
        int otherMulti = commonMulti / other.b;
        b = commonMulti;

        for(int &mono : mapPoly)
            mono *= selfMulti;
        for(QMap<QString, int>::const_iterator iter = other.mapPoly.begin(); iter != other.mapPoly.end(); ++iter)
            if((mapPoly[iter.key()] += iter.value() * otherMulti) == 0)
                mapPoly.remove(iter.key());
        reduct();
    }
    return *this;
}
Frac& Frac::sub(const Frac &other) {
    if(!other.mapPoly.isEmpty()) {
        int CommonMulti = j::Lcm(b, other.b);
        int selfMulti = CommonMulti / b;
        int otherMulti = CommonMulti / other.b;
        b = CommonMulti;

        for(int &mono : mapPoly)
            mono *= selfMulti;
        for(QMap<QString, int>::const_iterator iter = other.mapPoly.begin(); iter != other.mapPoly.end(); ++iter)
            if((mapPoly[iter.key()] -= iter.value() * otherMulti) == 0)
                mapPoly.remove(iter.key());
        reduct();
    }
    return *this;
}
