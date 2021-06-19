#include "expression-analysis.h"

ExpressionAnalysis::ExpressionAnalysis(){}

//Функция для вычисления выражения с двумя переменными
QString ExpressionAnalysis::calculate(QString a, QString b, QString spec_symb)
{
    if(spec_symb == '+')
        return QString::number(a.toDouble() + b.toDouble());
    else if(spec_symb == '-')
        return QString::number(a.toDouble() - b.toDouble());
    else if(spec_symb == '/')
        return QString::number(a.toDouble() / b.toDouble());
    else if(spec_symb == '*')
        return QString::number(a.toDouble() * b.toDouble());
    else if(spec_symb == '^')
        return QString::number(pow(a.toDouble(), b.toDouble()));
    else return NULL;
}

//Функция для вычисления с одной перемеренной
QString ExpressionAnalysis::calculate_trigonometry(QString x, QString spec_symb)
{
    if(spec_symb == "cos"){
        return QString::number(cos(x.toDouble()));
    }
    else if(spec_symb == "sin"){
        return QString::number(sin(x.toDouble()));
    }
    else if(spec_symb == "tg"){
        return QString::number(tan(x.toDouble()));
    }
    else if(spec_symb == "ctg"){
        return QString::number(1 / tan(x.toDouble()));
    }
    else
        return NULL;
}

//Проверка на то, что символ является операцией
bool ExpressionAnalysis::is_spec_symbol(QString symbol)
{
    if(symbol == '+'   ||
       symbol == '-'   ||
       symbol == '*'   ||
       symbol == '/'   ||
       symbol == '^'   ||
       symbol == "cos" ||
       symbol == "sin" ||
       symbol == "tg"  ||
       symbol == "ctg"
    ) return true;
    else return false;
}

//Получение приоритета для правильного составления обратной польской нотации
int ExpressionAnalysis::get_priority_spec_symbol(QString symbol)
{
    if(symbol == '+')
        return addition;
    else if(symbol == '-')
        return subtraction;
    else if(symbol == '/')
        return division;
    else if(symbol == '*')
        return multiplication;
    else if(symbol == '^')
        return exponentiation;
    else if(symbol == "cos")
        return cosine;
    else if(symbol == "sin")
        return sinus;
    else if(symbol == "tg")
        return tanget;
    else if(symbol == "ctg")
        return contangens;
    else return 0;
}


void ExpressionAnalysis::analize_bracket()
{
    if(bracket_left != 0 && bracket_right != 0){
        if(bracket_left == bracket_right){
            //Если выражение заканчивается скобкой
            if(input.size() == 1){
                // Удаление всех закрывающих скобок
                QMutableVectorIterator<QString> iter(stack_operation);
                while(iter.hasNext())
                    if(iter.next() == ')')
                        iter.remove();
                while(!stack_operation.isEmpty()){
                    if(stack_operation.last() != '('){
                        output.push_back(stack_operation.last());
                        stack_operation.remove(stack_operation.size() - 1);
                    }
                    else{
                        stack_operation.remove(stack_operation.size() - 1);
                    }
                }
                bracket_left = 0;
                bracket_right = 0;
            }
            //Возможно участок кода ниже не нужен, но это не точно
            else{
                QMutableVectorIterator<QString> iter(stack_operation);
                while(iter.hasNext())
                    if(iter.next() == ')')
                        iter.remove();
                while(!stack_operation.isEmpty()){
                    if(stack_operation.last() != '('){
                        output.push_back(stack_operation.last());
                        stack_operation.remove(stack_operation.size() - 1);
                    }
                    else{
                        stack_operation.remove(stack_operation.size() - 1);
                    }
                }
                bracket_left = 0;
                bracket_right = 0;
            }
        }
    }
}

//====ФИЧА====//
//Функция удаляет первую закрывающую скобку и вычисляет значение между ними
//это нужно для того, чтобы корректно работать со вложенными скобками
void ExpressionAnalysis::remove_first_right_bracket(){
    if(bracket_left > bracket_right){
        QMutableVectorIterator<QString> iter(stack_operation);
        while(iter.hasNext())
            if(iter.next() == ')')
                iter.remove();
        while(stack_operation.last() != '('){
            output.push_back(stack_operation.last());
            stack_operation.remove(stack_operation.size() - 1);
        }
        stack_operation.remove(stack_operation.size() - 1);
        bracket_left -= 1;
        bracket_right -= 1;
    }
    else return;
}

