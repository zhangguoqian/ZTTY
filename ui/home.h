//
// Created by 11518 on 2022/11/3.
//

#ifndef ZCOM_HOME_H
#define ZCOM_HOME_H

#include <QWidget>
#include <QMouseEvent>
#include <QRubberBand>
#include "singlesend.h"
#include "multiplesend.h"
#include "../control/zcontrol.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Home; }
QT_END_NAMESPACE

class Home : public QWidget {
Q_OBJECT
protected:
    //! 重写定时器
    void timerEvent(QTimerEvent *event) override;
    //! 重写鼠标事件
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
public:
    explicit Home(QWidget *parent = nullptr);
    ~Home() override;
private slots:
    //! 打开/关闭串口按钮
    void slotPBnTtySetClicked();
    //! 串口名字改变
    void slotTtyNameTextChanged(const QString & ttyName);
    //! 波特率改变
    void slotBaudTextChanged(const QString &text);
    //! 停止位改变
    void slotStopTextChanged(const QString &text);
    //! 校验位改变
    void slotParityTextChanged(const QString &text);
    //! 数据位改变
    void slotDataTextChanged(const QString &text);
    //! 显示串口读到的数据
    void slotSerialRead();
    //! 串口写操作
    void slotSerialWrite(QByteArray array,bool isHex,bool isEnter);
    void slotSerialWrite(QByteArray array,bool isEnter);
    //! 串口发生错误操作
    void slotSerialError(QSerialPort::SerialPortError error);
    //! 循环获取串口列表
    void slotSerialPortListChange(const QStringList &list,QString currentTtyName);
    //! 16进制接收
    void slotHexShowStateChanged(int state);
private:
    Ui::Home *ui;
    int m_TimerId; //定时器ID，用来显示当前时间的定时器ID，每秒触发一次

    SingleSend *mpSingleSend; //单条发送的界面
    MultipleSend *mpMultipleSend; //多条发送的界面
    ZControl *mpZControl; //总控界面
    uint64_t m_SendNumber; //发送计数
    uint64_t m_RecNumber; //接收计数
    QByteArray m_RecShow; //接收展示

    QRubberBand *mpRubberBand;//橡皮筋
    QPoint m_Start; //橡皮筋起点
};


#endif //ZCOM_HOME_H
