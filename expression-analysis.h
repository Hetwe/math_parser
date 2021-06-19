#ifndef EXPRESSIONANALYSIS_H
#define EXPRESSIONANALYSIS_H
#include <QQueue>
#include <QStack>
#include <QString>
#include <QVector>
#include <QDebug>

//Используются для выставления проритета
#define addition 1
#define subtraction 1
#define division 2
#define multiplication 3
#define exponentiation 4
#define cosine 5
#define sinus 5
#define tanget 5
#define contangens 5

class ExpressionAnalysis
{
private:
    QVector<QString> input; //Входная строчка
    QVector<QString> stack_operation; //Вектор с операциями
    QQueue<QString> output; //Выходная строчка, которая содержит выражение в обратной польской нотации
    QStack<QString> result; //Стек для вычисления выражения в обратной польской нотации
    int bracket_left = 0; //кл-во левых скобки
    int bracket_right = 0; //кол-во правых скобки

    QString expression;
    QString calculate_trigonometry(QString, QString);
    bool is_spec_symbol(QString);
    int get_priority_spec_symbol(QString);
    void analize_bracket();
    void remove_first_right_bracket();
    QString calculate(QString, QString, QString);
public:
    ExpressionAnalysis();
    QString analysis(QString);
};

#endif // EXPRESSIONANALYSIS_H
