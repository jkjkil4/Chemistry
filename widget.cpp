#include "widget.h"


QMap<Widget::Error::Type, QString> Widget::Error::mapText = {
    { Any, "%1" },
    { FormulaError, "(E0-1) 解析化学式出错，在 \"%1\" 输入框的 第%2行 第%3个化学式 \"%4\"" },
    { FormulaNotExists, "(E0-2) 化学式未定义，在 \"%1\" 输入框的 第%2行 第%3个化学式 \"%4\"" },
    { FormulaMultiDefined, "(E0-3) 化学式 \"%1\" 重复出现" },

    { IsEmpty, "(E1-0) %1为空" },

    { ElementNotExists, "(E2-0) \"%1\" 元素在 %2 中存在，却无法在 %3 中找到" }
};


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    editFormula->setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);
    j::SetPointSize(editFormula, 13);
    editRel->setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);
    j::SetPointSize(editRel, 13);;
    connect(btnAnalysis, SIGNAL(clicked()), this, SLOT(onAnalysis()));

    stackedWidget->setObjectName("StackedWidget");
    stackedWidget->setStyleSheet("#StackedWidget{background-color:rgb(225, 225, 225)}");
    stackedWidget->addWidget(viewNone);
    stackedWidget->addWidget(viewError);
    stackedWidget->addWidget(viewResult);


    //创建布局
    QHBoxLayout *layEditFormula = new QHBoxLayout;
    layEditFormula->addWidget(editFormula);
    QGroupBox *gbEditFormula = new QGroupBox("化学式");
    gbEditFormula->setLayout(layEditFormula);

    QHBoxLayout *layEditRel = new QHBoxLayout;
    layEditRel->addWidget(editRel);
    QGroupBox *gbEditRel = new QGroupBox("反应物与生成物");
    gbEditRel->setLayout(layEditRel);

    QHBoxLayout *layTop = new QHBoxLayout;
    layTop->addWidget(gbEditFormula);
    layTop->addWidget(gbEditRel);

    QHBoxLayout *layBottom = new QHBoxLayout;
    layBottom->addStretch();
    layBottom->addWidget(btnAnalysis);

    QVBoxLayout *layEdit = new QVBoxLayout;
    layEdit->addLayout(layTop);
    layEdit->addLayout(layBottom);
    setLayout(layEdit);

    QWidget *widgetEdit = new QWidget;
    widgetEdit->setLayout(layEdit);

    QSplitter *splitter = new QSplitter(Qt::Vertical);
    splitter->addWidget(widgetEdit);
    splitter->addWidget(stackedWidget);
    splitter->setSizes(QList<int>() << 400 << 100);

    QHBoxLayout *layMain = new QHBoxLayout;
    layMain->addWidget(splitter);
    setLayout(layMain);


    //设置属性
    resize(800, 608);
}

Widget::~Widget()
{

}

//如果有错误，对错误内容进行提示 并跳至结尾
#define CHECK_ERR \
    if(!lErrors.isEmpty()) {                \
        viewError->clear();                 \
        for(Error &err : lErrors) {         \
            QListWidgetItem *item = new QListWidgetItem(err.text());\
            item->setIcon(QApplication::style()->standardIcon(QStyle::StandardPixmap::SP_MessageBoxCritical));\
            viewError->addItem(item);       \
        }                                   \
        stackedWidget->setCurrentWidget(viewError);\
        goto End;                           \
    }

