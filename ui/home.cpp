//
// Created by 11518 on 2022/11/3.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Home.h" resolved
#include <QTime>
#include <QMessageBox>
#include <QFileDialog>
#include <QComboBox>
#include <QMetaEnum>
#include <QtConcurrent/QtConcurrent>
#include "home.h"
#include "ui_Home.h"


Home::Home(QWidget *parent) : QWidget(parent),
                              ui(new Ui::Home),
                              mpZControl(ZControl::instance()),
                              m_SendNumber(0),
                              m_RecNumber(0),
                              mpRubberBand(new QRubberBand(QRubberBand::Rectangle, this)){
    ui->setupUi(this);

    m_TimerId = startTimer(1000);

    mpSingleSend = new SingleSend();
    ui->tabWidget->addTab(mpSingleSend, tr("单条发送"));
    connect(mpSingleSend,SIGNAL(signalSerialWrite(QByteArray,bool,bool)),this,SLOT(slotSerialWrite(QByteArray, bool,bool)));
    connect(mpSingleSend,&SingleSend::signalClearSendInfo,[=](){
        this->m_SendNumber = 0;
        ui->label_SendNumber->setText(tr("发送:%0").arg(m_SendNumber));
    });

    mpMultipleSend = new MultipleSend();
    ui->tabWidget->addTab(mpMultipleSend, tr("多条发送"));
    //connect(mpMultipleSend,SIGNAL(signalSerialWrite(QByteArray,bool,bool)),this,SLOT(slotSerialWrite(QByteArray, bool,bool)));

    ui->cbBox_Tty->addItems(mpZControl->getMpSerialPort()->getTtyList());
    connect(ui->cbBox_Tty, SIGNAL(currentTextChanged(const QString &)),this,SLOT(slotTtyNameTextChanged(const QString &)));


    ui->cbBox_Baud->addItems(mpZControl->getMpSerialPort()->getBaudList());
    ui->cbBox_Baud->setCurrentIndex(mpZControl->getMpSettings()->getBaudRadioIndex());
    connect(ui->cbBox_Baud, SIGNAL(currentTextChanged(const QString &)),this,SLOT(slotBaudTextChanged(const QString &)));

    ui->cbBox_Stop->addItems(mpZControl->getMpSerialPort()->getStopList());
    ui->cbBox_Stop->setCurrentIndex(mpZControl->getMpSettings()->getStopBitIndex());
    connect(ui->cbBox_Stop, SIGNAL(currentTextChanged(const QString &)),this,SLOT(slotStopTextChanged(const QString &)));

    ui->cbBox_Parity->addItems(mpZControl->getMpSerialPort()->getParityList());
    ui->cbBox_Parity->setCurrentIndex(mpZControl->getMpSettings()->getParityBitIndex());
    connect(ui->cbBox_Parity, SIGNAL(currentTextChanged(const QString &)),this,SLOT(slotParityTextChanged(const QString &)));

    ui->cbBox_Data->addItems(mpZControl->getMpSerialPort()->getDataList());
    ui->cbBox_Data->setCurrentIndex(mpZControl->getMpSettings()->getDataBitIndex());
    connect(ui->cbBox_Data, SIGNAL(currentTextChanged(const QString &)),this,SLOT(slotDataTextChanged(const QString &)));

    ui->tEdit_Rec->setReadOnly(true);

    ui->ckBox_TimeLine->setChecked(mpZControl->getMpSettings()->getTimeLineState());
    connect(ui->ckBox_TimeLine,&QCheckBox::stateChanged,[=](int state){
        mpZControl->getMpSettings()->setTimeLineState(state);
    });

    connect(mpZControl->getMpSerialPort(), SIGNAL(readyRead()),this,SLOT(slotSerialRead()));
    connect(mpZControl->getMpSerialPort(), SIGNAL(error(QSerialPort::SerialPortError)),this,SLOT(slotSerialError(QSerialPort::SerialPortError)));
    connect(mpZControl->getMpSerialPort(), SIGNAL(signalSerialPortListChange(const QStringList &, QString)),this,SLOT(slotSerialPortListChange(const QStringList &,QString)));
    connect(ui->pBn_TtySet,SIGNAL(clicked()),this,SLOT(slotPBnTtySetClicked()));
    connect(ui->pBn_Clear,&QPushButton::clicked,[=](){
        m_RecNumber = 0;
        ui->label_RecNumber->setText(tr("接收:%0").arg(m_RecNumber));
    });
    connect(ui->pBn_Save,&QPushButton::clicked,[=](){
        QString fileName = QFileDialog::getSaveFileName(this,tr("选择文件夹"),"./","文本(*.txt);;所有文件(*)");
        if(fileName.isEmpty())
        {
            return ;
        }
        QtConcurrent::run([=](){
            QFile file(fileName);
            if(!file.open(QIODevice::WriteOnly))
            {
                QMessageBox::warning(this, tr("提示"),tr("文件保存失败。"));
                return;
            }
            file.write(ui->tEdit_Rec->toPlainText().toLocal8Bit());
            file.close();
        });
    });
    ui->ckBox_HexShow->setChecked(mpZControl->getMpSettings()->getHexFormalRec());
    connect(ui->ckBox_HexShow,SIGNAL(stateChanged(int)),this, SLOT(slotHexShowStateChanged(int)));


}

