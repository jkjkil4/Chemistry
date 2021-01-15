#include "formula.h"
#include <QRegularExpression>
//#include <QDebug>


Formula::Iter::Iter(const Formula &formula) : formula(formula) {
    if(formula.type == Group) {
        listIter = formula.groupData().begin();
        childIter = new Iter(*listIter);
    }
}

Formula::Iter::Iter(const Iter &other) : formula(other.formula) {
    listIter = other.listIter;
    if(other.childIter)
        childIter = new Iter(*other.childIter);
    mHasNext = other.mHasNext;
}

Formula::Data Formula::Iter::next() {
    if(!mHasNext)
        return Data();

    if(formula.type == Element) {
        mHasNext = false;
        return Data(formula);
    } else {
        Data data = childIter->next();
        if(!childIter->hasNext()) {
            listIter++;
            j::SafeDelete(childIter);
            if(listIter == formula.groupData().end())
                mHasNext = false;
            else childIter = new Iter(*listIter);
        }
        return data;
    }
}

Formula::Iter::~Iter() {
    j::SafeDelete(childIter);
}


//static QRegularExpression ruleElement = QRegularExpression("([A-Z]{1}[a-z]*)(?:\\[([+-]{0,1}\\d+)(?:\\:([,a-z0-9]*)){0,1}\\]){0,1}");
static QRegularExpression ruleElement = QRegularExpression("[A-Z]{1}[a-z]*");

Formula::Formula(Type type, const QString &str, int count) : type(type), count(count)
{
    if(type == Element) {
        //使用正则表达式
        QRegularExpressionMatch match = ruleElement.match(str);
        if(match.capturedStart() != 0 || match.capturedLength() != str.length()) {
            vaild = false;
            return;
        }

//        //得到结果
//        QStringList list = match.capturedTexts();
//        if(list.size() > 2) {
//            pElec = new Elec;
//            pElec->value = PlainFrac(list[2]);
//            if(list.size() > 3)
//                pElec->keys = list[3].split(",", QString::SkipEmptyParts).toSet();
//        }

//        data = new QString(list[1]);
        data = new QString(str);
    } else {
        QList<Formula> *pLChildren = new QList<Formula>;  //new 临时的list
        int bracketCount = 0;   //统计括号数量
        QString divide;     //用于分割
        for(auto iter = str.rbegin(); iter != str.rend(); ++iter) { //反向遍历str的字符
            QChar ch = *iter;   //字符
            divide.insert(0, ch);   //将该字符插入到divide的第一个
            if(ch == ')') {     //如果是')'
                bracketCount++; //括号数量自增
            } else if(ch == '(') {  //如果是'('
                bracketCount--; //括号数量自减
                if(bracketCount == 0) { //如果括号数量为0
                    QString inner;
                    int childCount;
                    if(!ParseStr(Group, divide, inner, childCount)) {   //解析字符串，如果失败则结束
                        delete pLChildren;
                        vaild = false;
                        return;
                    }
                    Formula child(Group, inner, childCount); //子内容(组)
                    if(!child.isVaild()) {  //如果 子内容 解析失败则结束
                        delete pLChildren;
                        vaild = false;
                        return;
                    }
                    pLChildren->insert(0, child); //将该 子内容 插入到 pLChildren 的第一个
                    divide.clear();     //清空divide
                }
            } else if(bracketCount == 0 && ch >= 'A' && ch <= 'Z') {    //如果 括号数量为0 并且 当前字母为大写字母
                QString inner;
                int childCount;
                if(!ParseStr(Element, divide, inner, childCount)) {   //解析字符串，如果失败则结束
                    delete pLChildren;
                    vaild = false;
                    return;
                }
                Formula child(Element, inner, childCount);   //子内容(单个元素)
                if(!child.isVaild()) {  //如果 子内容 解析失败则结束
                    delete pLChildren;
                    vaild = false;
                    return;
                }
                pLChildren->insert(0, child); //将该 子内容 插入到 pLChildren 的第一个
                divide.clear(); //清空divide
            }
        }
        if(pLChildren->isEmpty() || bracketCount != 0) {    //如果没有 子内容 或者 括号数量不为0，则结束
            delete pLChildren;
            vaild = false;
            return;
        }

        data = pLChildren;
    }
}

