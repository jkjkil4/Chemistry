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
#include <QTextStream>

#include "Parser/formulagroup.h"
#include "Widget/formulawidget.h"
#include "Parser/highlighter.h"

#include "global.h"

//#define DEBUG_FORMULAKEY

class Widget : public QWidget
{
    Q_OBJECT
public:
    struct Error
    {
        enum Type { Any, FormulaError, FormulaMultiDefined, GLMultiDefined, IsEmpty, ElementNotExists, GLNotExists } type;
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
    struct UnkNum
    {
        UnkNum() = default;
        UnkNum(const QString &name) : name(name) {}
        QString name;
        Frac value;
    };
    struct Part
    {
        QString name;   //该部分的名称
        QMap<QString, Frac> mapElemCount;   //各原子数
        Frac elec;  //总电荷数
    };
    struct GLKey
    {
        QString strElem, strRel;
        inline bool operator<(const GLKey &other) const {
            if(strElem != other.strElem)
                return strElem < other.strElem;
            return strRel < other.strRel;
        }
    };
    struct GL
    {
        bool isVaild = false;
        int count;
        QString strUnkNum;
        Formula::Data data;
    };
    struct GLPair
    {
        GL a, b;
        QString strElem;
        QString strUnkNum;
    };

    void getReactantsAndProducts(QList<FormulaGroup> &lReactants, QList<FormulaGroup> &lProducts, QList<Error> &lErrors);
    void getBase(const QList<FormulaGroup> &lReactants, const QList<FormulaGroup> &lProducts,
                 Part &left, Part &right, QMap<GLKey, GLPair> &mapGlPairs,
                 QMap<FormulaGroup, UnkNum> &mapUnkNums, QList<Error> &lErrors);

    QStackedWidget *stackedWidget = new QStackedWidget;
    QWidget *viewNone = new QWidget;
    QListWidget *viewError = new QListWidget;
    FormulaWidget *viewResult = new FormulaWidget;

    QPlainTextEdit *editRel = new QPlainTextEdit;

    QPushButton *btnAnalysis = new QPushButton("配平");

    Highlighter *highter = new Highlighter(editRel->document());
};

#endif // WIDGET_H
