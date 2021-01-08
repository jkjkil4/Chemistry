#ifndef FORMULAGROUP_H
#define FORMULAGROUP_H

#include "formula.h"

class FormulaGroup
{
public:
    class Iter
    {
    public:
        Iter(const FormulaGroup &formula);

        bool hasNext() const { return mHasNext; }
        Formula::Data next();

        int currentCount() { return mHasNext ? childIter.currentCount() : 0; }

    private:
        const FormulaGroup &formula;
        QList<Formula>::const_iterator listIter;
        Formula::Iter childIter;
        bool mHasNext = true;
    };

    FormulaGroup() = default;
    explicit FormulaGroup(const QString &str);
    ~FormulaGroup() = default;

    bool isVaild() const { return vaild; }

    //将化学式格式化为字符串
    QString format() const;
    //绘制化学式
    void paint(QPainter &p, int x, int y, Formula::PaintAlign pa, QRect *pRect = nullptr) const;
    //得到元素数量
    void elementCount(QMap<QString, Frac> &map, const Frac &mul = 1) const;

    const PlainFrac& elec() const { return mElec; }

    bool operator<(const FormulaGroup& other) const {
        if(str != other.str)
            return str < other.str;
        return mElec < other.mElec;
    }
    bool operator==(const FormulaGroup& other) const { return mElec == other.mElec && str == other.str; }

private:
    bool vaild = true;
    QString str;
    QList<Formula> lFormulas;
    PlainFrac mElec;
};

#endif // FORMULAGROUP_H
