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

    ZPageData *getMpPageData() const;
    ZSettings *getMpSettings() const;
    ZSerialPort *getMpSerialPort() const;
private:
    ZControl();
    ZSerialPort *mpSerialPort;
    ZSettings *mpSettings;
    ZPageData *mpPageData;
};


#endif //ZTTY_ZCONTROL_H
