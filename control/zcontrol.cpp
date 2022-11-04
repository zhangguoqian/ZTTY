//
// Created by 11518 on 2022/11/3.
//

#include "zcontrol.h"

ZControl *ZControl::instance() {
    static ZControl instance;
    return &instance;
}

ZControl::~ZControl() {
    delete mpSettings;
    delete mpSerialPort;

}

ZControl::ZControl():mpSerialPort(new ZSerialPort),
                     mpSettings(new ZSettings("zgq","ZCOM")){
}

ZSerialPort *ZControl::getMpSerialPort() const {
    return mpSerialPort;
}

ZSettings *ZControl::getMpSettings() const {
    return mpSettings;
}
