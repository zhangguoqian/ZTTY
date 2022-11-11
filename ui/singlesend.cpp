//
// Created by 11518 on 2022/11/3.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SingleSend.h" resolved

#include <QMessageBox>
#include <QtConcurrent/QtConcurrent>
#include <QFileDialog>
#include <QTimer>
#include "singlesend.h"
#include "ui_SingleSend.h"

SingleSend::SingleSend(QWidget *parent) :
        QWidget(parent), ui(new Ui::SingleSend),mpControl(ZControl::instance()) {
    ui->setupUi(this);

    connect(ui->pBn_Clear,&QPushButton::clicked,[=](){
        ui->tEdit_Send->clear();
        ui->progressBar->setValue(0);
        ui->lineEdit_FileName->clear();
        emit signalClearSendInfo();
    });

    ui->spin_Cycle->setValue(mpControl->getMpSettings()->getCycleValue());
    ui->ckBox_SendEnter->setChecked(mpControl->getMpSettings()->getIsSendEnter());
    ui->ckBox_HexSend->setChecked(mpControl->getMpSettings()->getHexFormalSingleSend());
    m_CurrentArray = mpControl->getMpSettings()->getTextSend();
    switch(ui->ckBox_HexSend->checkState()){
        case Qt::Unchecked:
            ui->tEdit_Send->setText(QString::fromLocal8Bit(m_CurrentArray));
            break;
        case Qt::Checked:
            ui->tEdit_Send->setText(m_CurrentArray.toHex(' '));
            break;
    }
    connect(ui->tEdit_Send, SIGNAL(textChanged()),this,SLOT(slotTextSendChanged()));
    connect(ui->pBn_Send, SIGNAL(clicked()),this,SLOT(slotpBnSendClicked()));
    connect(ui->ckBox_HexSend,SIGNAL(stateChanged(int)),this,SLOT(slotHexSendStateChanged(int)));
    connect(ui->ckBox_Timer,SIGNAL(stateChanged(int)),this,SLOT(slotTimerSendStateChanged(int)));
    connect(ui->pBn_OpenFile,&QPushButton::clicked,this,[=](){
        QString fileName = QFileDialog::getOpenFileName(this,tr("选择文件"),"./","*.txt;;*");
        if(fileName.isEmpty())
        {
            return ;
        }
        ui->lineEdit_FileName->setText(fileName);

    });
    connect(ui->pBn_SendFile,&QPushButton::clicked,[=](){
        if(!mpControl->getMpSerialPort()->isOpen()||mpControl->getMpSerialPort()->error()!=QSerialPort::SerialPortError::NoError)
        {
            QMessageBox::warning(this,tr("提示"),tr("串口没有打开。"));
            return ;
        }
        QFile file(ui->lineEdit_FileName->text());
        if(!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::warning(this,tr("提示"),tr("文件打开失败。"));
            return;
        }
        m_IsSendFile = true;
        m_FileArray = file.readAll();
        file.close();
        QtConcurrent::run([this](){
            int step = 4096;
            QList<QByteArray> byteArrayList;
            QByteArray byteArray;
            int size = m_FileArray.size();
            int quotient = size/step;
            int remainder = size%step;
            for (int i = 0; i < quotient; ++i) {
                byteArrayList << m_FileArray.mid(step*i,step);
            }
            byteArrayList << m_FileArray.mid(step*quotient,remainder);
            for (int i = 0; i < quotient; ++i) {
                if(!m_IsSendFile)
                {
                    break;
                }
                emit signalSerialWrite(byteArrayList[i],ui->ckBox_HexSend->isChecked(),0);
                ui->progressBar->setValue(100*i/quotient);
                QThread::msleep(ui->spin_Cycle->value());
            }
            if(m_IsSendFile){
                emit signalSerialWrite(byteArrayList[quotient],ui->ckBox_HexSend->isChecked(),ui->ckBox_SendEnter->isChecked());
                ui->progressBar->setValue(100);
            }

        });
    });
    connect(ui->pBn_StopSendFile,&QPushButton::clicked,[=](){
        bool temp = m_IsSendFile;
        m_IsSendFile = false;
        if(temp)
            QMessageBox::warning(this,tr("提示"),tr("已停止发送文件。"));
    });
}

SingleSend::~SingleSend() {
    mpControl->getMpSettings()->setTextSend(m_CurrentArray);
    mpControl->getMpSettings()->setHexFormalSingleSend(ui->ckBox_HexSend->isChecked());
    mpControl->getMpSettings()->setSendEnter(ui->ckBox_SendEnter->isChecked());
    mpControl->getMpSettings()->setCycleValue(ui->spin_Cycle->value());
    delete ui;
}

void SingleSend::slotpBnSendClicked() {
    if(!mpControl->getMpSerialPort()->isOpen()){
        QMessageBox::warning(this,tr("提示"),tr("串口没有打开。"));
    }else{
        if(m_CurrentArray.isEmpty())
        {
            return;
        }
        emit signalSerialWrite(m_CurrentArray,ui->ckBox_HexSend->isChecked(),ui->ckBox_SendEnter->isChecked());
    }
}

void SingleSend::slotHexSendStateChanged(int state) {
    switch(state){
        case Qt::Unchecked:
            ui->tEdit_Send->setText(QString::fromLocal8Bit(m_CurrentArray));
            break;
        case Qt::Checked:
            ui->tEdit_Send->setText(m_CurrentArray.toHex(' '));
            break;
    }
}

void SingleSend::slotTextSendChanged() {
    switch(ui->ckBox_HexSend->checkState()){
        case Qt::Unchecked:
            m_CurrentArray = ui->tEdit_Send->toPlainText().toLocal8Bit();
            break;
        case Qt::Checked:
            QRegExpValidator validator;
            QRegExp regExp("[0-0A-Fa-f]");
            if(regExp.exactMatch(ui->tEdit_Send->toPlainText()))
            {
                qDebug()<<"s";
            }
            m_CurrentArray = QByteArray::fromHex(ui->tEdit_Send->toPlainText().toLocal8Bit());
            break;
    }
}

void SingleSend::timerEvent(QTimerEvent *event) {
    QObject::timerEvent(event);
    if(!mpControl->getMpSerialPort()->isOpen()){
        //emit ui->ckBox_Timer->stateChanged(0);
        ui->ckBox_Timer->setChecked(false);
        QMessageBox::warning(this,tr("提示"),tr("串口没有打开。"));
    }else{
        if(m_CurrentArray.isEmpty())
        {
            return;
        }
        emit signalSerialWrite(m_CurrentArray,ui->ckBox_HexSend->isChecked(),ui->ckBox_SendEnter->isChecked());
    }
}

void SingleSend::slotTimerSendStateChanged(int state) {
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
