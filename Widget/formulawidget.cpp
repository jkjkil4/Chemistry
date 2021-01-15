#include "formulawidget.h"


FormulaWidget::FormulaWidget(QWidget *parent)
    : QWidget(parent) {}

void FormulaWidget::paintEvent(QPaintEvent *) {
    if(lReactants.isEmpty() && lProducts.isEmpty())
        return;

    QRect rect;
    QPainter p(this);
    j::SetPointSize(&p, 18);

    int fmHeight = QFontMetrics(p.font()).height();
    int x = 20;
    int y = 20;

    j::DrawText(&p, x, y + fmHeight, Qt::AlignLeft | Qt::AlignBottom, "反应物:", -1, -1, &rect);
    x += rect.width() + 20;
    paint(&p, lReactants, x, y);
    //x += 20;
    //j::DrawText(p, x, y + fmHeight, Qt::AlignLeft | Qt::AlignBottom, "=", -1, -1, &rect);
    //x += rect.width() + 20;

    x = 20;
    j::DrawText(&p, x, y + 2 * fmHeight + 10, Qt::AlignLeft | Qt::AlignBottom, "生成物:", -1, -1, &rect);
    x += rect.width() + 20;
    paint(&p, lProducts, x, y + fmHeight + 10);
}

void FormulaWidget::clear() {
    lReactants.clear();
    lProducts.clear();
}

void FormulaWidget::paint(QPainter *p, const QList<Pair> &list, int &x, int y) {
    int fmHeight = QFontMetrics(p->font()).height();
    QRect rect;
    bool hasPrev = false;
    for(const Pair &pair : list) {
        if(hasPrev) {
            x += 20;
            j::DrawText(p, x, y + fmHeight, Qt::AlignLeft | Qt::AlignBottom, "+", -1, -1, &rect);
            x += rect.width() + 20;
        } else hasPrev = true;
        j::DrawText(p, x, y + fmHeight, Qt::AlignLeft | Qt::AlignBottom, pair.count.format(), -1, -1, &rect);
        x += rect.width() + 10;
        pair.formula.paint(p, x, y, Formula::PA_Top, &rect);
        x += rect.width();
    }
}
