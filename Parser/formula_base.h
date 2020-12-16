#ifndef FORMULA_BASE_H
#define FORMULA_BASE_H

//#include <QVariant>
#include <QList>

#include "Class/frac.h"
#include "parsererror.h"

#include <QPainter>

class Formula_Parent
{
public:
    enum Type { Group, Element };
    Formula_Parent() = default;
    Formula_Parent(Frac count);
    virtual ~Formula_Parent() = default;

    virtual void paint(QPainter &p, int &xOffset) = 0;

    Frac count = 1;
};

class Formula_Group : public Formula_Parent
{
public:
    Formula_Group(QString str, Frac count = 1, bool *ok = nullptr);
    ~Formula_Group() override;

    void paint(QPainter &p, int &xOffset) override;

    QList<Formula_Parent*> childs;
};

class Formula_Element : public Formula_Parent
{
public:
    Formula_Element(const QString &str, bool *ok = nullptr);
    ~Formula_Element() override = default;

    void paint(QPainter &p, int &xOffset) override;

    QString formatInfo();

    QString element;
    Frac elec;
};

#endif // FORMULA_BASE_H
