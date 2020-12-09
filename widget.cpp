#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    j::SetPointSize(editFormula, 11);
    j::SetPointSize(editRel, 11);
    tabWidget->addTab(editFormula, "化学式");
    tabWidget->addTab(editRel, "反应物与生成物");
    connect(btnAnalysis, SIGNAL(clicked()), this, SLOT(onAnalysis()));

    //创建布局
    QHBoxLayout *layBottom = new QHBoxLayout;
    layBottom->addStretch();
    layBottom->addWidget(btnAnalysis);

    QVBoxLayout *layMain = new QVBoxLayout;
    layMain->addWidget(tabWidget);
    setLayout(layMain);

    //设置属性
    resize(800, 608);
}

Widget::~Widget()
{

}

void Widget::onAnalysis() {

    //QStringList lReactants, lProducts;


}
