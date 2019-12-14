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

class Similarity{
private:
    Database *m_db;
    QString m_sql;
public:
    Similarity(){
        m_db = new Database("similarity","/home/jackey/GitHub/NJUCS_exercise/db/md5.db");
    }

    ~Similarity(){}
    //从文本中提取最长的一句话
    QString filterMaxStr(QString ctx){
        if(ctx.isEmpty()) return nullptr;
        QString result;



        return result;
    }

    QString toMD5(QString maxStr){
        if(maxStr.isEmpty()) return nullptr;
        QString result;

        return result;
    }

    bool save2db(QString md5){
        m_sql.clear();
        int maxId = m_db->getMaxId("md5");
        m_sql = QString("insert into md5 values(:id,:value)");
        m_db->ptr_query->prepare(m_sql);
        m_db->ptr_query->bindValue(":id",maxId);
        m_db->ptr_query->bindValue(":value",md5);
        if(m_db->ptr_query->exec()){
            qDebug()<<"Save value: "<<md5<<" into db";
        }
        else{
            qDebug()<<"Cannot insert data into db: "<<m_db->ptr_query->lastError();
        }
        return true;
    }
};

#endif // SIMILARITY_HPP
