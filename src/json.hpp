#ifndef JSON_HPP
#define JSON_HPP

#include "base.hpp"

class Json{
private:
    QString jsonFilePath;
    QByteArray allData;
    QFile jsonFile;

public:
    Json(){
        jsonFilePath = QDir::currentPath() + QDir::separator() + "config.json";
        qDebug()<<"Json file path is: "<<jsonFilePath;

        jsonFile.setFileName(jsonFilePath);
        if(!jsonFile.exists()){
            qDebug()<<"App configuration json file do not exists";
            return;
        }
        if(!jsonFile.open(QIODevice::ReadOnly)){
            qDebug()<<"Couldn't open project configuration json file";
            return;
        }
        allData = jsonFile.readAll();
        jsonFile.close();
    }

    ~Json(){
        jsonFile.close();
    }

    QString getValue(QString key){
        QString value = nullptr;
        QJsonParseError json_error;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(allData,&json_error));
        if(json_error.error !=QJsonParseError::NoError){
            qDebug()<<"json error: "<<json_error.errorString();
            return value;
        }
        QJsonObject rootObj = jsonDoc.object();

        value = rootObj.value(key).toString();

        return value;
    }

    void updateValue(QString key,QString value){
        QJsonParseError json_error;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(allData,&json_error));
        if(json_error.error !=QJsonParseError::NoError){
            qDebug()<<"json error: "<<json_error.errorString();
            return;
        }

        QJsonObject m_jsonObj = jsonDoc.object();

        m_jsonObj[key]= value;

        if(jsonFile.open(QIODevice::WriteOnly|QIODevice::Text)){
            QJsonDocument tempDoc(m_jsonObj);
            jsonFile.write(tempDoc.toJson());
        }
    }
};

#endif // JSON_HPP
