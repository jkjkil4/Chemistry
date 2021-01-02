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

#include "Parser/formulagroup.h"
#include "Widget/formulawidget.h"

//#define DEBUG_FORMULAKEY

#ifdef DEBUG_FORMULAKEY
#include <QDebug>
#endif

class Widget : public QWidget
{
    Q_OBJECT
public:
    struct FormulaKey
    {
        explicit FormulaKey(const QString &str, bool *ok = nullptr);
        explicit FormulaKey(const QString &key, const PlainFrac &elec) : key(key), elec(elec) {}
        explicit FormulaKey(const FormulaGroup &formula) : key(formula.format()), elec(formula.elec()) {}

        QString key;
        PlainFrac elec;

        inline bool operator<(const FormulaKey &other) const {
            if(key != other.key)
                return key < other.key;
            return elec < other.elec;
        }
        inline bool operator==(const FormulaKey &other) const { return key == other.key && elec == other.elec; }
#ifdef DEBUG_FORMULAKEY
        friend inline QDebug& operator<<(QDebug& de, const FormulaKey& fk) {
            return de << "FormulaKey(" + fk.key + ", " + fk.elec.format() + ")";
        }
#endif
    };

    struct UnkNum
    {
        UnkNum() = default;
        UnkNum(const QString &name) : name(name) {}
        QString name;
        Frac value;
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
    FormulaWidget *viewResult = new FormulaWidget;

    QPlainTextEdit *editFormula = new QPlainTextEdit;
    QPlainTextEdit *editRel = new QPlainTextEdit;

    QPushButton *btnAnalysis = new QPushButton("配平");
};

#endif // WIDGET_H
