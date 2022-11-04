//
// Created by 11518 on 2022/11/4.
//

#include "zsettings.h"

ZSettings::ZSettings(const QString &organization, const QString &application, QObject *parent) : QSettings(organization,
                                                                                                           application,
                                                                                                           parent) {

}

ZSettings::ZSettings(const QString &fileName, QSettings::Format format, QObject *parent) : QSettings(fileName, format,
                                                                                                     parent) {

}

ZSettings::~ZSettings() {

}

void ZSettings::setBaudRadioIndex(int index) {
    this->beginGroup("TTY");
    this->setValue("BAUD",index);
    this->endGroup();
}

int ZSettings::getBaudRadioIndex() const {
    return this->value("TTY/BAUD").toInt();
}

void ZSettings::setStopBitIndex(int index) {
    this->beginGroup("TTY");
    this->setValue("STOP",index);
    this->endGroup();
}

int ZSettings::getStopBitIndex() const {
    return this->value("TTY/STOP").toInt();
}

void ZSettings::setDataBitIndex(int index) {
    this->beginGroup("TTY");
    this->setValue("DATA",index);
    this->endGroup();
}

int ZSettings::getDataBitIndex() const {
    return this->value("TTY/DATA").toInt();
}

void ZSettings::setParityBitIndex(int index) {
    this->beginGroup("TTY");
    this->setValue("PARITY",index);
    this->endGroup();
}

int ZSettings::getParityBitIndex() const {
    return this->value("TTY/PARITY").toInt();
}

int ZSettings::getTimeLineState() const {
    return this->value("SHOW/TIME_LINE").toInt();
}

void ZSettings::setTimeLineState(int isChecked) {
    this->beginGroup("SHOW");
    this->setValue("TIME_LINE",isChecked);
    this->endGroup();
}

QString ZSettings::getTextSend() const {
    return this->value("SHOW/SEND_TEXT").toString();
}

void ZSettings::setTextSend(const QString &text) {
    this->beginGroup("SHOW");
    this->setValue("SEND_TEXT",text);
    this->endGroup();
}
