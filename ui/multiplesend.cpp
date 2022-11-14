//
// Created by 11518 on 2022/11/3.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MultipleSend.h" resolved

#include <QMessageBox>
#include "multiplesend.h"
#include "ui_MultipleSend.h"


MultipleSend::MultipleSend(QWidget *parent) :
        QWidget(parent), ui(new Ui::MultipleSend) ,mpControl(ZControl::instance()){
    if(mpControl->getMpPageData()->openPageData(m_PageDatas,m_CurrentPageIndex)){
        m_SumPage = m_PageDatas.size();
    }else{
        m_SumPage = 1;
        m_CurrentPageIndex = 0;
        m_PageDatas={Page(10)};
    }

    ui->setupUi(this);

    ui->ckBox_HexSend->setChecked(mpControl->getMpSettings()->getHexFormalMultipleSend());
    ui->ckBox_SendEnter->setChecked(mpControl->getMpSettings()->getIsMultipleSendEnter());
    ui->spin_Cycle->setValue(mpControl->getMpSettings()->getMultipleCycleValue());

    for (int i = 0; i < 10; ++i) {
        mpCkBoxList<<this->findChild<QCheckBox*>(QString("cBox_%0").arg(i));

    }
    for (int i = 0; i < 10; ++i) {
        mpSendEditList<<this->findChild<QLineEdit*>(QString("lEdit_%0").arg(i));
    }
    for (int i = 0; i < 10; ++i) {
        mpPbnList<<this->findChild<QPushButton*>(QString("pBn_%0").arg(i));
        mpPbnList[i]->setText(QString::number(i));
    }

    setCurrentPageData();

    connect(ui->pBn_HomePage,SIGNAL(clicked()),this,SLOT(slotFirstPage()));
    connect(ui->pBn_AddPage,SIGNAL(clicked()),this,SLOT(slotAddPage()));
    connect(ui->pBn_LastPage,SIGNAL(clicked()),this,SLOT(slotLastPage()));
    connect(ui->pBn_NextPage,SIGNAL(clicked()),this,SLOT(slotNextPage()));
    connect(ui->pBn_PrePage,SIGNAL(clicked()),this,SLOT(slotPrePage()));
    connect(ui->pBn_Jump,SIGNAL(clicked()),this,SLOT(slotJumpPage()));
    connect(ui->pBn_DelPage,SIGNAL(clicked()),this,SLOT(slotRemovePage()));

    connect(ui->ckBox_HexSend, SIGNAL(stateChanged(int)),this,SLOT(slotHexSendStateChanged(int)));
    connect(ui->ckBox_AutoCycleSend,SIGNAL(stateChanged(int)),this, SLOT(slotTimerSendStateChanged(int)));
}

void MultipleSend::slotHexSendStateChanged(int state) {
//    Page &page = m_PageDatas[m_CurrentPageIndex];
//    for (int i = 0; i < 10; ++i) {
//        switch(state){
//            case Qt::Unchecked:
//                mpSendEditList[i]->setText(QString::fromLocal8Bit(page[i].second));
//                break;
//            case Qt::Checked:
//                mpSendEditList[i]->setText(QString::fromLocal8Bit(page[i].second.toHex(' ')));
//                break;
//        }
//   }
}

void MultipleSend::slotTimerSendStateChanged(int state) {
    switch (state) {
        case Qt::Unchecked:
            killTimer(m_TimerCycleSend);
            ui->spin_Cycle->setEnabled(false);
            break;
        case Qt::Checked:
            m_TimerCycleSend = startTimer(ui->spin_Cycle->value());
            ui->spin_Cycle->setEnabled(true);
            break;
    }
}

MultipleSend::~MultipleSend() {
    mpControl->getMpSettings()->setHexFormalMultipleSend(ui->ckBox_HexSend->isChecked());
    mpControl->getMpSettings()->setMultipleSendEnter(ui->ckBox_SendEnter->isChecked());
    mpControl->getMpSettings()->setMultipleCycleValue(ui->spin_Cycle->value());


    if(!mpControl->getMpPageData()->savePageData(m_PageDatas,m_CurrentPageIndex)){
        std::remove("data.dat");
    }
    delete ui;
}

void MultipleSend::slotFirstPage() {
    m_CurrentPageIndex = 0;

    setCurrentPageData();
}

void MultipleSend::slotLastPage() {
    m_CurrentPageIndex = m_SumPage-1;
    setCurrentPageData();
}