Home::~Home() {
    mpZControl->getMpSettings()->setHexFormalRec(ui->ckBox_HexShow->isChecked());
    killTimer(m_TimerId);
    delete mpSingleSend;
    delete mpMultipleSend;
    delete ui;
}

void Home::timerEvent(QTimerEvent *event) {
    QObject::timerEvent(event);
    QString currentTimeStr = QTime::currentTime().toString("hh:mm:ss");
    ui->label_CurrentTime->setText(tr("当前时间 %0").arg(currentTimeStr));
}

void Home::slotPBnTtySetClicked() {

    if(mpZControl->getMpSerialPort()->isOpen()){
        ui->pBn_TtySet->setText(tr("打开串口"));
        qDebug()<<mpZControl->getMpSerialPort()->isBreakEnabled();
        return;
    }

    bool info = mpZControl->getMpSerialPort()->openTty(ui->cbBox_Tty->currentText(),ui->cbBox_Baud->currentText().toInt(),
                                                       ui->cbBox_Stop->currentText(),ui->cbBox_Data->currentText(),ui->cbBox_Parity->currentText());

//    mpZControl->getMpSerialPort()->setBreakEnabled(true);
    if(!info){
        QMessageBox::warning(this,tr("提示"),tr("串口打开失败。"));
    }else{
        ui->pBn_TtySet->setText(tr("关闭串口"));
    }

}

void Home::slotTtyNameTextChanged(const QString &ttyName) {
    if(mpZControl->getMpSerialPort()->isOpen()&&mpZControl->getMpSerialPort()->error()==QSerialPort::NoError) {
        mpZControl->getMpSerialPort()->close();
        ui->pBn_TtySet->setText(tr("打开串口"));
        QMessageBox::warning(this,tr("提示"),tr("串口设置失败，不支持的错误操作。"));
    }
    qDebug()<<"串口名:" << ttyName;
}

void Home::slotBaudTextChanged(const QString &text) {
    QComboBox *sender = static_cast<QComboBox *>(this->sender());
    mpZControl->getMpSettings()->setBaudRadioIndex(sender->currentIndex());

    bool info = mpZControl->getMpSerialPort()->setBaudRate(text.toInt());
    if(!info){
        //return;
    }
    qDebug()<<"波特率:" << text;
}

void Home::slotStopTextChanged(const QString &text) {
    QComboBox *sender = static_cast<QComboBox *>(this->sender());
    mpZControl->getMpSettings()->setStopBitIndex(sender->currentIndex());

    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::StopBits>();
    int value = metaEnum.keyToValue(text.toStdString().c_str());
    bool info = mpZControl->getMpSerialPort()->setStopBits(static_cast<QSerialPort::StopBits>(value));
    if(!info){
        //return;
    }
    qDebug()<<"停止位:" << static_cast<QSerialPort::StopBits>(value);
}

void Home::slotParityTextChanged(const QString &text) {
    QComboBox *sender = static_cast<QComboBox *>(this->sender());
    mpZControl->getMpSettings()->setParityBitIndex(sender->currentIndex());

    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::Parity>();
    int value = metaEnum.keyToValue(text.toStdString().c_str());
    bool info = mpZControl->getMpSerialPort()->setParity(static_cast<QSerialPort::Parity>(value));
    if(!info){
        //return;
    }
    qDebug()<<"校验位:" << static_cast<QSerialPort::Parity>(value);
}

