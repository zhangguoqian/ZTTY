//
// Created by 11518 on 2022/11/3.
//

#ifndef ZTTY_ZSERIALPORT_H
#define ZTTY_ZSERIALPORT_H

#include <QSerialPort>
#include <QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class ZSerialPort; }
QT_END_NAMESPACE

class ZSerialPort : public QSerialPort {
Q_OBJECT
protected:
    void timerEvent(QTimerEvent *event) override;

public:
    explicit ZSerialPort(QObject *parent = nullptr);

    ~ZSerialPort() override;
    //!获取非活跃的串口列表
    QStringList getTtyList() const;
    //!获取波特率列表
    QStringList getBaudList() const;
    //!获取停止位列表
    QStringList getStopList() const;
    //!获取数据位列表
    QStringList getDataList() const;
    //! 获取校验位列表
    QStringList getParityList() const;

    //! 打开串口,返回串口是否打开成功，成功true，否则为false
    bool openTty(const QString &tty,int baud,QString stopBit,
                 QString dataBit,QString parityBit);
signals:
    //! 串口列表信息和当前串口名
    void signalSerialPortListChange(const QStringList &list,QString currentPortName);
private:
    int m_TimerIdSerialPort;
};


#endif //ZTTY_ZSERIALPORT_H
