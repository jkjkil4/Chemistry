#include "highlighter.h"
#include <QDebug>

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    formatBracket.setForeground(Qt::darkRed);
    formatRel.setForeground(QColor(180, 40, 180));
    formatDigit.setForeground(QColor(100, 100, 220));
    formatElec.setForeground(QColor(220, 100, 0));
    formatProduct.setForeground(Qt::darkGreen);
}

void Highlighter::highlightBlock(const QString &text) {
    //匹配"$"
    QRegularExpressionMatchIterator matchIterator = ruleProduct.globalMatch(text);
    while(matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), formatProduct);
    }

    //匹配数字
    matchIterator = ruleDigit.globalMatch(text);
    while(matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), formatDigit);
    }

    //匹配得失电子
    matchIterator = ruleGL.globalMatch(text);
    while(matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        QStringList list = match.capturedTexts();

        setFormat(match.capturedStart(), 1, formatBracket);
        switch(list.length()) {
        case 3: {
            int pos = match.capturedStart() + 1 + list[1].length();
            setFormat(pos, 1, formatBracket);
            setFormat(pos + 1, match.capturedEnd() - pos - 2, formatRel);
            QRegularExpressionMatchIterator matchIterator2 = ruleGLInner.globalMatch(text, pos + 1);
            while(matchIterator2.hasNext()) {
                QRegularExpressionMatch match2 = matchIterator2.next();
                if(match2.capturedEnd() >= match.capturedEnd())
                    break;
                setFormat(match2.capturedStart(), match2.capturedLength(), formatBracket);
            }
        }
        [[clang::fallthrough]]; case 2:
            setFormat(match.capturedStart() + 1, list[1].length(), formatElec);
            break;
        }
        setFormat(match.capturedEnd() - 1, 1, formatBracket);
    }

    //匹配电荷总数
    matchIterator = ruleElec.globalMatch(text);
    while(matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();

        setFormat(match.capturedStart(), 1, formatBracket);
        setFormat(match.capturedStart() + 1, match.capturedLength() - 2, formatElec);
        setFormat(match.capturedEnd() - 1, 1, formatBracket);
    }
}
