#include "widget.h"
//#include <QDebug>


QMap<Widget::Error::Type, QString> Widget::Error::mapText = {
    { Any, "%1" },
    { FormulaError, "(E0-1) 解析化学式出错，在 \"%1\" 输入框的 第%2行 第%3个化学式 \"%4\"" },
    { FormulaMultiDefined, "(E0-3) 化学式 \"%1\" 重复出现" },
    { GLMultiDefined, "(E0-4) 元素 \"%1\" 的得失电子对应关系 \"%2\" 在 %3 中重复出现" },

    { IsEmpty, "(E1-0) %1为空" },

    { ElementNotExists, "(E2-0) \"%1\" 元素在 %2 中存在，却无法在 %3 中找到" },
    { GLNotExists, "(E2-1) \"%1\" 元素的得失电子对应关系 \"%2\" 在 %3 中存在，却无法在 %4 中找到" }
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
        QMessageBox::information(this, "帮助", "<a href=\"https://github.com/jkjkil4/Chemistry/blob/main/README.md\">在github上的说明文档</a>");
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


void Widget::getReactantsAndProducts(QList<FormulaGroup> &lReactants, QList<FormulaGroup> &lProducts, QList<Error> &lErrors) {
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

void Widget::getBase(const QList<FormulaGroup> &lReactants, const QList<FormulaGroup> &lProducts,
                     Part &left, Part &right, QMap<GLKey, GLPair> &mapGlPairs,
                     QMap<FormulaGroup, UnkNum> &mapUnkNums, QList<Error> &lErrors)
{
    int unkNumCount = 0;
    auto fnGetBase = [&mapUnkNums, &unkNumCount]
            (const QList<FormulaGroup> &list, Part &part)
    {//lambda，用于 设置未知数 得到原子数 得到电荷数
        for(const FormulaGroup &formula : list) {
            //未知数
            UnkNum &unkNum = mapUnkNums[formula] = unkNumCount == 0 ? "" : 'v' + QString::number(unkNumCount);
            if(unkNumCount == 0)
                unkNum.value = 1;
            Frac mul = Frac(1, unkNum.name);

            //遍历该化学式的所有元素
            FormulaGroup::Iter fIter(formula);
            while(fIter.hasNext()) {
                int count = fIter.currentCount();
                Formula::Data data = fIter.next();
                //得到原子数
                part.mapElemCount[data.name()] += count * mul;
            }
            //得到电荷数
            part.elec += formula.elec() * mul;

            unkNumCount++;
        }
    };
    fnGetBase(lReactants, left);
    fnGetBase(lProducts, right);

    unkNumCount = 1;
    //反应物的对应关系
    for(const FormulaGroup &formula : lReactants) {
        UnkNum &unkNum = mapUnkNums[formula];   //得到未知数

        //遍历该化学式的所有元素
        FormulaGroup::Iter fIter(formula);
        while(fIter.hasNext()) {
            int count = fIter.currentCount();
            Formula::Data data = fIter.next();
            if(data.hasGL()) {
                for(const QString &strRel : data.glKeys()) {
                    GLPair &pair = mapGlPairs[{ data.name(), strRel }];
                    if(pair.strUnkNum.isEmpty()) {
                        pair.strUnkNum = "gl" + QString::number(unkNumCount);
                        unkNumCount++;
                    }
                    if(pair.a.isVaild) {
                        lErrors << Error(Error::GLMultiDefined, QStringList() << data.name() << strRel << "反应物");
                        continue;
                    }
                    pair.a.isVaild = true;
                    pair.a.count = count;
                    pair.a.strUnkNum = unkNum.name;
                    pair.a.elec = data.glValue();
                }
            }
        }
    }
    //生成物的对应关系
    for(const FormulaGroup &formula : lProducts) {
        UnkNum &unkNum = mapUnkNums[formula];   //得到未知数

        //遍历该化学式的所有元素
        FormulaGroup::Iter fIter(formula);
        while(fIter.hasNext()) {
            int count = fIter.currentCount();
            Formula::Data data = fIter.next();
            if(data.hasGL()) {
                for(const QString &strRel : data.glKeys()) {
                    GLPair &pair = mapGlPairs[{ data.name(), strRel }];
                    if(pair.b.isVaild) {
                        lErrors << Error(Error::GLMultiDefined, QStringList() << data.name() << strRel << "生成物");
                        continue;
                    }
                    pair.b.isVaild = true;
                    pair.b.count = count;
                    pair.b.strUnkNum = unkNum.name;
                    pair.b.elec = data.glValue();
                }
            }
        }
    }

    //检查原子
    for(auto iter = left.mapElemCount.begin(); iter != left.mapElemCount.end(); ++iter)
        if(!right.mapElemCount.contains(iter.key()))
            lErrors << Error(Error::ElementNotExists, QStringList() << iter.key() << "反应物" << "生成物");
    for(auto iter = right.mapElemCount.begin(); iter != right.mapElemCount.end(); ++iter)
        if(!left.mapElemCount.contains(iter.key()))
            lErrors << Error(Error::ElementNotExists, QStringList() << iter.key() << "生成物" << "反应物");

    //检查得失电子对应关系
    for(auto iter = mapGlPairs.begin(); iter != mapGlPairs.end(); ++iter) {
        const GLKey &key = iter.key();
        const GLPair &pair = iter.value();
        if(!pair.a.isVaild || !pair.b.isVaild)
            lErrors << Error(Error::GLNotExists, QStringList() << key.strElem << key.strRel
                             << (pair.a.isVaild ? "反应物" : "生成物")
                             << (pair.b.isVaild ? "生成物" : "反应物"));
    }
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
//#include <QDebug>
void Widget::onAnalysis() {
    stackedWidget->setCurrentWidget(viewNone);

    QList<Error> lErrors;   //错误列表

    QList<FormulaGroup> lReactants, lProducts;    //lReactants为反应物，lProducts为生成物

    //得到反应物和生成物
    getReactantsAndProducts(lReactants, lProducts, lErrors);
    CHECK_ERR

    {//配平
        QMap<FormulaGroup, UnkNum> mapUnkNums;   //用于FormulaKey和未知数对应
        QList<Frac> lFracs;     //配平的关系式
        QStringList lRemoveLetters;
        QMap<GLKey, GLPair> mapGlPairs; //得失电子守恒的对应关系
        Part left, right;
        left.name = "反应物";
        right.name = "生成物";

        //获得用于配平的东西
        getBase(lReactants, lProducts, left, right, mapGlPairs, mapUnkNums, lErrors);
        if(!lErrors.isEmpty()) goto Jump;

        //原子守恒
        for(auto iter = left.mapElemCount.begin(); iter != left.mapElemCount.end(); ++iter)
            lFracs << *iter - right.mapElemCount[iter.key()];

        //电荷守恒
        lFracs << left.elec - right.elec;

        //得失电子守恒
        if(!mapGlPairs.isEmpty()) {
            Frac glTotal;
            QMap<QString, Frac> mapBetweenUnkNums;
            for(GLPair &pair : mapGlPairs) {
                Frac frac(1, pair.strUnkNum);
                glTotal += (pair.b.elec - pair.a.elec) * frac;

                auto iter = mapBetweenUnkNums.find(pair.a.strUnkNum);
                if(iter == mapBetweenUnkNums.end())
                    iter = mapBetweenUnkNums.insert(pair.a.strUnkNum, Frac(pair.a.count, pair.a.strUnkNum));
                *iter -= frac;

                iter = mapBetweenUnkNums.find(pair.b.strUnkNum);
                if(iter == mapBetweenUnkNums.end())
                    iter = mapBetweenUnkNums.insert(pair.b.strUnkNum, Frac(pair.b.count, pair.b.strUnkNum));
                *iter -= frac;

                lRemoveLetters << pair.strUnkNum;
            }
            lFracs << glTotal;
            for(Frac &frac : mapBetweenUnkNums)
                lFracs << frac;
        }

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
            QList<Frac> lRes = Frac::SolvingEquations(lFracs, lUnkNumbers, lRemoveLetters, &err);
            if(err.hasError()) {
                lErrors << Error(Error::Any, QStringList() <<
                                 (err.type == Frac::SolvingError::Insufficient
                                  ? "无法成功配平，可能是化学式有误或条件(得失电子守恒)不足"
                                  : "无法成功配平，化学式有误(各守恒冲突)"));
                goto Jump;
            }

            //判断结果是否存在负数或零
            for(Frac &res : lRes) {
                if(res.toPlain() <= 0) {
                    lErrors << Error(Error::Any, QStringList() << "无法成功配平，化学式有误(结果出现负数或零)");
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
                    for(int mono : firstUnkNum->value.top())
                        vValues << mono;
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
