/*
 * @Author:幽弥狂
 * @E-mail:1768478912@qq.com
 * @Phone:13812991101
 * @Date:2019-11-15
 * @License:LGPL3
 * @Function:计算MD5值
 * @Description:计算文本，字符串等等的MD5值
 */

#ifndef DATABASE_HPP
#define DATABASE_HPP

#include "base.hpp"

class Database{
public:
    Database(QString name,QString dbPath="exercise.db"){
        //初始化数据库
        m_db = QSqlDatabase::addDatabase("QSQLITE",name);
        m_db.setDatabaseName(dbPath);
        if(m_db.open()){
            qDebug()<<dbPath+" : has been opened successfully";
        }
        else {
            qDebug()<<"Cannot open db: "<<dbPath<<" : "<<m_db.lastError();
        }

        ptr_query = new QSqlQuery(m_db);
    }

    ~Database(){
        m_db.close();
        delete ptr_query;
    }

    int getMaxId(QString tableName){
        int maxId = -1;
        m_sql = QString("select max(id) from %1").arg(tableName);
        ptr_query->prepare(m_sql);
        if(ptr_query->exec()){
            while (ptr_query->next()) {
                maxId = ptr_query->value(0).toInt();
            }
        }
        else{
            qDebug()<<"Cannot query the max of id from table: "<<tableName;
        }
        return maxId;
    }

    QStringList getListData(QString tableName){
        QStringList list;
        if(tableName.isEmpty()){
            return list;
        }

        m_sql.clear();
        m_sql = QString("select value from %1").arg(tableName);
        ptr_query->prepare(m_sql);
        if(ptr_query->exec()){
            while (ptr_query->next()) {
                list.append(ptr_query->value(0).toString());
            }
        }
        else{
            qDebug()<<"Cannot query data from table: "<<tableName;
        }

        return list;
    }

public:
    QString m_sql;
    QSqlQuery *ptr_query;

private:
    QSqlDatabase m_db;
};

#endif // DATABASE_HPP
