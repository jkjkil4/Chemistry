#ifndef FORMULA_H
#define FORMULA_H

#include "formula_base.h"

class Formula
{
public:
    Formula(const QString &str, bool *ok = nullptr) : formula(str, 1, ok) {}
    ~Formula() = default;
    Formula_Group formula;
};

#endif // FORMULA_H
