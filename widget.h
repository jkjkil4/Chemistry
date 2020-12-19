#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QGroupBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QListWidget>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QTextBlock>
#include <QApplication>

#include <mlibheader.h>

#include "Parser/formula.h"

class Widget : public QWidget
{
    Q_OBJECT
public:
    struct FormulaKey
    {
        FormulaKey(const QString &key, const Frac &elec) : key(key), elec(elec) {}
        explicit FormulaKey(Formula *formula) : key(formula->format()), elec(formula->elec) {}
        QString key;
        Frac elec;
        friend inline bool operator<(const FormulaKey &a, const FormulaKey &b) {
            if(a.key != b.key)
                return a.key < b.key;
            Frac tmp(a.elec);
            tmp.sub(b.elec).moveNegativeToTop();
            return !tmp.mapPoly.isEmpty() && tmp.mapPoly[""] < 0;
        }
    };
    struct Error
    {
        enum Type { FormulaError } type;
        QStringList args;
        static QMap<Type, QString> mapText;

        Error(Type type, const QStringList &args) : type(type), args(args) {}
        QString text() {
            QString str = mapText[type];
            for(QString &arg : args)
                str = str.arg(arg);
            return str;
        }
    };

    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void onAnalysis();

private:
    QStackedWidget *stackedWidget = new QStackedWidget;
    QWidget *viewNone = new QWidget;
    QListWidget *viewError = new QListWidget;
    QWidget *viewResult = new QWidget;

    QPlainTextEdit *editFormula = new QPlainTextEdit;
    QPlainTextEdit *editRel = new QPlainTextEdit;

    QPushButton *btnAnalysis = new QPushButton("配平");
};

#endif // WIDGET_H
