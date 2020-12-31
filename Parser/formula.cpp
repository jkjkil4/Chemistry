#include "formula.h"
//#include <QDebug>


Formula::Formula(Type type, const QString &str, int count) : type(type), count(count)
{
    if(type == Element) {
        //得到中括号的位置
        int indexOfLeft = str.indexOf('[');
        int indexOfRight = str.lastIndexOf(']');
        if(indexOfLeft == -1 || indexOfRight == -1 || indexOfLeft >= indexOfRight) {
            vaild = false;
            return;
        }

        //得到元素名称
        QString strElement = str.left(indexOfLeft);
        if(strElement.isEmpty()) {
            vaild = false;
            return;
        }

        //得到电荷数
        bool ok;
        PlainFrac frac(str.mid(indexOfLeft + 1, indexOfRight - indexOfLeft - 1), &ok);
        if(!ok) {
            vaild = false;
            return;
        }

        mElec = frac.moveNegativeToTop();
        data = new QString(strElement);
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

        for(Formula &child : *pLChildren)
            mElec.sum(PlainFrac(child.mElec).mul(child.count));
        data = pLChildren;
    }
}

Formula::Formula(const Formula &other) {
    type = other.type;
    count = other.count;
    mElec = other.mElec;
    vaild = other.vaild;
    data = (type == Element ? (void*)new QString(*(QString*)other.data) : (void*)new QList<Formula>(*(QList<Formula>*)other.data));
}

Formula::~Formula() {
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

void Formula::paint(QPainter &p, int &x, int y, PaintAlign pa, bool useBrackets) const {
    int yy = pa == PA_Top ? y + QFontMetrics(p.font()).height() : y;
    QRect rect;

    if(type == Element) {
        //绘制元素
        j::DrawText(p, x, yy, Qt::AlignLeft | Qt::AlignBottom, elementData(), -1, -1, &rect);
        x += rect.width();

        //绘制数量
        if(count != 1) {
            int pointSize = p.font().pointSize();
            j::SetPointSize(&p, qMax(1, pointSize / 2));
            j::DrawText(p, x, yy, Qt::AlignLeft | Qt::AlignBottom, QString::number(count), -1, -1, &rect);
            j::SetPointSize(&p, pointSize);
            x += rect.width();
        }
    } else {
        if(count != 1 && !useBrackets) {
            //绘制数量
            j::DrawText(p, x, yy, Qt::AlignLeft | Qt::AlignBottom, QString::number(count), -1, -1, &rect);
            x += rect.width();
        }

        //绘制子内容
        for(const Formula &child : groupData())
            child.paint(p, x, y, pa, true);

        if(count != 1 && useBrackets) {
            //绘制数量
            int pointSize = p.font().pointSize();
            j::SetPointSize(&p, qMax(1, pointSize / 2));
            j::DrawText(p, x, yy, Qt::AlignLeft | Qt::AlignBottom, QString::number(count), -1, -1, &rect);
            j::SetPointSize(&p, pointSize);
            x += rect.width();
        }
    }
}

void Formula::elementCount(QMap<QString, Frac> &map, const Frac &mul) const {
    if(type == Element) {
        map[elementData()] += count * mul;
    } else {
        Frac childMul = mul * count;
        for(const Formula &child : groupData())
            child.elementCount(map, childMul);
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

