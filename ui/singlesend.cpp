//
// Created by 11518 on 2022/11/3.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SingleSend.h" resolved

#include <QMessageBox>
#include "singlesend.h"
#include "ui_SingleSend.h"


SingleSend::SingleSend(QWidget *parent) :
        QWidget(parent), ui(new Ui::SingleSend),mpControl(ZControl::instance()) {
    ui->setupUi(this);
    ui->tEdit_Send->setText(mpControl->getMpSettings()->getTextSend());
    connect(ui->pBn_Send, SIGNAL(clicked()),this,SLOT(slotpBnSendClicked()));
}

SingleSend::~SingleSend() {
    mpControl->getMpSettings()->setTextSend(ui->tEdit_Send->toPlainText());
    delete ui;
}

void SingleSend::slotpBnSendClicked() {
    if(!mpControl->getMpSerialPort()->isOpen()){
        QMessageBox::warning(this,tr("提示"),tr("串口没有打开。"));
    }else{
        QByteArray array = ui->tEdit_Send->toPlainText().toLocal8Bit();
        if(array.isEmpty())
        {
            return;
        }
        emit signalSerialWrite(array);
    }
}
