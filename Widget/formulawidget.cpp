#include "formulawidget.h"

FormulaWidget::FormulaWidget(const QList<FormulaStruct> &lReactants, const QList<FormulaStruct> &lProducts, QWidget *parent)
    : QWidget(parent), lReactants(lReactants), lProducts(lProducts) {}

FormulaWidget::~FormulaWidget() {
    for(FormulaStruct &fs : lReactants)
        delete fs.formula;
    for(FormulaStruct &fs : lProducts)
        delete fs.formula;
}

void FormulaWidget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    j::SetPointSize(&p, 35);
    DrawFormula(p, 20, 70, formula);
}
