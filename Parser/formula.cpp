#include "formula.h"
#include <QDebug>


Formula::Formula(Frac count) : count(count) {}


#define IsBlockLetter(ch) (ch >= 'A' && ch <= 'Z')
#define IsLetter(ch) (ch >= 'a' && ch <= 'z')
#define IsDigit(ch) (ch >= '0' && ch <= '9')
#define IsSign(ch) (ch == '+' || ch == '-')

Formula_Group::Formula_Group(QString str, Frac count, bool *ok)
    : Formula(count)
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
                qDebug().noquote() << "\033[35m" << divide << "\033[0m";
                QString strDigit;
                for(int i = divide.length() - 1; i >= 0; i--) {
                    if(divide[i] >= '0' && divide[i] <= '9') {
                        strDigit.insert(0, divide[i]);
                        divide.remove(i, 1);
                    } else break;
                }
                qDebug().noquote() << "\033[34m" << divide << strDigit << "\033[0m";
                childs << new Formula_Group(divide.mid(1, divide.length() - 2));
            } else {
                qDebug() << divide;
                //bool ok;
                childs << new Formula_Element(divide, ok);
            }
            divide.clear();
        }
        divide += ch;
    }
    SET_PTR(ok, bracketsCount == 0);
}

Formula_Group::~Formula_Group() {
    for(Formula *child : childs) {
        delete child;
    }
}


Formula_Element::Formula_Element(const QString &str, bool *ok)
{
    int indexOfLeftElec = str.indexOf('[');
    int indexOfRightElec = str.lastIndexOf(']');
    element = (element = str.left(indexOfLeftElec));
    QString strElec = str.mid(indexOfLeftElec + 1, indexOfRightElec - indexOfLeftElec - 1);
    QString strCount = str.right(str.length() - indexOfRightElec - 1);

}
