#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QQueue>
#include <QStack>
#include <QVector>
#include <QLabel>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(QObject* parent = 0);
public slots:
    void runConsistent(int, int, QString);
    void runStepbystep(int, int, QString);
    void runSegmental(int, int, QString);
signals:

private:

};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void write();
    void call();
    ~MainWindow();
    bool eventFilter(QObject *watched, QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

public slots:


private slots:
    void on_expression_textEdited(const QString &arg1);
    void on_calculate_clicked();

signals:
    void signalMouseHover(QLabel*);
    void startConsistent(int, int, QString);
    void startStepbystep(int, int, QString);
    void startSegmental(int, int, QString);
private:
    Ui::MainWindow *ui;
    Worker *workerConsistent;
    Worker *workerStepbystep;
    Worker *workerSegmental;
    QThread *thread1;
    QThread *thread2;
    QThread *thread3;
    QPoint lastPos;
    bool moving = false;
};
#endif // MAINWINDOW_H