void Home::slotDataTextChanged(const QString &text) {
    QComboBox *sender = static_cast<QComboBox *>(this->sender());
    mpZControl->getMpSettings()->setDataBitIndex(sender->currentIndex());

    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::DataBits>();
    int value = metaEnum.keyToValue(text.toStdString().c_str());
    bool info = mpZControl->getMpSerialPort()->setDataBits(static_cast<QSerialPort::DataBits>(value));
    if(!info){

        //return;
    }
    qDebug()<<"数据位:" << static_cast<QSerialPort::DataBits>(value);
}

void Home::slotSerialRead() {
    QSerialPort *pSerialPort = static_cast<QSerialPort*>(sender());
    QByteArray byteArray;
    ui->tEdit_Rec->setTextColor(QColor(Qt::red));
    if(ui->ckBox_TimeLine->isChecked())
    {
        byteArray+=QDateTime::currentDateTime().toString("[yyyy/MM/dd hh:mm:ss.zzz]\nRX:").toLocal8Bit();
    }

    QByteArray recArray = pSerialPort->readAll();
    QString str;
    switch (ui->ckBox_HexShow->checkState()) {
        case Qt::Unchecked:
            str = QString::fromLocal8Bit(recArray.trimmed());
            break;
        case Qt::Checked:
            str = QString::fromLocal8Bit(recArray.toHex(' '));
            break;
    }
    str = QString::fromLocal8Bit(byteArray)+str;
    m_RecNumber+=recArray.size();
    ui->label_RecNumber->setText(tr("接收:%0").arg(m_RecNumber));
    ui->tEdit_Rec->append(str);
}

void Home::slotSerialWrite(QByteArray array,bool isHex,bool isEnter) {
    QByteArray byteArray;
    ui->tEdit_Rec->setTextColor(QColor(Qt::green));
    if(ui->ckBox_TimeLine->isChecked())
    {
        byteArray.append(QDateTime::currentDateTime().toString("[yyyy/MM/dd hh:mm:ss.zzz]\nRX:").toLocal8Bit());
    }
    if(isEnter){
        array.append("\r\n");
    }
    QString str;
    if(!isHex){
        str = QString::fromLocal8Bit(array.trimmed());
    }else{
        str = QString::fromLocal8Bit(array.toHex(' '));
    }


    str = QString::fromLocal8Bit(byteArray)+str;
    qint64 size = mpZControl->getMpSerialPort()->write(array);
    m_SendNumber+=size ;
    ui->label_SendNumber->setText(tr("发送:%0").arg(m_SendNumber));
    ui->tEdit_Rec->append(str);
}

void Home::slotSerialError(QSerialPort::SerialPortError error) {

    if(error!=QSerialPort::NoError)
    {
        mpZControl->getMpSerialPort()->close();
        ui->pBn_TtySet->setText(tr("打开串口"));
        QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::SerialPortError>();
        QString errorStr = metaEnum.valueToKey(error);
        QMessageBox::warning(this,tr("提示"),tr("串口设置失败，不支持的错误操作,错误信息%0。").arg( errorStr));
    }
    qDebug()<<"串口错误信息:" << error;
}

void Home::slotSerialPortListChange(const QStringList &list,QString currentTtyName) {
    ui->cbBox_Tty->clear();
    ui->cbBox_Tty->addItems(list);
    ui->cbBox_Tty->setCurrentText(currentTtyName);
}

void Home::slotHexShowStateChanged(int state) {
    switch (state) {
        case Qt::Unchecked:

            break;
        case Qt::Checked:

            break;
    }
}

void Home::mousePressEvent(QMouseEvent *event) {
    m_Start = event->pos();
    mpRubberBand->setGeometry(QRect(m_Start, QSize()));
    mpRubberBand->show();
    QWidget::mousePressEvent(event);
}

void Home::mouseReleaseEvent(QMouseEvent *event) {
    if(mpRubberBand)
        mpRubberBand->hide();
    QWidget::mouseReleaseEvent(event);
}

void Home::mouseMoveEvent(QMouseEvent *event) {
    if(mpRubberBand)
        mpRubberBand->setGeometry(QRect(m_Start, event->pos()).normalized());
    QWidget::mouseMoveEvent(event);
}