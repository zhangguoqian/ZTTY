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
    bool getHexFormalSingleSend()const;
    //! 设置发送HEX
    void setHexFormalSingleSend(bool checked);
    //! 获取接收HEX
    bool getHexFormalRec()const;
    //! 设置接收HEX
    void setHexFormalRec(bool checked);
    //! 获取周期时间
    int getCycleValue() const;
    //! 设置周期时间
    void setCycleValue(int value);
    //! 获取是否发送新行
    bool getIsSendEnter() const;
    //! 设置是否发送新行
    void setSendEnter(bool icChecked);
    //! 获取是否开启DTR功能
    bool getDtr() const;
    //! 设置DTR功能开关
    void setDtr(bool icChecked);
    //! 获取是否开启Rts功能
    bool getRts() const;
    //! 设置Rts功能开关
    void setRts(bool icChecked);
    //! 获取多条发送有多少页
    int getSumPage() const;
    //! 设置多条发送有多少页
    void setSumPage(int index);
    //! 获取多条发送有多少页
    int getCurrentPage() const;
    //! 设置多条发送有多少页
    void setCurrentPage(int index);
    //! 获取发送HEX
    bool getHexFormalMultipleSend()const;
    //! 设置发送HEX
    void setHexFormalMultipleSend(bool checked);
    //! 获取周期时间
    int getMultipleCycleValue() const;
    //! 设置周期时间
    void setMultipleCycleValue(int value);
    //! 获取是否发送新行
    bool getIsMultipleSendEnter() const;
    //! 设置是否发送新行
    void setMultipleSendEnter(bool icChecked);
    //! 析构函数
    virtual ~ZSettings();
};


#endif //ZTTY_ZSETTINGS_H
