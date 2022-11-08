//
// Created by 11518 on 2022/11/3.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ZSerialPort.h" resolved

#include <QMetaEnum>
#include <QDebug>
#include "zserialport.h"

ZSerialPort::ZSerialPort(QObject *parent) :
        QSerialPort(parent) {
    m_TimerIdSerialPort = startTimer(1500);
//    qDebug()<<this->readBufferSize();
    this->setReadBufferSize(256);
}

ZSerialPort::~ZSerialPort() {

}

QStringList ZSerialPort::getTtyList() const {
    QStringList list;
    QList<QSerialPortInfo> postList = QSerialPortInfo::availablePorts();
    for (auto &var:postList) {
        if(!var.isBusy())
        {
            list.append(var.portName());
            //list.append(var.portName()+":"+var.description());
        }
    }
    return list;
}

QStringList ZSerialPort::getBaudList() const {
    QStringList list;
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::BaudRate>();
    for (int i = 0; i < metaEnum.keyCount(); i++) {
        int value = metaEnum.value(i);
        if(value!=-1)
        {
            list += QString::number(metaEnum.value(i));
        }
        // Or list += metaEnum.key(i);
    }
    return list;
}

QStringList ZSerialPort::getStopList() const {
    QStringList list;
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::StopBits>();
    for (int i = 0; i < metaEnum.keyCount(); i++) {
        int value = metaEnum.value(i);
        if(value!=-1)
        {
            list += metaEnum.valueToKey(value);
        }
    }
    return list;
}

QStringList ZSerialPort::getParityList() const {
    QStringList list;
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::Parity>();
    for (int i = 0; i < metaEnum.keyCount(); i++) {
        int value = metaEnum.value(i);
        if(value!=-1)
        {
            list += metaEnum.valueToKey(value);
        }
    }
    return list;
}

QStringList ZSerialPort::getDataList() const {
    QStringList list;
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::DataBits>();
    for (int i = 0; i < metaEnum.keyCount(); i++) {
        int value = metaEnum.value(i);
        if(value!=-1)
        {
            list += metaEnum.valueToKey(value);
        }
    }
    return list;
}

bool ZSerialPort::openTty(const QString &tty, int baud,
                          QString stopBit, QString dataBit,QString parityBit) {
    this->setPortName(tty);
    this->setBaudRate(baud);
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::StopBits>();
    this->setStopBits(static_cast<StopBits>(metaEnum.keyToValue(stopBit.toStdString().c_str())));

    metaEnum = QMetaEnum::fromType<QSerialPort::DataBits>();
    this->setDataBits(static_cast<DataBits>(metaEnum.keyToValue(dataBit.toStdString().c_str())));

    metaEnum = QMetaEnum::fromType<QSerialPort::Parity>();
    this->setParity(static_cast<Parity>(metaEnum.keyToValue(parityBit.toStdString().c_str())));

    return open(QIODevice::ReadWrite);
}

void ZSerialPort::timerEvent(QTimerEvent *event) {
    QObject::timerEvent(event);
    static QStringList list = this->getTtyList();
    if(this->isOpen()&&this->error()==SerialPortError::NoError){

    }else{
        QStringList listTemp = this->getTtyList();
        QString currentTty = this->portName();
        if (list != listTemp) {
            list = listTemp;
            emit signalSerialPortListChange(list, currentTty);
        }
    }
}
