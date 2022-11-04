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

    virtual ~SingleSend() override;
signals:
    void signalSerialWrite(QByteArray array);
private slots:
    void slotpBnSendClicked();
private:
    Ui::SingleSend *ui;
    ZControl *mpControl;
};


#endif //ZTTY_SINGLESEND_H
