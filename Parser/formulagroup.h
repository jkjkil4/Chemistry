#ifndef FORMULAGROUP_H
#define FORMULAGROUP_H

#include "formula.h"

class FormulaGroup
{
public:
    explicit FormulaGroup(const QString &str);
    ~FormulaGroup() = default;

    bool isVaild() const { return vaild; }

    //将化学式格式化为字符串
    QString format() const;
    //绘制化学式
    void paint(QPainter &p, int x, int y, Formula::PaintAlign pa);

    const PlainFrac& elec() const { return mElec; }

private:
    bool vaild = true;
    QList<Formula> lFormulas;
    PlainFrac mElec;
};

#endif // FORMULAGROUP_H
