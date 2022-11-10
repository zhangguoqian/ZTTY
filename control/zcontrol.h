//
// Created by 11518 on 2022/11/3.
//

#ifndef ZTTY_ZCONTROL_H
#define ZTTY_ZCONTROL_H

#include <QObject>
#include <QDebug>
#include "serialport/zserialport.h"
#include "data/zsettings.h"
#include "data/zpagedata.h"

class ZControl : public QObject{
    Q_OBJECT
public:
    static ZControl *instance();
    virtual ~ZControl();
private:
    ZControl();
    ZSerialPort *mpSerialPort;
    ZSettings *mpSettings;
    ZPageData *mpPageData;
public:
    ZPageData *getMpPageData() const;

public:
    ZSettings *getMpSettings() const;

public:
    ZSerialPort *getMpSerialPort() const;
};


#endif //ZTTY_ZCONTROL_H
