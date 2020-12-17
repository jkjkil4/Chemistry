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
    Formula_Parent(int count);
    virtual ~Formula_Parent() = default;

    virtual void paint(QPainter &p, int &x, int y, bool useBrackets = false) const = 0;

    int count = 1;
    Frac elec;
};

class Formula_Group : public Formula_Parent
{
public:
    Formula_Group(QString str, int count = 1, bool *ok = nullptr);
    Formula_Group(const Formula_Group &other) = delete;
    ~Formula_Group() override;

    void paint(QPainter &p, int &x, int y, bool useBrackets = false) const override;

    QList<Formula_Parent*> childs;
};

class Formula_Element : public Formula_Parent
{
public:
    Formula_Element(const QString &str, bool *ok = nullptr);
    ~Formula_Element() override = default;

    void paint(QPainter &p, int &x, int y, bool) const override;

    QString formatInfo() const;

    QString element;
};

typedef Formula_Group Formula;

#endif // FORMULA_BASE_H
