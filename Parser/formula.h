#ifndef FORMULA_H
#define FORMULA_H

#include <QString>
#include <QList>

#include "parsererror.h"

class Formula
{
public:
    enum Type { Group, Element };
    virtual ~Formula() = default;
};

class Formula_Group
{
public:
    Formula_Group(const QString &str, int count = 1);

    int count;
    QList<Formula*> childs;

private:
    enum ParserState {
        Element_Head, Element_Body,
        Elec_Left, Elec_Body, Elec_Right,
        Bracket_Left, Bracket_Right
    };
};

class Formula_Element
{
public:
    Formula_Element(const QString &str, int count, );
};

#endif // FORMULA_H