void Widget::onAnalysis() {
    /*stackedWidget->setCurrentWidget(viewNone);

    QList<Error> lErrors;

    QMap<FormulaKey, Formula*> mapFormulas;
    QList<FormulaKey> lReactants, lProducts;

    {//解析化学式
        QTextDocument *doc = editFormula->document();
        int count = doc->lineCount();
        repeat(i, count) {
            QString line = doc->findBlockByLineNumber(i).text();
            QStringList list = line.simplified().split(' ', QString::SplitBehavior::SkipEmptyParts);
            int index = 0;
            for(QString &str : list) {
                bool ok;
                Formula *formula = new Formula(str, 1, &ok);
                if(ok) {
                    mapFormulas[FormulaKey(formula)] = formula;
                } else {
                    lErrors << Error(Error::FormulaError,  QStringList() << "化学式" << QString::number(i + 1)
                                     << QString::number(index + 1) << str);
                    delete formula;
                }
                index++;
            }
        }
    }
    CHECK_ERR

    {//解析反应物与生成物
        QTextDocument *doc = editRel->document();
        int count = doc->lineCount();
        repeat(i, count) {
            QString line = doc->findBlockByLineNumber(i).text();
            QStringList list = line.simplified().split(' ', QString::SplitBehavior::SkipEmptyParts);
            int index = 0;
            for(QString &str : list) {
                bool isProduct = false;
                if(str[0] == '$') {
                    isProduct = true;
                    str.remove(0, 1);
                }
                bool ok;
                FormulaKey key(str, &ok);
                if(ok) {
                    if(!mapFormulas.contains(key)) {
                        lErrors << Error(Error::FormulaNotExists, QStringList() << "反应物与生成物" << QString::number(i + 1)
                                         << QString::number(index + 1) << str);
                    } else {
                        if(!lReactants.contains(key) && !lProducts.contains(key)) {
                            (isProduct ? lProducts : lReactants) << key;
                        } else {
                            lErrors << Error(Error::FormulaMultiDefined, QStringList() << str);
                        }
                    }
                } else {
                    lErrors << Error(Error::FormulaError, QStringList() << "反应物与生成物" << QString::number(i + 1)
                                     << QString::number(index + 1) << str);
                }
                index++;
            }
        }
        //检查是否为空
        if(lErrors.isEmpty()) {
            if(lReactants.isEmpty())
                lErrors << Error(Error::IsEmpty, QStringList() << "反应物");
            if(lProducts.isEmpty())
                lErrors << Error(Error::IsEmpty, QStringList() << "生成物");
        }
    }
    CHECK_ERR

    {//配平
        QMap<QString, Frac> mapLElement, mapRElement;
        QMap<FormulaKey, QString> mapElementCoeff;
        {//得到两边的各原子总数
            int count = 0;
            for(FormulaKey &key : lReactants) {   //得到反应物的各原子总数
                QString coeff = count == 0 ? "" : (mapElementCoeff[key] = "v" + QString::number(count));
                QMap<QString, Frac> tmpMap;
                mapFormulas[key]->elementCount(tmpMap);
                for(auto iter = tmpMap.begin(); iter != tmpMap.end(); ++iter)
                    mapLElement[iter.key()].sum(iter->changeMono("", coeff));
                count++;
            }
            for(FormulaKey &key : lProducts) {    //得到生成物的各原子总数
                QString coeff = mapElementCoeff[key] = "v" + QString::number(count);
                QMap<QString, Frac> tmpMap;
                mapFormulas[key]->elementCount(tmpMap);
                for(auto iter = tmpMap.begin(); iter != tmpMap.end(); ++iter)
                    mapRElement[iter.key()].sum(iter->changeMono("", coeff));
                count++;
            }
        }

        //检查是否两边含有相同元素(如果没有则报错)
        for(auto iter = mapLElement.begin(); iter != mapLElement.end(); ++iter)
            if(!mapRElement.contains(iter.key()))
                lErrors << Error(Error::ElementNotExists, QStringList() << iter.key() << "反应物" << "生成物");
        for(auto iter = mapRElement.begin(); iter != mapRElement.end(); ++iter)
            if(!mapLElement.contains(iter.key()))
                lErrors << Error(Error::ElementNotExists, QStringList() << iter.key() << "生成物" << "反应物");

        if(lErrors.isEmpty()) {
            QList<Frac> lEquations;
            {//原子守恒
                for(auto iter = mapLElement.begin(); iter != mapLElement.end(); ++iter)
                    lEquations << Frac(*iter).sub(mapRElement[iter.key()]);
            }

            for(Frac &frac : lEquations)
                qDebug().noquote() << frac.format();

            //解方程
            QStringList lCoeffs;
            for(QString &coeff : mapElementCoeff)
                lCoeffs << coeff;
            bool ok;
            Frac::SolvingEquations(lEquations, lCoeffs, &ok);
            qDebug() << ok;
        }
    }
    CHECK_ERR

    //清空
    End:
    for(Formula *formula : mapFormulas)
        delete formula;
    mapFormulas.clear();*/
}

#undef CHECK_ERR
