#include "formulagroup.h"


FormulaGroup::Iter::Iter(const FormulaGroup &formula)
    : formula(formula), listIter(formula.mFormulas.begin()), childIter(new Formula::Iter(*listIter)) {}

Formula::Data FormulaGroup::Iter::next() {
    if(!hasNext())
        return Formula::Data();
    Formula::Data data = childIter->next();
    if(!childIter->hasNext()) {
        listIter++;
        j::SafeDelete(childIter);
        if(listIter == formula.mFormulas.end())
            mHasNext = false;
        else childIter = new Formula::Iter(*listIter);
    }
    return data;
}


FormulaGroup::FormulaGroup(const QString &str)
{
    //得到大括号的位置
    int indexOfLeft = str.indexOf("{");
    int indexOfRight = str.lastIndexOf("}");

    //对特定情况进行判断
    if((indexOfLeft == -1) != (indexOfRight == -1)) {
        mVaild = false;
        return;
    }
    bool hasBrackets = (indexOfLeft != -1 && indexOfRight != -1);
    if(hasBrackets && (indexOfRight != str.length() - 1 || indexOfLeft >= indexOfRight)) {
        mVaild = false;
        return;
    }


    //分割
    QStringList lStrFormulas = (hasBrackets ? str.left(indexOfLeft) : str).split('_', QString::SkipEmptyParts);
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
            mVaild = false;
            mFormulas.clear();
            return;
        }

        //得到化学式
        QString strFormula = str.right(str.length() - leftLen);
        Formula formula(Formula::Group, strFormula, count);
        if(!formula.isVaild()) {
            mVaild = false;
            mFormulas.clear();
            return;
        }
        mFormulas << formula;
    }

    //得到电荷总数
    QString strElec = (hasBrackets ? str.mid(indexOfLeft + 1, indexOfRight - indexOfLeft - 1) : "0");
    bool ok;
    mElec = PlainFrac(strElec, &ok);
    if(!ok) {
        mVaild = false;
        mFormulas.clear();
        return;
    }

    bool hasPrev = false;
    for(const Formula &formula : mFormulas) {
        if(hasPrev) {
            mFormatStr += '_';
        } else hasPrev = true;
        mFormatStr += formula.format();
    }
}

void FormulaGroup::paint(QPainter *p, int x, int y, Formula::PaintAlign pa, QRect *pRect) const {
    int xStart = x;
    QRect rect;
    int fmHeight = QFontMetrics(p->font()).height();
    int yy = pa == Formula::PA_Top ? y + fmHeight : y;
    bool hasPrev = false;
    for(const Formula &formula : mFormulas) {
        if(hasPrev) {
            j::DrawText(p, x, yy, Qt::AlignLeft | Qt::AlignBottom, "·", -1, -1, &rect);
            x += rect.width();
        } else hasPrev = true;
        formula.paint(p, x, y, pa);

        if(mElec != 0) {
            PlainFrac absElec = mElec;
            absElec.abs();

            QString strElec;
            if(absElec != 1)
                strElec = absElec.format();
            strElec += (mElec < 0 ? "-" : "+");

            x += 1;
            int pointSize = p->font().pointSize();
            j::SetPointSize(p, qMax(1, pointSize / 2));
            j::DrawText(p, x, yy - fmHeight, Qt::AlignLeft | Qt::AlignTop, strElec, -1, -1, &rect);
            j::SetPointSize(p, pointSize);
            x += rect.width();
        }
    }
    SET_PTR(pRect, QRect(xStart, y, x - xStart, fmHeight));
}

