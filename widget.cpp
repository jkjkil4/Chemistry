#include "widget.h"


QMap<Widget::Error::Type, QString> Widget::Error::mapText = {
    { Any, "%1" },
    { FormulaError, "(E0-1) 解析化学式出错，在 \"%1\" 输入框的 第%2行 第%3个化学式 \"%4\"" },
    { FormulaNotExists, "(E0-2) 化学式未定义，在 \"%1\" 输入框的 第%2行 第%3个化学式 \"%4\"" },
    { FormulaMultiDefined, "(E0-3) 化学式 \"%1\" 重复出现" },

    { IsEmpty, "(E1-0) %1为空" },

    { ElementNotExists, "(E2-0) \"%1\" 元素在 %2 中存在，却无法在 %3 中找到" },
};


Widget::FormulaKey::FormulaKey(const QString &str, bool *ok) {
    int indexOfLeft = str.indexOf('{');
    int indexOfRight = str.lastIndexOf('}');
    if(indexOfLeft == -1 || indexOfRight == -1 || indexOfLeft >= indexOfRight) {
        SET_PTR(ok, false);
        qDebug() << "E1" << indexOfLeft << indexOfRight;
        return;
    }
    QString tmpKey = str.left(indexOfLeft);
    if(tmpKey.isEmpty()) {
        SET_PTR(ok, false);
        qDebug() << "E2" << indexOfLeft << indexOfRight;
        return;
    }
    bool ok2;
    PlainFrac tmpElec(str.mid(indexOfLeft + 1, indexOfRight - indexOfLeft - 1), &ok2);
    if(!ok2) {
        SET_PTR(ok, false);
        qDebug() << "E3" << indexOfLeft << indexOfRight;
        return;
    }

    key = tmpKey;
    elec = tmpElec;
    SET_PTR(ok, true);
}


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
    stackedWidget->setCurrentWidget(viewNone);

    QList<Error> lErrors;   //错误列表

    QMap<FormulaKey, FormulaGroup> mapFormulas; //用于FormulaKey和FormulaGroup对应
    QList<FormulaKey> lReactants, lProducts;    //lReactants为反应物，lProducts为生成物

    {//解析化学式
        QTextDocument *doc = editFormula->document();
        int count = doc->lineCount();
        repeat(i, count) {
            QString line = doc->findBlockByLineNumber(i).text();
            QStringList list = line.simplified().split(' ', QString::SplitBehavior::SkipEmptyParts);
            int index = 0;
            for(QString &str : list) {
                FormulaGroup formula(str);
                if(formula.isVaild()) {
                    mapFormulas[FormulaKey(formula)] = formula;
                } else {
                    lErrors << Error(Error::FormulaError,  QStringList() << "化学式" << QString::number(i + 1)
                                     << QString::number(index + 1) << str);
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
        struct Part
        {
            QMap<QString, Frac> mapElemCount;
            Frac elec;
        } left, right;

        QMap<FormulaKey, UnkNum> mapUnkNums;   //用于FormulaKey和未知数对应
        QList<Frac> lFracs;     //配平的关系式
        {//获得用于配平的东西
            int unkNumCount = 0;
            auto fnGetUnkNums = [&mapFormulas, &mapUnkNums, &unkNumCount]
                    (QList<FormulaKey> &list, Part &part)
            {//lambda，用于 设置未知数 得到原子数
                for(auto iter = list.begin(); iter != list.end(); ++iter) {
                    //未知数
                    UnkNum &unkNum = mapUnkNums[*iter] = unkNumCount == 0 ? "" : 'v' + QString::number(unkNumCount);
                    if(unkNumCount == 0)
                        unkNum.value = 1;
                    const FormulaGroup &formula = mapFormulas[*iter];
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
            bool ok;
            QList<Frac> lRes = Frac::SolvingEquations(lFracs, lUnkNumbers, &ok);
            if(!ok) {
                lErrors << Error(Error::Any, QStringList() << "无法成功配平，可能是化学式有误或本程序能力有限(目前有的守恒关系: 原子守恒、电荷守恒)");
            } else {
                QVector<int> vValues;
                for(Frac &frac : lRes)
                    vValues << frac.bottom();
                int commonMulti = j::Lcm(vValues);
                for(Frac &frac : lRes)
                    frac.mul(commonMulti);

                firstUnkNum->value = commonMulti;

                auto lPUnkNumIter = lPUnkNum.begin();
                for(Frac &frac : lRes) {
                    (*lPUnkNumIter)->value = frac;
                    lPUnkNumIter++;
                }

                viewResult->clear();
                for(FormulaKey &key : lReactants)
                    viewResult->addReactant(mapUnkNums[key].value, mapFormulas[key]);
                for(FormulaKey &key : lProducts)
                    viewResult->addProduct(mapUnkNums[key].value, mapFormulas[key]);
                stackedWidget->setCurrentWidget(viewResult);
            }
        }
        Jump:
        CHECK_ERR
    }

    End:;
}

#undef CHECK_ERR
