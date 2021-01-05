#include "widget.h"


QMap<Widget::Error::Type, QString> Widget::Error::mapText = {
    { Any, "%1" },
    { FormulaError, "(E0-1) 解析化学式出错，在 \"%1\" 输入框的 第%2行 第%3个化学式 \"%4\"" },
    { FormulaMultiDefined, "(E0-3) 化学式 \"%1\" 重复出现" },

    { IsEmpty, "(E1-0) %1为空" },

    { ElementNotExists, "(E2-0) \"%1\" 元素在 %2 中存在，却无法在 %3 中找到" },
};


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    editRel->setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);
    j::SetFamily(editRel, fontSourceCodePro.family);
    j::SetPointSize(editRel, 13);
    connect(btnAnalysis, SIGNAL(clicked()), this, SLOT(onAnalysis()));

    stackedWidget->setObjectName("StackedWidget");
    stackedWidget->setStyleSheet("#StackedWidget{background-color:rgb(225, 225, 225)}");
    stackedWidget->addWidget(viewNone);
    stackedWidget->addWidget(viewError);
    stackedWidget->addWidget(viewResult);

    QMenuBar *menuBar = new QMenuBar;

    QMenu *menuOther = new QMenu("其他");
    menuBar->addMenu(menuOther);

    QAction *actHelp = new QAction("帮助");
    menuOther->addAction(actHelp);
    connect(actHelp, &QAction::triggered, [&](){
        QFile file(":/rc/rc/explain1.txt");
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            in.setCodec("utf-8");
            QMessageBox::information(this, "帮助", in.readAll());
            file.close();
        }
    });

    QAction *actExample = new QAction("例子");
    menuOther->addAction(actExample);
    connect(actExample, &QAction::triggered, [&](){
        QFile file(":/rc/rc/explain2.txt");
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            in.setCodec("utf-8");
            QMessageBox::information(this, "例子", in.readAll());
            file.close();
        }
    });

    menuOther->addSeparator();

    QAction *actAbout = new QAction("关于");
    menuOther->addAction(actAbout);
    connect(actAbout, &QAction::triggered, [&](){

        QMessageBox::about(this, "关于",
                           "作者: jkjkil4<br>"
                           "github: <a href = https://github.com/jkjkil4/Chemistry>https://github.com/jkjkil4/Chemistry</a><br>"
                           "反馈问题: jkjkil@qq.com");
    });
    QAction *actAboutQt = new QAction("关于Qt");
    menuOther->addAction(actAboutQt);
    connect(actAboutQt, &QAction::triggered, [&](){ QMessageBox::aboutQt(this); });


    //创建布局
    QHBoxLayout *layEditRel = new QHBoxLayout;
    layEditRel->addWidget(editRel);
    QGroupBox *gbEditRel = new QGroupBox("反应物与生成物");
    gbEditRel->setLayout(layEditRel);

    QHBoxLayout *layBottom = new QHBoxLayout;
    layBottom->addStretch();
    layBottom->addWidget(btnAnalysis);

    QVBoxLayout *layEdit = new QVBoxLayout;
    layEdit->addWidget(gbEditRel);
    layEdit->addLayout(layBottom);

    QWidget *widgetEdit = new QWidget;
    widgetEdit->setLayout(layEdit);

    QSplitter *splitter = new QSplitter(Qt::Vertical);
    splitter->addWidget(widgetEdit);
    splitter->addWidget(stackedWidget);
    splitter->setSizes(QList<int>() << 400 << 100);

    QHBoxLayout *layCentral = new QHBoxLayout;
    layCentral->addWidget(splitter);

    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(layCentral);

    QVBoxLayout *layMain = new QVBoxLayout;
    layMain->setMargin(0);
    layMain->addWidget(menuBar);
    layMain->addWidget(centralWidget);
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
    stackedWidget->setCurrentWidget(viewNone);

    QList<Error> lErrors;   //错误列表

    QList<FormulaGroup> lReactants, lProducts;    //lReactants为反应物，lProducts为生成物

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
                FormulaGroup formula(str);
                if(formula.isVaild()) {
                    if(!lReactants.contains(formula) && !lProducts.contains(formula)) {
                        (isProduct ? lProducts : lReactants) << formula;
                    } else {
                        lErrors << Error(Error::FormulaMultiDefined, QStringList() << str);
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
        struct Part
        {
            QMap<QString, Frac> mapElemCount;
            Frac elec;
        } left, right;

        QMap<FormulaGroup, UnkNum> mapUnkNums;   //用于FormulaKey和未知数对应
        QList<Frac> lFracs;     //配平的关系式
        {//获得用于配平的东西
            int unkNumCount = 0;
            auto fnGetUnkNums = [&mapUnkNums, &unkNumCount]
                    (const QList<FormulaGroup> &list, Part &part)
            {//lambda，用于 设置未知数 得到原子数
                for(auto iter = list.begin(); iter != list.end(); ++iter) {
                    //未知数
                    UnkNum &unkNum = mapUnkNums[*iter] = unkNumCount == 0 ? "" : 'v' + QString::number(unkNumCount);
                    if(unkNumCount == 0)
                        unkNum.value = 1;
                    const FormulaGroup &formula = *iter;
                    Frac mul = Frac(1, unkNum.name);
                    //得到原子数
                    formula.elementCount(part.mapElemCount, mul);
                    //得到电荷数
                    part.elec += formula.elec() * mul;

                    unkNumCount++;
                }
            };
            fnGetUnkNums(lReactants, left);
            fnGetUnkNums(lProducts, right);

            //检查
            for(auto iter = left.mapElemCount.begin(); iter != left.mapElemCount.end(); ++iter)
                if(!right.mapElemCount.contains(iter.key()))
                    lErrors << Error(Error::ElementNotExists, QStringList() << iter.key() << "反应物" << "生成物");
            for(auto iter = right.mapElemCount.begin(); iter != right.mapElemCount.end(); ++iter)
                if(!left.mapElemCount.contains(iter.key()))
                    lErrors << Error(Error::ElementNotExists, QStringList() << iter.key() << "生成物" << "反应物");
            if(!lErrors.isEmpty())
                goto Jump;
        }

        //原子守恒
        for(auto iter = left.mapElemCount.begin(); iter != left.mapElemCount.end(); ++iter)
            lFracs << *iter - right.mapElemCount[iter.key()];

        //电荷守恒
        lFracs << left.elec - right.elec;

        {//解方程，得出结果
            QList<UnkNum*> lPUnkNum;
            UnkNum* firstUnkNum = nullptr;
            QStringList lUnkNumbers;
            for(UnkNum &unkNum : mapUnkNums) {
                if(unkNum.name.isEmpty()) {
                    firstUnkNum = &unkNum;
                } else {
                    lPUnkNum << &unkNum;
                    lUnkNumbers << unkNum.name;
                }
            }
            Frac::SolvingError err;
            QList<Frac> lRes = Frac::SolvingEquations(lFracs, lUnkNumbers, &err);
            if(err.hasError()) {
                lErrors << Error(Error::Any, QStringList() <<
                                 (err.type == Frac::SolvingError::Insufficient
                                  ? "无法成功配平，可能是化学式有误或本程序能力有限(目前有的守恒关系: 原子守恒、电荷守恒)"
                                  : "无法成功配平，化学式有误(各守恒冲突)"
                                    ));
                goto Jump;
            }

            //判断结果是否存在负数
            for(Frac &res : lRes) {
                if(res.toPlain() < 0) {
                    lErrors << Error(Error::Any, QStringList() << "无法成功配平，化学式有误(结果出现负数)");
                    goto Jump;
                }
            }

            {//进行一些处理后 输出最终结果
                {//通分
                    QVector<int> vValues;
                    for(Frac &frac : lRes)
                        vValues << frac.bottom();
                    int commonMulti = j::Lcm(vValues);
                    if(commonMulti > 1) {
                        for(Frac &frac : lRes)
                            frac.mul(commonMulti);
                        firstUnkNum->value = commonMulti;
                    }
                }

                {//约分
                    QVector<int> vValues;
                    for(Frac &frac : lRes)
                        for(int mono : frac.top())
                            vValues << mono;
                    int div = j::Gcd(vValues);
                    if(div > 1) {
                        for(Frac &frac : lRes)
                            frac.div(div);
                        firstUnkNum->value.div(div);
                    }
                }

                auto lPUnkNumIter = lPUnkNum.begin();
                for(Frac &frac : lRes) {
                    (*lPUnkNumIter)->value = frac;
                    lPUnkNumIter++;
                }

                viewResult->clear();
                for(FormulaGroup &formula : lReactants)
                    viewResult->addReactant(mapUnkNums[formula].value, formula);
                for(FormulaGroup &formula : lProducts)
                    viewResult->addProduct(mapUnkNums[formula].value, formula);
                stackedWidget->setCurrentWidget(viewResult);
            }
        }
        Jump:
        CHECK_ERR
    }

    End:;
}

#undef CHECK_ERR
