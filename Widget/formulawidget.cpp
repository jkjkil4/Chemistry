#include "formulawidget.h"


FormulaWidget::FormulaWidget(QWidget *parent)
    : QWidget(parent) {}

void FormulaWidget::paintEvent(QPaintEvent *) {
    if(lReactants.isEmpty() && lProducts.isEmpty())
        return;

    QPainter p(this);
    j::SetPointSize(&p, 20);

    int height = QFontMetrics(p.font()).height();
    int x = 50;
    int y = 50;
    paint(p, lReactants, x, y);
    x += 20;
    QRect rect;
    j::DrawText(p, x, y + height, Qt::AlignLeft | Qt::AlignBottom, "=", -1, -1, &rect);
    x += rect.width() + 20;
    paint(p, lProducts, x, y);
}

void FormulaWidget::clear() {
    lReactants.clear();
    lProducts.clear();
}

void FormulaWidget::paint(QPainter &p, const QList<Pair> &list, int &x, int y) {
    int fmHeight = QFontMetrics(p.font()).height();
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
