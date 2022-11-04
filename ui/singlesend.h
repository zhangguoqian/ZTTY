//
// Created by 11518 on 2022/11/3.
//

#ifndef ZTTY_SINGLESEND_H
#define ZTTY_SINGLESEND_H

#include <QWidget>
#include "../control/zcontrol.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SingleSend; }
QT_END_NAMESPACE

class SingleSend : public QWidget {
Q_OBJECT

public:
    explicit SingleSend(QWidget *parent = nullptr);

protected:
    void timerEvent(QTimerEvent *event) override;

public:

    virtual ~SingleSend() override;
signals:
    void signalSerialWrite(QByteArray array);
private slots:
    //! 点击发送
    void slotpBnSendClicked();
    //! 16进制状态转换
    void slotHexSendStateChanged(int state);
    //! 发送端文本改变
    void slotTextSendChanged();

    //! 循环发送
    void slotTimerSendStateChanged(int state);
private:
    Ui::SingleSend *ui;
    ZControl *mpControl;
    QByteArray m_CurrentArray;
    int m_TimerCycleSend;
};


#endif //ZTTY_SINGLESEND_H
