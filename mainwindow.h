#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QQueue>
#include <QStack>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void analize();
    bool is_spec_symbol(QString);
    int get_priority_spec_symbol(QString);
    void analize_bracket();
    void remove_first_right_bracket();
    QString calculate(QString, QString, QString);
    QString calculate_trigonometry(QString, QString);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    QVector<QString> input;
    QVector<QString> stack_operation;
    QQueue<QString> output;
    QStack<QString> result;
    int count = 0;
    QString expression;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
