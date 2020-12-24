#include "formulagroup.h"

FormulaGroup::FormulaGroup(const QString &str)
{
    QStringList lStrFormulas = str.split('_', QString::SkipEmptyParts);
    for(QString &str : lStrFormulas) {
        //得到左端数字长度
        int leftLen = 0;
        for(QChar ch : str) {
            if(ch < '0' || ch > '9')
                break;
            leftLen++;
        }

        //得到个数
        bool ok = true;
        int count = leftLen == 0 ? 1 : str.left(leftLen).toInt(&ok);
        if(!ok || count <= 0) {
            vaild = false;
            return;
        }

        //得到化学式
        QString strFormula = str.right(str.length() - leftLen);
        Formula formula(Formula::Group, strFormula, count);
        if(!formula.isVaild()) {
            vaild = false;
            return;
        }
        lFormulas << formula;
    }
}

QString FormulaGroup::format() {
    QString res;
    bool hasPrev = false;
    for(Formula &formula : lFormulas) {
        if(hasPrev) {
            res += '_';
        } else hasPrev = true;
        res += formula.format();
    }
    return res;
}
