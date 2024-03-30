#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <QLayout>
#include <QMessageBox>
#include <iostream>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    waitForOperator = true;  // 初始状态等待操作数
    result = 0.0;            // 初始状态结果为0
    ui->result->setText("0");

    connectSolts();    // 连接所有的按键与对应的槽函数
    setShortCut();

}

Widget::~Widget()
{
    delete ui;
}

bool Widget::calculate(double operand, QString pendingOperator)
{
    if(pendingOperator == "+")
    {
        result += operand;     // 加法运算
    }
    else if(pendingOperator == "-")
    {
        result -= operand;
    }
    else if(pendingOperator == "*")
    {
        result *= operand;
    }
    else
    {
        if(operand == 0)
        {
            return false;
        }
        result /= operand;
    }
    return true;
}

void Widget::abortOperation()
{
    // 终止运算，清除数据，输出错误信息
    result = 0;
    pendingOperator.clear();      // 清除运算符
    ui->result->setText("0");     // 清除显示结果
    waitForOperator = true;       // 等待一个操作数输入
    QMessageBox::warning(this, "运算错误", "除数不能为零");
}

void Widget::connectSolts()
{
    // 链接槽函数
    // 将十个数字键连接到槽函数onDigitClicked();
    QPushButton *digit_btns[10] =
    {
        ui->btn_0,
        ui->btn_1,
        ui->btn_2,
        ui->btn_3,
        ui->btn_4,
        ui->btn_5,
        ui->btn_6,
        ui->btn_7,
        ui->btn_8,
        ui->btn_9
    };

    for(auto btn : digit_btns)
    //for(int i=0; i<10; i++)
    {
         // 将按键连接到槽函数
        connect(btn, SIGNAL(clicked()), this, SLOT(onDigitClicked()));
    }

    QPushButton *operatorBtn[4] =
    {
        ui->btn_add,
        ui->btn_subtract,
        ui->btn_multiply,
        ui->btn_divide
    };

    for(auto btn : operatorBtn)
    // for(int i=0; i<4; i++)
    {
        connect(btn, SIGNAL(clicked()), this, SLOT(onOperatorClicked()));
    }

    connect(ui->btn_equal, SIGNAL(clicked()), this, SLOT(onEqualBtnClicked()));   // 等号键按下
    connect(ui->btn_dot, SIGNAL(clicked()), this, SLOT(onDotBtnClicked()));       // 小数点键安按下
    connect(ui->btn_clear, SIGNAL(clicked()), this, SLOT(onClearBtnClicked()));    // 清除键按下
    connect(ui->btn_clear_all, SIGNAL(clicked()), this, SLOT(onClearAllBtnClicked()));  // 清除所有 按钮
    //connect(ui->btn_neg_pos, SIGNAL(clicked()),this, SLOT(onSignBtnClicked()));    // 符号按键
}


void Widget::onDigitClicked()
{
    // std::cout << "数字键按下" << std::endl;
    qDebug() << "digit key pressed" << endl;
    QPushButton *digitBtn = static_cast<QPushButton*>(sender());   // sender()表示信号的发送者
    QString value = digitBtn->text();    // 获取按钮的text属性
    // 判断按键
    if(ui->result->text() == "0" && value == "0")    // 按键为0
        return;
    if(waitForOperator)                  // 等在操作数 状态为真
    {
        ui->result->setText(value);
        waitForOperator = false;         // 此时不再需要等待操作数
    }
    else
    {
        ui->result->setText(ui->result->text() + value);
    }
}

void Widget::onOperatorClicked()
{
    // qDebug() << "operator key pressed" << endl;
    // 判断按下的运算符键
    QPushButton *clickedBtn = static_cast<QPushButton*>(sender());    // 将信号源转化为QpusuBytton指针
    QString value = clickedBtn->text();      // 获取运算符
    // 此时的状态是按下了运算符，所以需要获取第一个运算数
    double operand = ui->result->text().toDouble();     // 获取运算数
    if(pendingOperator.isEmpty())   // 运算符为空
    {
        result = operand;
    }
    else
    {
        if(!calculate(operand, value))
        {
            abortOperation();
            return;
        }
        ui->result->setText(QString::number(result));
    }
    // 更新运算符
    pendingOperator = value;
    waitForOperator = true;    // 等待新的输入数字
}

