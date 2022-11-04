//
// Created by 11518 on 2022/11/3.
//

#ifndef ZTTY_MULTIPLESEND_H
#define ZTTY_MULTIPLESEND_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class MultipleSend; }
QT_END_NAMESPACE

class MultipleSend : public QWidget {
Q_OBJECT

public:
    explicit MultipleSend(QWidget *parent = nullptr);

    ~MultipleSend() override;

private:
    Ui::MultipleSend *ui;
};


#endif //ZTTY_MULTIPLESEND_H
