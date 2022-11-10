//
// Created by 11518 on 2022/11/3.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MultipleSend.h" resolved

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

    for (int i = 0; i < 10; ++i) {
        mpCkBoxList<<this->findChild<QCheckBox*>(QString("cBox_%0").arg(i));

    }
    for (int i = 0; i < 10; ++i) {
        mpSendEditList<<this->findChild<QLineEdit*>(QString("lEdit_%0").arg(i));
    }
    for (int i = 0; i < 10; ++i) {
        mpPbnList<<this->findChild<QPushButton*>(QString("pBn_%0").arg(i));
    }

    setCurrentPageData();

    connect(ui->pBn_HomePage,SIGNAL(clicked()),this,SLOT(slotFirstPage()));
    connect(ui->pBn_AddPage,SIGNAL(clicked()),this,SLOT(slotAddPage()));
    connect(ui->pBn_LastPage,SIGNAL(clicked()),this,SLOT(slotLastPage()));
    connect(ui->pBn_NextPage,SIGNAL(clicked()),this,SLOT(slotNextPage()));
    connect(ui->pBn_PrePage,SIGNAL(clicked()),this,SLOT(slotPrePage()));
    connect(ui->pBn_Jump,SIGNAL(clicked()),this,SLOT(slotJumpPage()));
    connect(ui->pBn_DelPage,SIGNAL(clicked()),this,SLOT(slotRemovePage()));
}

MultipleSend::~MultipleSend() {
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
            page[i].second = text.toLocal8Bit();
        });
        connect(mpPbnList[i], &QPushButton::clicked, [=,&page]() {
            emit signalMultipleSerialWrite(page[i].second,ui->ckBox_SendEnter->isChecked());
        });
    }


    for (int i = 0; i < 10; ++i) {
        mpCkBoxList[i]->setChecked(page[i].first);
        mpSendEditList[i]->setText(page[i].second.trimmed());
    }

    ui->spin_Page->setMaximum(m_SumPage);
    ui->label_PageNumber->setText(QString("页码:%1/%2").arg(m_CurrentPageIndex+1).arg(m_SumPage));
}
