#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QKeyEvent>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();


private:
    Ui::Widget *ui;

private:
    bool calculate(double operand, QString pendingOperator);  // 做算数运算
    void abortOperation();     // 结束运算
    void connectSolts();       // 将数字键以及符号键连接到槽函数

    QString pendingOperator;   // 存储运算符
    double result;             // 存储运算结果
    bool waitForOperator;      // 标志位，是否等待操作数

private slots:
    void onDigitClicked();     // 数字键按下对应的槽函数
    void onOperatorClicked();  // 运算符键按下对应的槽函数
    void onEqualBtnClicked();  // 按下运算键对应的槽函数
    void onDotBtnClicked();    // 按下小数点对应的槽函数
    void onClearBtnClicked();  // 清除按钮对应的槽函数
    void onClearAllBtnClicked();  // 清除所有按钮对应的槽函数
    //void onSignBtnClicked();   // 正负号按键所对应的槽函数

public:
    void setShortCut();        // 设计快捷键

protected:
    void keyPressEvent(QKeyEvent* event);

};

#endif // WIDGET_H
