#include "formulawidget.h"

FormulaWidget::FormulaWidget(Formula *formula, QWidget *parent)
    : QWidget(parent), formula(formula) {}

FormulaWidget::~FormulaWidget() {
    delete formula;
}

void FormulaWidget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    j::SetPointSize(&p, 35);
    int x = 40;
    int y = 100;
    formula->paint(p, x, y);
    if(!formula->elec.mapPoly.isEmpty()) {
        x += 10;
        y -= QFontMetrics(p.font()).height();
        j::SetPointSize(&p, qMax(1, p.font().pointSize() / 2));
        QString elec = formula->elec.format(false, false);
        bool isOne = Frac(formula->elec).sum(1).mapPoly.isEmpty() || Frac(formula->elec).sub(1).mapPoly.isEmpty();
        if(elec[0] == '-') {
            elec.remove(0, 1);
            isOne ? elec = '-' : elec.append('-');
        } else isOne ? elec = '+' : elec.append('+');
        j::DrawText(p, x, y, Qt::AlignLeft | Qt::AlignTop, elec);
    }
}