void MultipleSend::slotNextPage() {
    if(m_CurrentPageIndex==m_SumPage-1)
    {
        return;
    }
    m_CurrentPageIndex++;

    setCurrentPageData();
}

void MultipleSend::slotPrePage() {
    if(m_CurrentPageIndex==0)
    {
        return;
    }
    m_CurrentPageIndex--;

    setCurrentPageData();
}

void MultipleSend::slotRemovePage() {
    if(m_SumPage==1)
    {
        return;
    }
    m_PageDatas.remove(m_CurrentPageIndex);
    m_SumPage--;
    if(m_SumPage==m_CurrentPageIndex)
    {
        m_CurrentPageIndex--;
    }
    setCurrentPageData();
}

void MultipleSend::slotAddPage() {
    m_SumPage++;
    m_PageDatas.append(Page (10));
    m_CurrentPageIndex++;
    setCurrentPageData();
}

void MultipleSend::slotJumpPage() {
    m_CurrentPageIndex = ui->spin_Page->value()-1;

    setCurrentPageData();
}

void MultipleSend::setCurrentPageData() {
    for (int i = 0; i < 10; ++i) {
        disconnect(mpCkBoxList[i], nullptr, nullptr, nullptr);
        disconnect(mpSendEditList[i], nullptr, nullptr, nullptr);
        disconnect(mpPbnList[i], nullptr, nullptr, nullptr);
    }
    Page&page = m_PageDatas[m_CurrentPageIndex];
    //qDebug()<<page <<m_CurrentPageIndex;
    for (int i = 0; i < 10; ++i) {
        connect(mpCkBoxList[i], &QCheckBox::clicked, [=,&page](bool checked) {
            page[i].first = checked;
        });
        connect(mpSendEditList[i], &QLineEdit::textChanged, [=,&page](const QString &text) {
//            switch(ui->ckBox_HexSend->checkState()){
//                case Qt::Unchecked:
//                    page[i].second = text.toLocal8Bit();
//                    break;
//                case Qt::Checked:
//                    page[i].second = QByteArray::fromHex(text.toLocal8Bit());
//                    break;
//            }
        });
        connect(mpPbnList[i], &QPushButton::clicked, [=,&page]() {
            if(!mpControl->getMpSerialPort()->isOpen()){
                QMessageBox::warning(this,tr("提示"),tr("串口没有打开。"));
                return;
            }
            QString str = page[i].second;
            QByteArray array;
            switch(ui->ckBox_HexSend->checkState()){
                case Qt::Unchecked:
                    array = str.toLocal8Bit();
                    break;
                case Qt::Checked:
                    str = str.remove(QRegExp("\\s"));
                    qDebug() << str;
                    array = QByteArray::fromHex(str.toLocal8Bit());
                    break;
            }
            emit signalMultipleSerialWrite( array,ui->ckBox_SendEnter->isChecked());
        });
    }

    for (int i = 0; i < 10; ++i) {
        mpCkBoxList[i]->setChecked(page[i].first);
        mpSendEditList[i]->setText(page[i].second.trimmed());
    }

    ui->spin_Page->setMaximum(m_SumPage);
    ui->label_PageNumber->setText(QString("页码:%1/%2").arg(m_CurrentPageIndex+1).arg(m_SumPage));
}

void MultipleSend::timerEvent(QTimerEvent *event) {
    QObject::timerEvent(event);
    if(!mpControl->getMpSerialPort()->isOpen()){
        //emit ui->ckBox_Timer->stateChanged(0);
        ui->ckBox_AutoCycleSend->setChecked(false);
        QMessageBox::warning(this,tr("提示"),tr("串口没有打开。"));
    }else{
        QString str;
        QByteArray array;
        for (auto &vars:m_PageDatas) {
            for(auto &var:vars){
                if(var.first){
                    str = var.second;
                    switch(ui->ckBox_HexSend->checkState()){
                        case Qt::Unchecked:
                            array = str.toLocal8Bit();
                            break;
                        case Qt::Checked:
                            str = str.remove(QRegExp("\\s"));
                            array = QByteArray::fromHex(str.toLocal8Bit());
                            break;
                    }
                    emit signalMultipleSerialWrite( array,ui->ckBox_SendEnter->isChecked());
                }
            }
        }
//        if(m_CurrentArray.isEmpty())
//        {
//            return;
//        }
//        emit signalSerialWrite(m_CurrentArray,ui->ckBox_HexSend->isChecked(),ui->ckBox_SendEnter->isChecked());
    }
}
