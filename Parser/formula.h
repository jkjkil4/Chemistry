#ifndef FORMULA_H
#define FORMULA_H

//#include <QVariant>
#include <QList>

#include "Class/frac.h"
#include "parsererror.h"

#include <QDebug>

class Formula
{
public:
    enum Type { Group, Element };
    Formula() = default;
    Formula(Frac count);
    virtual ~Formula() = default;

    Frac count = 1;
};

class Formula_Group : public Formula
{
public:
    Formula_Group(QString str, Frac count = 1, bool *ok = nullptr);
    ~Formula_Group() override;

    QList<Formula*> childs;
};

class Formula_Element : public Formula
{
public:
    Formula_Element(const QString &str, bool *ok = nullptr);
    ~Formula_Element() override = default;

    QString element;
    Frac elec;
};

#endif // FORMULA_H
