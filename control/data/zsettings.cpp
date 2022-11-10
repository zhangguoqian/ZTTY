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

QByteArray ZSettings::getTextSend() const {
    return this->value("SHOW/SEND_TEXT").toByteArray();
}

void ZSettings::setTextSend(const QByteArray &text) {
    this->beginGroup("SHOW");
    this->setValue("SEND_TEXT",text);
    this->endGroup();
}

bool ZSettings::getHexFormalSingleSend() const {
    return this->value("SHOW/SIGNAL_HEX").toBool();
}

void ZSettings::setHexFormalSingleSend(bool checked) {
    this->beginGroup("SHOW");
    this->setValue("SIGNAL_HEX",checked);
    this->endGroup();
}

int ZSettings::getCycleValue() const {
    int cycle =  this->value("SINGLE/CYCLE").toInt();
    cycle=(0==cycle?1000:cycle);
    return cycle;
}

void ZSettings::setCycleValue(int value) {
    this->beginGroup("SINGLE");
    this->setValue("CYCLE",value);
    this->endGroup();
}

bool ZSettings::getIsSendEnter() const {
    return this->value("SINGLE/ENTER").toBool();
}

void ZSettings::setSendEnter(bool icChecked) {
    this->beginGroup("SINGLE");
    this->setValue("ENTER",icChecked);
    this->endGroup();
}

bool ZSettings::getDtr() const {
    return this->value("SHOW/DTR").toBool();
}

void ZSettings::setDtr(bool icChecked) {
    this->beginGroup("SHOW");
    this->setValue("DTR",icChecked);
    this->endGroup();
}

bool ZSettings::getRts() const {
    return this->value("SHOW/RTS").toBool();
}

void ZSettings::setRts(bool icChecked) {
    this->beginGroup("SHOW");
    this->setValue("RTS",icChecked);
    this->endGroup();
}

bool ZSettings::getHexFormalRec() const {
    return this->value("SHOW/REC_HEX").toBool();
}

void ZSettings::setHexFormalRec(bool checked) {
    this->beginGroup("SHOW");
    this->setValue("REC_HEX",checked);
    this->endGroup();
}

int ZSettings::getSumPage() const {
    int index = this->value("MULTIPLE/PAGE").toInt();
    if(index==0)
    {
        index = 1;
    }
    return index;
}

void ZSettings::setSumPage(int index) {
    this->beginGroup("MULTIPLE");
    this->setValue("PAGE",index);
    this->endGroup();
}

int ZSettings::getCurrentPage() const {
    int index = this->value("MULTIPLE/CURRENT").toInt();
    return index;
}

void ZSettings::setCurrentPage(int index) {
    this->beginGroup("MULTIPLE");
    this->setValue("CURRENT",index);
    this->endGroup();
}
