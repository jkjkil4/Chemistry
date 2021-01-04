#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QGroupBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QListWidget>
#include <QMenuBar>
#include <QMessageBox>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QTextBlock>
#include <QApplication>

#include <mlibheader.h>

#include "Parser/formulagroup.h"
#include "Widget/formulawidget.h"

//#define DEBUG_FORMULAKEY

class Widget : public QWidget
{
    Q_OBJECT
public:
    struct UnkNum
    {
        UnkNum() = default;
        UnkNum(const QString &name) : name(name) {}
        QString name;
        Frac value;
    };

    struct Error
    {
        enum Type { Any, FormulaError, FormulaMultiDefined, IsEmpty, ElementNotExists } type;
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

    QPlainTextEdit *editRel = new QPlainTextEdit;

    QPushButton *btnAnalysis = new QPushButton("配平");
};

#endif // WIDGET_H
