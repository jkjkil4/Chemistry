#include "highlighter.h"
#include <QDebug>

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    //formatRel.setForeground(Qt::magenta);
    formatBracket.setForeground(Qt::darkRed);
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

    //匹配电荷总数
    matchIterator = ruleElec.globalMatch(text);
    while(matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();

        setFormat(match.capturedStart(), 1, formatBracket);
        setFormat(match.capturedStart() + 1, match.capturedLength() - 2, formatElec);
        setFormat(match.capturedEnd() - 1, 1, formatBracket);
    }
}
