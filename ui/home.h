//
// Created by 11518 on 2022/11/3.
//

#ifndef ZCOM_HOME_H
#define ZCOM_HOME_H

#include <QWidget>
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

public:
    explicit Home(QWidget *parent = nullptr);
    ~Home() override;
private slots:
    //! 打开/关闭串口按钮
    void slotPBnTtySetClicked();
    //! 串口名字改变
    void slotTtyNameTextChanged(const QString & ttyName);
    //! 波特率改变
    void slotBaudTextChanged(const QString &);
    //! 停止位改变
    void slotStopTextChanged(const QString &);
    //! 校验位改变
    void slotParityTextChanged(const QString &);
    //! 数据位改变
    void slotDataTextChanged(const QString &);
    //! 显示串口读到的数据
    void slotSerialRead();
    //! 串口写操作
    void slotSerialWrite(QByteArray);
    //! 串口发生错误操作
    void slotSerialError(QSerialPort::SerialPortError error);
private:
    Ui::Home *ui;
    int m_TimerId; //定时器ID，用来显示当前时间的定时器ID，每秒触发一次

    SingleSend *mpSingleSend;
    MultipleSend *mpMultipleSend;
    ZControl *mpZControl;
};


#endif //ZCOM_HOME_H
