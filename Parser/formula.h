#ifndef FORMULA_H
#define FORMULA_H

#include <QVariant>

#include "parsererror.h"

class Formula
{
public:
    enum Type { Group, Element };
    Formula(int count);
    virtual ~Formula() = default;

    int count;
};

class Formula_Group : public Formula
{
public:
    Formula_Group(QString str, int count = 1);
    ~Formula_Group() override;

    QList<Formula*> childs;
};

class Formula_Element : public Formula
{
public:
    Formula_Element(const QString &str, int count, int elec);

    int elec;
};

#endif // FORMULA_H