QString ExpressionAnalysis::analysis(QString expression)
{
    stack_operation.clear();
    result.clear();
    output.clear();
    input.clear();
    this->expression = expression;
    QString array_char[40];//Временный массив для хранения операций и операндов
    QString temp;
    int count = 0;
    //Так как 'expression' содержит числа, то нужно сделать так,
    //чтобы многозначные числа считались как один элемент.
    //Тоже самое и с тригонометрическими функциями
    for(int i = 0, j = 0; i < expression.length(); i++, count++){
        if(expression[i] == ')' ||
           expression[i] == '+' ||
           expression[i] == '/' ||
           expression[i] == '*' ||
           expression[i] == '^'
        ){
            array_char[count] = expression[i];
            continue;
        }
        else if(expression[i] == '-'){
            if(i == 0){
                array_char[count] = '0';
                array_char[++count] = '-';
                continue;
            }
            array_char[count] = expression[i];
            continue;
        }
        else if(expression[i] == '('){
            if(expression[i + 1] == '-'){
                array_char[count] = '(';
                array_char[++count] = '0';
                array_char[++count] = '-';
                i++;
                continue;
            }
            array_char[count] = expression[i];
            continue;
        }

        for(j = i; j < expression.length(); j++){
            if(expression[j] == 'c'){
                if(expression[j + 1] == 'o'){
                    temp = "cos";
                }else{
                    temp = "ctg";
                }
                i += 2;
                break;
            }
            if(expression[j] == 's'){
                temp = "sin";
                i += 2;
                break;
            }
            else if(expression[j] == 't'){
                temp = "tg";
                i += 1;
                break;
            }
            if(expression[j] != '+' &&
               expression[j] != '-' &&
               expression[j] != '/' &&
               expression[j] != '*' &&
               expression[j] != ')' &&
               expression[j] != '^'
            ){
                temp += expression[j];
                continue;
            }
            else{
                i = j - 1;
                break;
            }

        }
        if(j == expression.length()){
            i = j;
        }
        array_char[count] = temp;
        temp.clear();
    }
    for(int i = 0; i < array_char->length(); i++){
        qDebug() << array_char[i];
    }
    for(int i = 0; i < count; i++){
        input.push_back(array_char[i]);
    }
    qDebug() << input;
//===================================================================
    //Преобразование выражения в обратную польскую запись
    while(!input.isEmpty()){
        if(input.first() == "("){
            stack_operation.push_back("(");
            bracket_left += 1;
            analize_bracket();
            input.pop_front();
            continue;
        }
        else if(input.first() == ")"){
            stack_operation.push_back(")");
            bracket_right += 1;
            remove_first_right_bracket();
            analize_bracket();
            input.pop_front();
            continue;
        }


        if(is_spec_symbol(input.first())){
            if(stack_operation.empty()){
                stack_operation.push_back(input.first());
            }
            else{
                if(get_priority_spec_symbol(input.first()) <= get_priority_spec_symbol(stack_operation.last())){
                    output.push_back(stack_operation.last());
                    stack_operation.remove(stack_operation.size() - 1);
                    stack_operation.push_back(input.first());
                }
                else{
                    stack_operation.push_back(input.first());
                }

            }
        }
        else{
            output.push_back(input.first());
        }
        input.pop_front();
    }

    while(!stack_operation.isEmpty()){
        output.push_back(stack_operation.last());
        stack_operation.remove(stack_operation.size() - 1);
    }
    qDebug() << stack_operation;
    qDebug() << output;

    //Вычисление обратной польской нотации через стек
    while(!output.isEmpty()){
        result.push(output.dequeue());
        if(is_spec_symbol(result.top())){
            QString a, b, spec_symb;
            //Вычисление если нужен только одна переменная
            if(result.top() == "cos" ||
               result.top() == "tg"  ||
               result.top() == "ctg" ||
               result.top() == "sin")
            {
                spec_symb = result.pop();
                a = result.pop();
                result.push(calculate_trigonometry(a, spec_symb));
            }
             //Вычисление если нужны две переменные
            else{
                spec_symb = result.pop();
                b = result.pop();
                a = result.pop();
                result.push(calculate(a, b, spec_symb));
            }
        }
    }
    qDebug() << result;
    return result.top();;
}
