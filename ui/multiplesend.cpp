//
// Created by 11518 on 2022/11/3.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MultipleSend.h" resolved

#include "multiplesend.h"
#include "ui_MultipleSend.h"


MultipleSend::MultipleSend(QWidget *parent) :
        QWidget(parent), ui(new Ui::MultipleSend) {
    ui->setupUi(this);
}

MultipleSend::~MultipleSend() {
    delete ui;
}
