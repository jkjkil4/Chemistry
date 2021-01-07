#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    Highlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    QRegularExpression ruleGL = QRegularExpression("\\[([+-]{0,1}\\d+)(?:\\:([,a-z0-9]*)){0,1}\\]");
    QRegularExpression ruleGLInner = QRegularExpression(",");
    QRegularExpression ruleDigit = QRegularExpression("\\d+");
    QRegularExpression ruleElec = QRegularExpression("\\{[+-]{0,1}\\d*\\}");
    QRegularExpression ruleProduct = QRegularExpression("\\$");

    QTextCharFormat formatBracket;
    QTextCharFormat formatRel;
    QTextCharFormat formatDigit;
    QTextCharFormat formatElec;
    QTextCharFormat formatProduct;
};

#endif // HIGHLIGHTER_H
