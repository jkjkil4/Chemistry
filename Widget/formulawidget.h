#ifndef FORMULAWIDGET_H
#define FORMULAWIDGET_H

#include <QWidget>

#include "Parser/formulagroup.h"

class FormulaWidget : public QWidget
{
    Q_OBJECT
protected:
    void paintEvent(QPaintEvent *) override;

public:
    explicit FormulaWidget(const FormulaGroup &formula, QWidget *parent = nullptr);
    ~FormulaWidget() override = default;

private:
    FormulaGroup formula;
};

#endif // FORMULAWIDGET_H
