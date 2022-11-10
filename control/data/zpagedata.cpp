//
// Created by 11518 on 2022/11/10.
//

#include <QFile>
#include <QDataStream>
#include "zpagedata.h"


bool ZPageData::savePageData(const QVector<Page> &pageDatas,int currentPage,const QString &fileName) {
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        return false;
    }
    QDataStream out(&file);
    out << pageDatas <<currentPage;
    file.close();
    return true;
}

bool ZPageData::openPageData(QVector<Page> &pageDatas,int &currentPage,const QString &fileName) {
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QDataStream in(&file);
    in >> pageDatas >>currentPage;
    file.close();
    return true;
}

