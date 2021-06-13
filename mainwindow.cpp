#include "mainwindow.h"
#include "ui_mainwindow.h"

#define addition 1
#define subtraction 1
#define division 2
#define multiplication 3
#define exponentiation 4
#define cosine 5

static int bracket_left = 0;
static int bracket_right = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

bool MainWindow::is_spec_symbol(QString symbol)
{
    if(symbol == '+' ||
       symbol == '-' ||
       symbol == '*' ||
       symbol == '/' ||
       symbol == '^' ||
       symbol == "cos"
    ) return true;
    else return false;
}
//(6+1-4)/(1+1*2)+1
int MainWindow::get_priority_spec_symbol(QString symbol)
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
    else return 0;
}

void MainWindow::analize_bracket()
{
    if(bracket_left != 0 && bracket_right != 0){
        if(bracket_left == bracket_right){
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
void MainWindow::remove_first_right_bracket(){
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

QString MainWindow::calculate(QString a, QString b, QString spec_symb)
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

QString MainWindow::calculate_trigonometry(QString x, QString spec_symb)
{
    if(spec_symb == "cos"){
        return QString::number(cos(x.toDouble()));
    }
}



void MainWindow::analize()
{
    QString string = ui->lineEdit->text();
     // expression = ui->lineEdit->text();
    QString array_char[100];
    QString temp;
    count = 0;
    for(int i = 0, j = 0; i < string.length(); i++, count++){
        if(string[i] == ')' ||
           string[i] == '+' ||
           string[i] == '/' ||
           string[i] == '*' ||
           string[i] == '^'
        ){
            array_char[count] = string[i];
            continue;
        }
        else if(string[i] == '-'){
            if(i == 0){

                array_char[count] = '0';
                array_char[++count] = '-';
                continue;
            }
            array_char[count] = string[i];
            continue;
        }
        else if(string[i] == '('){
            if(string[i + 1] == '-'){
                array_char[count] = '(';
                array_char[++count] = '0';
                array_char[++count] = '-';
                i++;
                continue;
            }
            array_char[count] = string[i];
            continue;
        }

        for(j = i; j < string.length(); j++){
            if(string[j] == 'c'){
                temp = "cos";
                i += 2;
                break;
            }
            if(string[j] != '+' &&
               string[j] != '-' &&
               string[j] != '/' &&
               string[j] != '*' &&
               string[j] != ')' &&
               string[j] != '^'
            ){
                temp += string[j];
                continue;
            }
            else{
                i = j - 1;
                break;
            }

        }
        if(j == string.length()){
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
    //system("pause");
//    QString a = "adsdffasdf";
//    QString b = "fasdfasdfsdafssdgds";
//    a = b;
//     expression = *new QString[count];
//     expression[0] = array_char[0];
//     for (int i = 0; i < count; i++) {
//         expression[i] = "1";
//     }
//===================================================================
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
//    for(int i = 0; i < expression.length(); i++)
//    {

//    }
    while(!stack_operation.isEmpty()){
        output.push_back(stack_operation.last());
        stack_operation.remove(stack_operation.size() - 1);
    }
    qDebug() << stack_operation;
    qDebug() << output;
    while(!output.isEmpty()){
        result.push(output.dequeue());
        if(is_spec_symbol(result.top())){
            QString a, b, spec_symb;
            if(result.top() == "cos"){
                spec_symb = result.pop();
                a = result.pop();
                result.push(calculate_trigonometry(a, spec_symb));
            }
            else{
                spec_symb = result.pop();
                b = result.pop();
                a = result.pop();
                result.push(calculate(a, b, spec_symb));
            }
        }
    }
    qDebug() << result;
    ui->lineEdit_2->setText(result.pop());
    stack_operation.clear();
    result.clear();
    output.clear();
    input.clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    analize();
}
