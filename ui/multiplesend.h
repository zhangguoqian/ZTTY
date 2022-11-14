//
// Created by 11518 on 2022/11/3.
//

#ifndef ZTTY_MULTIPLESEND_H
#define ZTTY_MULTIPLESEND_H

#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include "control/zcontrol.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MultipleSend; }
QT_END_NAMESPACE



class MultipleSend : public QWidget {
Q_OBJECT
signals:
    void signalMultipleSerialWrite(QByteArray array, bool isEnter);
public:
    explicit MultipleSend(QWidget *parent = nullptr);
    ~MultipleSend() override;
private slots:
    void slotFirstPage();
    void slotLastPage();
    void slotNextPage();
    void slotPrePage();
    void slotRemovePage();
    void slotAddPage();
    void slotJumpPage();
    //! 16进制转化
    void slotHexSendStateChanged(int state);
    //! 循环
    void slotTimerSendStateChanged(int state);
private:
    Ui::MultipleSend *ui;
    QVector<QCheckBox*> mpCkBoxList;
    QVector<QLineEdit*> mpSendEditList;
    QVector<QPushButton*> mpPbnList;
    ZControl *mpControl;
    QVector<Page> m_PageDatas;
    int m_CurrentPageIndex;
    int m_SumPage;
    int m_TimerCycleSend;
    void setCurrentPageData();

protected:
    void timerEvent(QTimerEvent *event) override;


};


#endif //ZTTY_MULTIPLESEND_H
