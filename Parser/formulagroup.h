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
    QString format();

    const Frac& getElec() const { return elec; }

private:
    bool vaild = true;
    QList<Formula> lFormulas;
    Frac elec;
};

#endif // FORMULAGROUP_H
