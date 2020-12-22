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

#include <QDebug>

class Widget : public QWidget
{
    Q_OBJECT
public:
    struct FormulaKey
    {
        explicit FormulaKey(const QString &str, bool *ok = nullptr) {
            int indexOfLeft = str.indexOf('{');
            int indexOfRight = str.indexOf('}');
            if(indexOfLeft == -1 || indexOfRight == -1 || indexOfLeft >= indexOfRight) {
                SET_PTR(ok, false);
                return;
            }
            key = str.left(indexOfLeft);
            if(key.isEmpty()) {
                SET_PTR(ok, false);
                return;
            }
            bool ok2;
            elec = Frac(str.mid(indexOfLeft + 1, indexOfRight - indexOfLeft - 1), &ok2);
            if(!ok2) {
                SET_PTR(ok, false);
                return;
            }
            SET_PTR(ok, true);
        }
        explicit FormulaKey(const QString &key, const Frac &elec) : key(key), elec(elec) {}
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
        friend inline bool operator==(FormulaKey &a, const FormulaKey &b) {
            Frac tmp(a.elec);
            tmp.sub(b.elec);
            return a.key == b.key && tmp.mapPoly.isEmpty();
        }
    };
    struct Error
    {
        enum Type { Any, FormulaError, FormulaNotExists, FormulaMultiDefined, IsEmpty, ElementNotExists } type;
        QStringList args;
        static QMap<Type, QString> mapText;

        Error(Type type, const QStringList &args) : type(type), args(args) {}
        QString text() const {
            QString str = mapText[type];
            for(const QString &arg : args)
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
