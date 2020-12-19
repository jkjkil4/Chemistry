#ifndef FORMULA_BASE_H
#define FORMULA_BASE_H

//#include <QVariant>
#include <QList>

#include "Class/frac.h"
//#include "parsererror.h"

#include <QPainter>

class Formula_Parent
{
public:
    enum Type { Group, Element };

    Formula_Parent() = default;
    Formula_Parent(int count);
    virtual ~Formula_Parent() = default;

    virtual void paint(QPainter &p, int &x, int y, bool useBrackets = false) const = 0;
    virtual QString format(bool useBrackets = false) const = 0;

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
    QString format(bool useBrackets = false) const override;

    QList<Formula_Parent*> childs;
};

class Formula_Element : public Formula_Parent
{
public:
    Formula_Element(const QString &str, bool *ok = nullptr);
    ~Formula_Element() override = default;

    void paint(QPainter &p, int &x, int y, bool = false) const override;
    QString format(bool = false) const override;

    QString element;
};

typedef Formula_Group Formula;

inline void DrawFormula(QPainter &p, int x, int y, Formula *formula, int *pWidth = nullptr) {
    int xStart = x;
    formula->paint(p, x, y);
    if(!formula->elec.mapPoly.isEmpty()) {
        x += 5;
        y -= QFontMetrics(p.font()).height();
        j::SetPointSize(&p, qMax(1, p.font().pointSize() / 2));
        QString elec = formula->elec.format(false, false);
        bool isOne = Frac(formula->elec).sum(1).mapPoly.isEmpty() || Frac(formula->elec).sub(1).mapPoly.isEmpty();
        if(elec[0] == '-') {
            elec.remove(0, 1);
            isOne ? elec = '-' : elec.append('-');
        } else isOne ? elec = '+' : elec.append('+');

        QRect rect;
        j::DrawText(p, x, y, Qt::AlignLeft | Qt::AlignTop, elec, -1, -1, &rect);
        x += rect.width();
    }
    SET_PTR(pWidth, x - xStart);
}

#endif // FORMULA_BASE_H
