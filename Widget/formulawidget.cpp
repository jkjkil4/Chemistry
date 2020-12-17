#include "formulawidget.h"

FormulaWidget::FormulaWidget(Formula *formula, QWidget *parent)
    : QWidget(parent), formula(formula) {}

FormulaWidget::~FormulaWidget() {
    delete formula;
}

void FormulaWidget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    j::SetPointSize(&p, 35);
    DrawFormula(p, 20, 70, formula);
}
