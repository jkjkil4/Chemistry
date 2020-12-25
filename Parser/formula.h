#ifndef FORMULA_BASE_H
#define FORMULA_BASE_H

#include <QVariant>
#include <QList>

#include "Class/frac.h"
//#include "parsererror.h"

#include <QPainter>
#include <QDebug>

class Formula
{
public:
    enum Type { Element, Group };

    explicit Formula(Type type, const QString &str, int count = 1);
    Formula(const Formula &other);
    ~Formula();

    VAR_GET_FUNC(Type, type, Type)
    VAR_GET_FUNC(Count, count, int)
    const PlainFrac& elec() const { return mElec; }
    bool isVaild() const { return vaild; }

    //化学式格式化为字符串
    QString format(bool useBrackets = false);

    //解析字符串(分离数字和数据)
    static bool ParseStr(Type type, const QString &str, QString &inner, int &count);

private:
    Type type;      //类型
    int count = 1;  //数量
    PlainFrac mElec;      //电荷总数

    void *data = nullptr;   //数据(type == Element -> QString, type == Group -> QList<Formula>)
    bool vaild = true;      //是否有效

    QString& rElementData();
    QList<Formula>& rGroupData();
};
/*
class Formula_Parent
{
public:
    enum Type { Group, Element };

    Formula_Parent() = default;
    Formula_Parent(int count);
    virtual ~Formula_Parent() = default;

    //绘制化学式，xy为左下角
    virtual void paint(QPainter &p, int &x, int y, bool useBrackets = false) const = 0;
    //格式化为字符串
    virtual QString format(bool useBrackets = false) const = 0;
    //得到元素数量，不会自动清空mapElementCount
    virtual void elementCount(QMap<QString, Frac> &mapElementCount, int mulCount = 1) const = 0;

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
    void elementCount(QMap<QString, Frac> &mapElementCount, int mulCount = 1) const override;

    QList<Formula_Parent*> childs;
};

class Formula_Element : public Formula_Parent
{
public:
    Formula_Element(const QString &str, bool *ok = nullptr);
    ~Formula_Element() override = default;

    void paint(QPainter &p, int &x, int y, bool = false) const override;
    QString format(bool = false) const override;
    void elementCount(QMap<QString, Frac> &mapElementCount, int mulCount = 1) const override;

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
}*/

#endif // FORMULA_BASE_H
