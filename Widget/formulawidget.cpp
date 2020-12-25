#include "formulawidget.h"


FormulaWidget::FormulaWidget(const FormulaGroup &formula, QWidget *parent)
    : QWidget(parent), formula(formula) {}

void FormulaWidget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    j::SetPointSize(&p, 35);
    formula.paint(p, 10, 10, Formula::PA_Top);
}
