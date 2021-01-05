#ifndef FORMULA_H
#define FORMULA_H

#include <QVariant>
#include <QList>

#include "Class/frac.h"
//#include "parsererror.h"

#include <QPainter>
//#include <QDebug>

#include <Lib/paint.h>

class Formula
{
public:
    enum Type { Element, Group };
    enum PaintAlign { PA_Top, PA_Bottom };

    explicit Formula(Type type, const QString &str, int count = 1);
    Formula(const Formula &other);
    ~Formula();

    VAR_GET_FUNC(Type, type, Type)
    VAR_GET_FUNC(Count, count, int)
    bool isVaild() const { return vaild; }

    //化学式格式化为字符串
    QString format(bool useBrackets = false) const;
    //绘制化学式
    void paint(QPainter &p, int &x, int y, PaintAlign pa, bool useBrackets = false) const;
    //得到元素数量
    void elementCount(QMap<QString, Frac> &map, const Frac &mul = 1) const;

    //解析字符串(分离数字和数据)
    static bool ParseStr(Type type, const QString &str, QString &inner, int &count);

private:
    Type type;      //类型
    int count = 1;  //数量

    void *data = nullptr;   //数据(type == Element -> QString, type == Group -> QList<Formula>)
    bool vaild = true;      //是否有效

    const QString& elementData() const;
    const QList<Formula>& groupData() const;
};

#endif // FORMULA_H
