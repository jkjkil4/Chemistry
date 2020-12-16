#include "formula.h"
//#include <QDebug>


Formula_Parent::Formula_Parent(int count) : count(count) {}


#define IsBlockLetter(ch) (ch >= 'A' && ch <= 'Z')
#define IsLetter(ch) (ch >= 'a' && ch <= 'z')
#define IsDigit(ch) (ch >= '0' && ch <= '9')
#define IsSign(ch) (ch == '+' || ch == '-')

Formula_Group::Formula_Group(QString str, int count, bool *ok)
    : Formula_Parent(count)
{
    str += '$';
    QString divide;
    int bracketsCount = 0;
    for(QChar ch : str) {
        if(ch == '(') bracketsCount++;
        else if(ch == ')') bracketsCount--;

        bool condBlockLetter = IsBlockLetter(ch) && bracketsCount == 0;
        bool condEnd = ch == '$';
        bool condBracket = ch == '(' && bracketsCount == 1;
        if((condBlockLetter || condEnd || condBracket) && !divide.isEmpty()) {
            if(divide[0] == '(') {
                //qDebug().noquote() << "\033[35m" << divide << "\033[0m";

                //分割式子和个数
                QString strDigit;
                for(int i = divide.length() - 1; i >= 0; i--) {
                    if(divide[i] >= '0' && divide[i] <= '9') {
                        strDigit.insert(0, divide[i]);
                        divide.remove(i, 1);
                    } else break;
                }

                //检查格式
                for(QChar ch : strDigit) {
                    if(!IsDigit(ch)) {
                        SET_PTR(ok, false);
                        return;
                    }
                }

                //个数
                Frac count = strDigit.isEmpty() ? 1 : strDigit.toInt();

                //qDebug().noquote() << "\033[34m" << divide << strDigit << "\033[0m";
                bool ok2;
                childs << new Formula_Group(divide.mid(1, divide.length() - 2), 1, &ok2);
                if(!ok2) {
                    SET_PTR(ok, false);
                    return;
                }
            } else {
                //qDebug() << divide;
                bool ok2;
                childs << new Formula_Element(divide, &ok2);
                if(!ok2) {
                    SET_PTR(ok, false);
                    return;
                }
            }
            divide.clear();
        }
        divide += ch;
    }

    if(childs.isEmpty()) {
        SET_PTR(ok, false);
        return;
    }

    for(Formula_Parent *child : childs) {
        elec.sum(Frac(child->elec).mul(child->count));
    }

    SET_PTR(ok, bracketsCount == 0);
}

Formula_Group::~Formula_Group() {
    for(Formula_Parent *child : childs) {
        delete child;
    }
}

void Formula_Group::paint(QPainter &p, int &xOffset) {

}


Formula_Element::Formula_Element(const QString &str, bool *ok)
{
    int indexOfLeftElec = str.indexOf('[');
    int indexOfRightElec = str.lastIndexOf(']');

    //得到元素、电荷数、个数
    element = (element = str.left(indexOfLeftElec));
    QString strElec = str.mid(indexOfLeftElec + 1, indexOfRightElec - indexOfLeftElec - 1);
    QString strCount = str.right(str.length() - indexOfRightElec - 1);

    //检查是否符合格式要求
    if(element.isEmpty() || strElec.isEmpty()) {
        SET_PTR(ok, false);
        return;
    }
    for(QChar ch : element) {
        if(!IsBlockLetter(ch) && !IsLetter(ch)) {
            SET_PTR(ok, false);
            return;
        }
    }
    bool ok2;
    elec = Frac(strElec, &ok2);
    if(!ok2) {
        SET_PTR(ok, false);
        return;
    }
    if(!strCount.isEmpty()) {
//        bool isLetter = IsLetter(strCount[0]);
//        for(QChar ch : strCount) {
//            if(!(isLetter ? IsLetter(ch) : IsDigit(ch))) {
//                SET_PTR(ok, false);
//                return;
//            }
//        }
        for(QChar ch : strCount) {
            if(!IsDigit(ch)) {
                SET_PTR(ok, false);
                return;
            }
        }
    }
    count = strCount.isEmpty() ? 1 : strCount.toInt();

    SET_PTR(ok, true);
}

void Formula_Element::paint(QPainter &p, int &xOffset) {

}

QString Formula_Element::formatInfo() {
    return "元素: " + element + "   电荷: " + elec.format() + "   个数: " + QString::number(count);
}
