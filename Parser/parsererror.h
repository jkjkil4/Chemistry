#ifndef PARSERERROR_H
#define PARSERERROR_H

#include <QString>

struct ParserError
{
    enum Type { Unknown };
    ParserError() : type(Unknown) {}
    ParserError(Type type, const QString &what) : type(type), what(what) {}

    Type type;
    QString what;
};

#endif // PARSERERROR_H
