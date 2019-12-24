/*
 * @Author:幽弥狂
 * @E-mail:1768478912@qq.com
 * @Phone:13812991101
 * @Date:2019-11-15
 * @License:LGPL3
 * @Function:计算文本相似度
 * @Description:筛选文本中最长的5个句子，计算其MD5值，存储至数据库中，
 *              添加新MD5值时先检测是否存在相同的MD5值
 */

#ifndef SIMILARITY_HPP
#define SIMILARITY_HPP

#include "base.hpp"
#include "md5.hpp"
#include "database.hpp"
#include <algorithm>
#include <QtAlgorithms>

class Similarity{
private:
    Database *pDb;
    QString mSQL;
public:
    Similarity(){
        pDb = new Database("similarity","/home/jackey/GitHub/NJUCS_exercise/db/md5.db");
    }

    ~Similarity(){

    }
    //从文本中提取最长的一句话
    QString filterMaxStr(QString ctx){
        if(ctx.isEmpty()) return nullptr;
        qDebug()<<ctx;
        QString result;

        QStringList symList;
        symList<<"。"<<"？"<<"！"<<"，"<<"."<<"?"<<"!"<<",";
        foreach(QString sym,symList){
            ctx.replace(sym," ");
        }

        qDebug()<<"ctx is: "<<ctx;
        QStringList scentenceList = ctx.split(" ");

        int size = 0;
        foreach(QString scentence,scentenceList){
            int temp = scentence.count();
            if(temp>size){
                result=scentence;
            }
        }
        qDebug()<<result;

        return result;
    }

    QString toMD5(QString maxStr){
        if(maxStr.isEmpty()) return nullptr;
        QString result;
        string temp = maxStr.toStdString();
        result = QString::fromStdString(MD5(temp).toStr());

        return result;
    }

    bool save2db(QString md5){
        mSQL.clear();
        int maxId = pDb->getMaxId("md5");
        mSQL = QString("insert into md5 values(:id,:value)");
        pDb->ptr_query->prepare(mSQL);
        pDb->ptr_query->bindValue(":id",maxId);
        pDb->ptr_query->bindValue(":value",md5);
        if(pDb->ptr_query->exec()){
            qDebug()<<"Save value: "<<md5<<" into db";
        }
        else{
            qDebug()<<"Cannot insert data into db: "<<pDb->ptr_query->lastError();
            return false;
        }
        return true;
    }

    bool checkOut(QString context){
        //获取最长子串
        QString maxstr = filterMaxStr(context);
        QString md = toMD5(maxstr);
        return save2db(md);
    }
};

#endif // SIMILARITY_HPP
