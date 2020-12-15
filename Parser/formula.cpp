#include "formula.h"
#include <QDebug>


Formula::Formula(int count) : count(count) {}


#define IsBlockLetter(ch) (ch >= 'A' && ch <= 'Z')
#define IsLetter(ch) (ch >= 'a' && ch <= 'z')
#define IsDigit(ch) (ch >= '0' && ch <= '9')
#define IsSign(ch) (ch == '+' || ch == '-')

Formula_Group::Formula_Group(QString str, int count)
    : Formula(count)
{
    str += '$';
    QString divide;
    for(QChar ch : str) {
        if((IsBlockLetter(ch) || ch == '$') && !divide.isEmpty()) {
            qDebug() << divide;
            divide.clear();
        }
        divide += ch;
    }
}

Formula_Group::~Formula_Group() {
    for(Formula *child : childs) {
        delete child;
    }
}


Formula_Element::Formula_Element(const QString &str, int count, int elec)
    : Formula(count), elec(elec)
{

}
