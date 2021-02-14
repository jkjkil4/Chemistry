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

    struct Elec
    {
        PlainFrac value;
        QSet<QString> keys;
    };

    struct Data
    {
    public:
        Data() {}
        Data(const Formula &formula) : formula(&formula) {}

        bool isVaild() const { return formula; }

        int count() const { return formula->mCount; }
        const QString& name() const { return formula->elementData(); }

//        bool hasGL() const { return formula->pElec; }
//        PlainFrac glValue() const { return formula->pElec->value; }
//        const QSet<QString>& glKeys() const { return formula->pElec->keys; }

//        inline bool operator<(const Data &other) const { return formula < other.formula; }

    private:
        const Formula *formula = nullptr;
    };

    class Iter
    {
    public:
        Iter(const Formula &formula);
        Iter(const Iter &other);
        ~Iter();

        bool hasNext() const { return mHasNext; }
        Data next();

        int currentCount() const { return childIter ? childIter->currentCount() * formula.mCount : formula.mCount; }

    private:
        const Formula &formula;
        QList<Formula>::const_iterator listIter = nullptr;
        Iter *childIter = nullptr;
        bool mHasNext = true;
    };

    explicit Formula(Type mType, const QString &str, int mCount = 1);
    Formula(const Formula &other);
    ~Formula();

    VAR_GET_FUNC(type, mType, Type)
    VAR_GET_FUNC(count, mCount, int)
    bool isVaild() const { return mVaild; }

    //化学式格式化为字符串
    QString format(bool useBrackets = false) const;
    //绘制化学式
    void paint(QPainter *p, int &x, int y, PaintAlign pa, bool useBrackets = false) const;

    //解析字符串(分离数字和数据)
    static bool ParseStr(Type mType, const QString &str, QString &inner, int &mCount);

    const QString& elementData() const;
    const QList<Formula>& groupData() const;

private:
    Type mType;      //类型
    int mCount = 1;  //数量

    void *mData = nullptr;   //数据(type == Element -> QString, type == Group -> QList<Formula>)
    bool mVaild = true;      //是否有效

//    //用于得失电子守恒的数据
//    Elec *pElec = nullptr;
};

#endif // FORMULA_H
