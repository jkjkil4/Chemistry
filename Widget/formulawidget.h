#ifndef FORMULAWIDGET_H
#define FORMULAWIDGET_H

#include <QWidget>

#include <Lib/paint.h>

#include "Parser/formulagroup.h"

class FormulaWidget : public QWidget
{
    Q_OBJECT
protected:
    void paintEvent(QPaintEvent *) override;

public:
    struct Pair
    {
        Frac count;
        FormulaGroup formula;
    };

    explicit FormulaWidget(QWidget *parent = nullptr);
    ~FormulaWidget() override = default;

    void addReactant(const Frac &frac, const FormulaGroup &formula) { add(lReactants, frac, formula); }
    void addProduct(const Frac &frac, const FormulaGroup &formula) { add(lProducts, frac, formula); }

    void clear();

private:
    void add(QList<Pair> &list, const Frac &frac, const FormulaGroup &formula) { list << Pair{ frac, formula }; }
    void paint(QPainter &p, const QList<Pair> &list, int &x, int y);
    QList<Pair> lReactants, lProducts;
};

#endif // FORMULAWIDGET_H
