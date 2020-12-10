#include "formula.h"


Formula::Formula(int count) : count(count) {}


#define IsBlockLetter(ch) (ch >= 'A' && ch <= 'Z')
#define IsLetter(ch) (ch >= 'a' && ch <= 'z')

Formula_Group::Formula_Group(const QString &str, int count)
    : Formula(count)
{
    QString tmpElement, tmpElec, tmpDigit;
    ParserState state = Element_Head;
    for(const QChar &ch : str) {
        switch(state) {
        case Element_Head:
            if(IsBlockLetter(ch)) {
                tmpElement = ch;
                state = Element_Body;
            } else {
                throw ParserError(ParserError::Unknown, "元素必须为大写字母开头");
            }
            break;
        case Element_Body:
            if(IsLetter(ch)) {
                tmpElement += ch;
            } else {
                if(ch != '[')
                    throw ParserError(ParserError::Unknown, "元素后必须标注化合价(用\"[\"和\"]\"包住化合价)");
                state = Elec_Left;
            }
            break;
        case Elec_Left:

            break;
        case Elec_Body:

            break;
        case Elec_Right:

            break;
        case Bracket_Left:

            break;
        case Bracket_Right:

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
