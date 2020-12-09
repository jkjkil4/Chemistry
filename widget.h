#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QTabWidget>
#include <QPushButton>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <header.h>

class Widget : public QWidget
{
    Q_OBJECT
public:
    class Formula
    {
    public:
        Formula(Formula *parent = nullptr) : parent(parent) {}
        ~Formula() {
            for(Formula *child : childs) {
                delete child;
            }
        }

        Formula *parent = nullptr;
        QList<Formula*> childs;
    };

    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void onAnalysis();

private:
    QTabWidget *tabWidget = new QTabWidget;

    QPlainTextEdit *editFormula = new QPlainTextEdit;
    QPlainTextEdit *editRel = new QPlainTextEdit;

    QPushButton *btnAnalysis = new QPushButton;
};

#endif // WIDGET_H
