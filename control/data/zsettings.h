//
// Created by 11518 on 2022/11/4.
//

#ifndef ZTTY_ZSETTINGS_H
#define ZTTY_ZSETTINGS_H

#include <QSettings>

class ZSettings : public QSettings{
public:
    explicit ZSettings(const QString &organization,
                       const QString &application = QString(), QObject *parent = nullptr);
    ZSettings(const QString &fileName, Format format, QObject *parent = nullptr);

    //! 设置波特率到settings
    void setBaudRadioIndex(int index);
    //! 获取波特率从settings
    int getBaudRadioIndex() const;
    //! 设置停止位到settings
    void setStopBitIndex(int index);
    //! 获取停止位从settings
    int getStopBitIndex() const;

    //! 设置数据位到settings
    void setDataBitIndex(int index);
    //! 获取数据位从settings
    int getDataBitIndex() const;
    //! 设置校验位到settings
    void setParityBitIndex(int index);
    //! 获取校验位从settings
    int getParityBitIndex() const;

    //! 设置时间轴显示到settings
    void setTimeLineState(int isChecked);
    //! 获取时间轴显示从settings
    int getTimeLineState() const;
    //! 获取发送信息
    QByteArray getTextSend()const;
    //! 设置发送信息
    void setTextSend(const QByteArray &text);
    //! 获取发送HEX
    bool getHexFormalSend()const;
    //! 设置发送HEX
    void setHexFormalSend(bool checked);
    //! 获取周期时间
    int getCycleValue() const;
    //! 设置周期时间
    void setCycleValue(int value);
    //! 获取是否发送新行
    bool getIsSendEnter() const;
    //! 设置是否发送新行
    void setSendEnter(bool icChecked);

    virtual ~ZSettings();
};


#endif //ZTTY_ZSETTINGS_H
