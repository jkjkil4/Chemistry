#include "widget.h"
#include <QDebug>

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

void Widget::onAnalysis() {
    stackedWidget->setCurrentWidget(viewNone);
    QVector<QPoint> vErrorPos;

    QMap<FormulaKey, Formula*> mapFormulas;
    QVector<Formula*> vReactants, vProducts;

    {//解析化学式
        QTextDocument *doc = editFormula->document();
        int count = doc->lineCount();
        repeat(i, count) {
            QString line = doc->findBlockByLineNumber(i).text();
            QStringList list = line.split(' ', QString::SplitBehavior::SkipEmptyParts);
            int index = 0;
            for(QString &str : list) {
                bool ok;
                Formula *formula = new Formula(str, 1, &ok);
                if(ok) {
                    mapFormulas[FormulaKey(formula)] = formula;
                } else {
                    vErrorPos << QPoint(index, i);
                    delete formula;
                }
                index++;
            }
        }
    }

    if(!vErrorPos.isEmpty()) {
        viewError->clear();
        for(QPoint pos : vErrorPos) {
            QListWidgetItem *item = new QListWidgetItem("(E0-1) 解析化学式出错，在 \"化学式\" 输入框的 第"
                               + QString::number(pos.y()) + "行 第" + QString::number(pos.x() + 1) + "个化学式");
            item->setIcon(QApplication::style()->standardIcon(QStyle::StandardPixmap::SP_MessageBoxCritical));
            viewError->addItem(item);
        }
        stackedWidget->setCurrentWidget(viewError);
        goto End;
    }


    //清空
    End:for(Formula *formula : mapFormulas)
        delete formula;
    mapFormulas.clear();
}
