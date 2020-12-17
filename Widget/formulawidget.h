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
    explicit FormulaWidget(Formula *formula, QWidget *parent = nullptr);
    ~FormulaWidget() override;

    VAR_GET_FUNC(Formula, formula, Formula*)

private:
    Formula *formula;
};

#endif // FORMULAWIDGET_H
