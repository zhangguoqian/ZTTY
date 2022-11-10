//
// Created by 11518 on 2022/11/10.
//

#ifndef ZTTY_ZPAGEDATA_H
#define ZTTY_ZPAGEDATA_H

#include <QByteArray>
#include <QPair>
#include <QVector>

typedef QVector<QPair<bool,QByteArray>> Page;

class ZPageData {
public:
    //! 保存文件到本地
    bool savePageData(const QVector<Page> &pageDatas,int currentPage,const QString &fileName = "data.dat");
    //! 读取文件从本地
    bool openPageData(QVector<Page> &pageDatas,int &currentPage,const QString &fileName = "data.dat");
private:
};


#endif //ZTTY_ZPAGEDATA_H