void Widget::onEqualBtnClicked()
{
    // 等号键按下，需要计算最终的结果
    double operand = ui->result->text().toDouble();   // 获取运算数
    if(pendingOperator.isEmpty())   // 没有输入加减乘除运算符，直接按了等号
    {
        return;
    }
    if(!calculate(operand, pendingOperator))
    {
        abortOperation();
        return;
    }
    ui->result->setText(QString::number(result));
    waitForOperator = true;
    result = 0.0;
    pendingOperator.clear();
}

void Widget::onDotBtnClicked()
{
    if(waitForOperator)
    {
        ui->result->setText("0");
    }
    if(ui->result->text().contains("."))
    {
        // no operation
    }
    else
    {
        ui->result->setText(ui->result->text() + ".");
    }
    waitForOperator = false;       // 当前数字的输入还未结束
}

void Widget::onClearBtnClicked()
{
    ui->result->setText("0");     // 输入的数字清零
    waitForOperator = true;       // 重新输入
}

void Widget::onClearAllBtnClicked()
{
    ui->result->setText("0");
    waitForOperator = true;
    result = 0.0;
    pendingOperator.clear();
}
/*
void Widget::onSignBtnClicked()
{
    QString text = ui->result->text();
    double value = text.toDouble();
    if(value > 0)
    {
        text.prepend("-");
    }
    else
    {
        text.remove(0, 1);
    }
    ui->result->setText(text);
}*/


void Widget::setShortCut()
{
    Qt::Key key[17] = {
        Qt::Key_0, Qt::Key_1, Qt::Key_2,
        Qt::Key_3, Qt::Key_4, Qt::Key_5,
        Qt::Key_6, Qt::Key_7, Qt::Key_8,
        Qt::Key_9,
        Qt::Key_Plus, Qt::Key_Minus, Qt::Key_Asterisk, Qt::Key_Slash,
        Qt::Key_Enter, Qt::Key_Period, Qt::Key_Backspace//, Qt::Key_M

    };

    QPushButton *btn[17] = {
        ui->btn_0, ui->btn_1, ui->btn_2,
        ui->btn_3, ui->btn_4, ui->btn_5,
        ui->btn_6, ui->btn_7, ui->btn_8,
        ui->btn_9,
        ui->btn_add, ui->btn_subtract, ui->btn_multiply, ui->btn_divide,
        ui->btn_equal, ui->btn_dot, ui->btn_clear//, ui->btn_neg_pos
    };

    for(int i=0; i<18; i++)
    {
        btn[i]->setShortcut(QKeySequence(key[i]));
    }
    ui->btn_clear_all->setShortcut(QKeySequence("Ctrl+Backspace"));
}


void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_0:
            emit ui->btn_0->clicked();
            break;
        case Qt::Key_1:
            emit ui->btn_1->clicked();
            break;
        case Qt::Key_2:
            emit ui->btn_2->clicked();
            break;
        case Qt::Key_3:
            emit ui->btn_3->clicked();
            break;
        case Qt::Key_4:
            emit ui->btn_4->clicked();
            break;
        case Qt::Key_5:
            emit ui->btn_5->clicked();
            break;
        case Qt::Key_6:
            emit ui->btn_6->clicked();
            break;
        case Qt::Key_7:
            emit ui->btn_7->clicked();
            break;
        case Qt::Key_8:
            emit ui->btn_8->clicked();
            break;
        case Qt::Key_9:
            emit ui->btn_9->clicked();
            break;
        case Qt::Key_Plus:
            emit ui->btn_add->clicked();
            break;
        case Qt::Key_Minus:
            emit ui->btn_subtract->clicked();
            break;
        case Qt::Key_Asterisk:
            emit ui->btn_multiply->clicked();
            break;
        case Qt::Key_Slash:
            emit ui->btn_divide->clicked();
            break;
        case Qt::Key_Enter:
        case Qt::Key_Equal:
            emit ui->btn_equal->clicked();
            break;
        case Qt::Key_Period:
            emit ui->btn_dot->clicked();
            break;
        /*case Qt::Key_M:
            emit ui->btn_neg_pos->clicked();
            break;*/
        case Qt::Key_Backspace:
            emit ui->btn_clear->clicked();
            break;
        default:
            break;
    }
}
