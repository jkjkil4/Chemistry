#include "formula.h"


Formula::Formula(int count) : count(count) {}


#define IsBlockLetter(ch) (ch >= 'A' && ch <= 'Z')
#define IsLetter(ch) (ch >= 'a' && ch <= 'z')
#define IsDigit(ch) (ch >= '0' && ch <= '9')
#define IsSign(ch) (ch == '+' || ch == '-')

Formula_Group::Formula_Group(const QString &str, int count)
    : Formula(count)
{
    enum ParserState {
        PS_Element_Head, PS_Element_Body,
        PS_Elec_Head, PS_Elec_Body, PS_Count
    };

    QString tmpElement, tmpElec, tmpDigit;
    ParserState state = PS_Element_Head;
    for(const QChar &ch : str) {
        switch(state) {
        case PS_Element_Head:
            tmpElement = "";
            tmpElec = "";
            tmpDigit = "";

            if(IsBlockLetter(ch)) {
                tmpElement = ch;
                state = PS_Element_Body;
            } else {
                throw ParserError(ParserError::Unknown, "元素必须为大写字母开头");
            }
            break;
        case PS_Element_Body:
            if(IsLetter(ch)) {
                tmpElement += ch;
            } else {
                if(ch != '[')
                    throw ParserError(ParserError::Unknown, "元素后必须标注化合价(用\"[\"和\"]\"包住化合价)");
                state = PS_Elec_Head;
            }
            break;
        case PS_Elec_Head:
            if(!IsSign(ch) && !IsDigit(ch))
                throw ParserError(ParserError::Unknown, "电荷数 格式错误");
            tmpElec = ch == '-' ? "-" : "";
            state = PS_Elec_Body;
            break;
        case PS_Elec_Body:
            if(ch == ']') {
                //TODO:
            }
            if(!IsDigit(ch))
                throw ParserError(ParserError::Unknown, "电荷数 格式错误");
            tmpElec += ch;
            break;
        case PS_Count:

            break;
        }
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
