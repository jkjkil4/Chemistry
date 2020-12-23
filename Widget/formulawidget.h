#ifndef FORMULAWIDGET_H
#define FORMULAWIDGET_H

#include <QWidget>

#include "Parser/formula.h"

class FormulaWidget : public QWidget
{
    Q_OBJECT
protected:
    void paintEvent(QPaintEvent *) override;

public:
    struct FormulaStruct
    {
        FormulaStruct(Formula *formula, const Frac &count) : formula(formula), count(count) {}
        Formula *formula;
        Frac count;
    };

    explicit FormulaWidget(const QList<FormulaStruct> &lReactants, const QList<FormulaStruct> &lProducts, QWidget *parent = nullptr);
    ~FormulaWidget() override;

private:
    QList<FormulaStruct> lReactants, lProducts;
};

#endif // FORMULAWIDGET_H
