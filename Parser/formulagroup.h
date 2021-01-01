#ifndef FORMULAGROUP_H
#define FORMULAGROUP_H

#include "formula.h"

class FormulaGroup
{
public:
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

private:
    bool vaild = true;
    QList<Formula> lFormulas;
    PlainFrac mElec;
};

#endif // FORMULAGROUP_H
