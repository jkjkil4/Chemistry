#include "highlighter.h"
#include <QDebug>

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    mFormatBracket.setForeground(Qt::darkRed);
//    formatRel.setForeground(QColor(200, 40, 200));
    mFormatDigit.setForeground(QColor(100, 100, 220));
    mFormatElec.setForeground(QColor(220, 100, 0));
    mFormatProduct.setForeground(Qt::darkGreen);
}

void Highlighter::highlightBlock(const QString &text) {
    //匹配"$"
    QRegularExpressionMatchIterator matchIterator = mRuleProduct.globalMatch(text);
    while(matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), mFormatProduct);
    }

    //匹配数字
    matchIterator = mRuleDigit.globalMatch(text);
    while(matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), mFormatDigit);
    }

//    //匹配得失电子
//    matchIterator = ruleGL.globalMatch(text);
//    while(matchIterator.hasNext()) {
//        QRegularExpressionMatch match = matchIterator.next();
//        QStringList list = match.capturedTexts();

//        setFormat(match.capturedStart(), 1, formatBracket);
//        switch(list.length()) {
//        case 3: {
//            int pos = match.capturedStart() + 1 + list[1].length();
//            setFormat(pos, 1, formatBracket);
//            setFormat(pos + 1, match.capturedEnd() - pos - 2, formatRel);
//            QRegularExpressionMatchIterator matchIterator2 = ruleGLInner.globalMatch(text, pos + 1);
//            while(matchIterator2.hasNext()) {
//                QRegularExpressionMatch match2 = matchIterator2.next();
//                if(match2.capturedEnd() >= match.capturedEnd())
//                    break;
//                setFormat(match2.capturedStart(), match2.capturedLength(), formatBracket);
//            }
//        }
//        [[clang::fallthrough]]; case 2:
//            setFormat(match.capturedStart() + 1, list[1].length(), formatElec);
//            break;
//        }
//        setFormat(match.capturedEnd() - 1, 1, formatBracket);
//    }

    //匹配电荷总数
    matchIterator = mRuleElec.globalMatch(text);
    while(matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();

        setFormat(match.capturedStart(), 1, mFormatBracket);
        setFormat(match.capturedStart() + 1, match.capturedLength() - 2, mFormatElec);
        setFormat(match.capturedEnd() - 1, 1, mFormatBracket);
    }
}