Formula::Formula(const Formula &other) {
    type = other.type;
    count = other.count;
    vaild = other.vaild;
//    if(other.pElec)
//        pElec = new Elec(*other.pElec);
    if(other.data)
        data = (type == Element ? (void*)new QString(other.elementData()) : (void*)new QList<Formula>(other.groupData()));
}

Formula::~Formula() {
//    j::SafeDelete(pElec);
    type == Element ? delete (QString*)data : delete (QList<Formula>*)data;
}


QString Formula::format(bool useBrackets) const {
    if(type == Element) {
        return count == 1 ? elementData() : elementData() + QString::number(count);
    } else {
        QString inner;
        const QList<Formula>& lChildren = groupData();
        for(const Formula& child : lChildren)
            inner += child.format(true);
        QString strCount = count == 1 ? "" : QString::number(count);
        return useBrackets ? '(' + inner + ')' + strCount : strCount + inner;
    }
}

void Formula::paint(QPainter *p, int &x, int y, PaintAlign pa, bool useBrackets) const {
    int yy = pa == PA_Top ? y + QFontMetrics(p->font()).height() : y;
    QRect rect;

    if(type == Element) {
        //绘制元素
        j::DrawText(p, x, yy, Qt::AlignLeft | Qt::AlignBottom, elementData(), -1, -1, &rect);
        x += rect.width();

        //绘制数量
        if(count != 1) {
            int pointSize = p->font().pointSize();
            j::SetPointSize(p, qMax(1, pointSize / 2));
            j::DrawText(p, x, yy, Qt::AlignLeft | Qt::AlignBottom, QString::number(count), -1, -1, &rect);
            j::SetPointSize(p, pointSize);
            x += rect.width();
        }
    } else {
        if(count != 1 && !useBrackets) {
            //绘制数量
            j::DrawText(p, x, yy, Qt::AlignLeft | Qt::AlignBottom, QString::number(count), -1, -1, &rect);
            x += rect.width();
        }

        if(useBrackets) {
            //绘制左括号
            j::DrawText(p, x, yy, Qt::AlignLeft | Qt::AlignBottom, "(", -1, -1, &rect);
            x += rect.width();
        }

        //绘制子内容
        for(const Formula &child : groupData())
            child.paint(p, x, y, pa, true);

        if(useBrackets) {
            //绘制右括号
            j::DrawText(p, x, yy, Qt::AlignLeft | Qt::AlignBottom, ")", -1, -1, &rect);
            x += rect.width();
        }

        if(count != 1 && useBrackets) {
            //绘制数量
            int pointSize = p->font().pointSize();
            j::SetPointSize(p, qMax(1, pointSize / 2));
            j::DrawText(p, x, yy, Qt::AlignLeft | Qt::AlignBottom, QString::number(count), -1, -1, &rect);
            j::SetPointSize(p, pointSize);
            x += rect.width();
        }
    }
}


const QString& Formula::elementData() const {
    if(type != Element)
        throw;
    return *(QString*)data;
}

const QList<Formula>& Formula::groupData() const {
    if(type != Group)
        throw;
    return *(QList<Formula>*)data;
}


bool Formula::ParseStr(Type type, const QString &str, QString &inner, int &count) {
    if(type == Element) {
        //得到右侧数字长度
        int rightLen = 0;
        for(auto iter = str.rbegin(); iter != str.rend(); ++iter) {
            QChar ch = *iter;
            if(ch < '0' || ch > '9')
                break;
            rightLen++;
        }

        //得到数量
        bool ok = true;
        int tmpCount = rightLen == 0 ? 1 : str.right(rightLen).toInt(&ok);
        if(!ok || tmpCount <= 0) //若失败则返回false
            return false;

        inner = str.left(str.size() - rightLen);    //子内容
        count = tmpCount;   //数量
    } else {
        //得到括号的位置
        int indexOfLeft = str.indexOf('(');
        int indexOfRight = str.lastIndexOf(')');

        //对特定情况进行判断
        if(indexOfLeft != 0 || indexOfLeft == -1 || indexOfRight == -1 || indexOfLeft >= indexOfRight)
            return false;

        //得到数量
        QString strCount = str.right(str.size() - indexOfRight - 1);
        bool ok = true;
        int tmpCount = strCount.isEmpty() ? 1 : strCount.toInt(&ok);
        if(!ok || tmpCount <= 0) //若失败则返回false
            return false;

        inner = str.mid(indexOfLeft + 1, indexOfRight - indexOfLeft - 1);   //子内容
        count = tmpCount;   //数量
    }
    return true;
}

