#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "expression-analysis.h"

#include <QMouseEvent>
#include <QFont>
#include <QFontMetrics>
#include <QLineEdit>
#include <QFontDatabase>

static QString valueOfConsistentAlgorithms;
static QString valueOfStepbystepAlgorithms;
static QString valueOfSegmentalAlgorithms;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap myPixmap(":/image/sum_icon_151075 (1).png");
    ui->picture->setPixmap(myPixmap);
    ui->close->installEventFilter(this);
    ui->rollUp->installEventFilter(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    int id = QFontDatabase::addApplicationFont(":/fonts/fonts/Nunito-SemiBold.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont monospace(family);


    thread1 = new QThread(this);
    thread2 = new QThread(this);
    thread3 = new QThread(this);

    connect(this, SIGNAL(destroyed()), thread1, SLOT(quit()));
    connect(this, SIGNAL(destroyed()), thread2, SLOT(quit()));
    connect(this, SIGNAL(destroyed()), thread3, SLOT(quit()));

    workerConsistent = new Worker();
    workerStepbystep = new Worker();
    workerSegmental = new Worker();

    connect(this, SIGNAL(startConsistent(int, int, QString)), workerConsistent, SLOT(runConsistent(int, int, QString)));
    connect(this, SIGNAL(startStepbystep(int, int, QString)), workerStepbystep, SLOT(runStepbystep(int, int, QString)));
    connect(this, SIGNAL(startSegmental(int, int, QString)), workerSegmental, SLOT(runSegmental(int, int, QString)));

    workerConsistent->moveToThread(thread1);
    workerStepbystep->moveToThread(thread2);
    workerSegmental->moveToThread(thread3);

    thread1->start();
    thread2->start();
    thread3->start();


}

void MainWindow::write()
{
    ui->consistent->setText(valueOfConsistentAlgorithms);
    ui->StepByStep->setText(valueOfStepbystepAlgorithms);
    ui->segmental->setText(valueOfSegmentalAlgorithms);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_calculate_clicked()
{
    int start = ui->start->text().toInt();
    int end = ui->end->text().toInt();
    QString string = ui->expression->text();
    emit startConsistent(start, end, string);
    emit startStepbystep(start, end, string);
    emit startSegmental(start, end, string);
    write();
}

Worker::Worker(QObject *parent)
{

}

void Worker::runConsistent(int start, int end, QString string)
{
    QString expression;
    double summa = 0.0;
    ExpressionAnalysis expressionAnalysis;// Объект класса, который вычисляет выражение под знаком суммы на каждой итерации
    //Вырожение под знаком суммы содержит неизвестный элемент 'x' вместо привычного 'n'
    for(int i = start; i <= end; i++){
        expression = string;
        expression.replace(QString("x"), QString(QString::number(i)));//Заменяем неизвестный 'x' на i
        summa += expressionAnalysis.analysis(expression).toDouble();//Функция analysis принимает в парметры выражение и возвращает строковое значение
    }
    valueOfConsistentAlgorithms = QString::number(summa);
}

void Worker::runStepbystep(int start, int end, QString string)
{
    QString expression;
    ExpressionAnalysis expressionAnalysis;
    double temp[1] = {0};
    //Реализация упрощенного шагового алгоритма для одного потока
    for(int i = 0; i < 1; i++){
        for(int k = start; k <= end; k += 1){
            expression = string;
            expression.replace(QString("x"), QString(QString::number(k)));
            temp[i] += expressionAnalysis.analysis(expression).toDouble();
        }
    }
    valueOfStepbystepAlgorithms = QString::number(temp[0]);
}

void Worker::runSegmental(int start, int end, QString string)
{

    QString expression;
    ExpressionAnalysis expressionAnalysis;
    double temp[1] = {0};
    int m = (end - start) / 1 + 1;
    for(int i = 0; i < 1; i++){
        //int start = i * m;
        int finish = (i + 1) * m < (end - start) ? (i + 1) * m : end;
        for(int k = start /*=start*/; k <= finish; k++){
            expression = string;
            expression.replace(QString("x"), QString(QString::number(k)));
            temp[i] += expressionAnalysis.analysis(expression).toDouble();
        }
    }
    valueOfSegmentalAlgorithms = QString::number(temp[0]);
}

//====ФУНКЦИИ НИЖЕ ОТНОСЯТСЯ ТОЛЬКО К ИНТЕРФЕЙСУ ПРИЛОЖЕНИЯ====//

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->close){
        if(event->type() == QEvent::MouseButtonPress){
            QApplication::quit();
            return true;
        }
    }
    else if(watched == ui->rollUp){
        if(event->type() == QEvent::MouseButtonPress){
            showMinimized();
            return true;
        }
    }
    return false;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton){
        moving = true;
        lastPos = event->pos();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons() == Qt::LeftButton && moving){
        this->move(this->pos() + (event->pos() - lastPos));
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton){
        moving = false;
    }

}

void MainWindow::on_expression_textEdited(const QString &arg1)
{
    if(ui->expression->text().length() == 0){
        ui->expression->resize(81, 31);
    }
    static int countSymb = 0;
    if(ui->expression->text().length() >= countSymb && countSymb != 0){
        qDebug() << "Edit1";
        ui->expression->resize(ui->expression->width() + 10, 31);
        countSymb++;
        return;
    }
    if(ui->expression->text().length() < countSymb && countSymb != 0){
        qDebug() << "Edit1";
        countSymb = ui->expression->text().length();
        ui->expression->resize(ui->expression->width() - 8, 31);
        return;
    }
    if(ui->expression->text().length() >= 6){
        qDebug() << "Edit";
        countSymb = ui->expression->text().length();
    }
}




