#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QTabWidget>
#include <QPushButton>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <header.h>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void onAnalysis();

private:
    QTabWidget *tabWidget = new QTabWidget;

    QPlainTextEdit *editFormula = new QPlainTextEdit;
    QPlainTextEdit *editRel = new QPlainTextEdit;

    QPushButton *btnAnalysis = new QPushButton;
};

#endif // WIDGET_H
