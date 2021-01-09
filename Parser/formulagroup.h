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

        int currentCount() { return childIter ? childIter->currentCount() : 0; }

    private:
        const FormulaGroup &formula;
        QList<Formula>::const_iterator listIter = nullptr;
        Formula::Iter *childIter = nullptr;
        bool mHasNext = true;
    };

    FormulaGroup() = default;
    explicit FormulaGroup(const QString &str);
    ~FormulaGroup() = default;

    bool isVaild() const { return vaild; }

    //化学式的字符串
    QString str() const { return formatStr; }
    //绘制化学式
    void paint(QPainter &p, int x, int y, Formula::PaintAlign pa, QRect *pRect = nullptr) const;

    const PlainFrac& elec() const { return mElec; }

    bool operator<(const FormulaGroup& other) const {
        if(formatStr != other.formatStr)
            return formatStr < other.formatStr;
        return mElec < other.mElec;
    }
    bool operator==(const FormulaGroup& other) const { return mElec == other.mElec && formatStr == other.formatStr; }

private:
    bool vaild = true;
    QString formatStr;
    QList<Formula> lFormulas;
    PlainFrac mElec;
};

#endif // FORMULAGROUP_H
