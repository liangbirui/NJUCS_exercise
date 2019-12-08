#ifndef JSON_HPP
#define JSON_HPP

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QString>
#include <QDebug>
#include <QDir>

class Json{
private:
    QString jsonFilePath;
    QByteArray allData;

public:
    Json(){
        jsonFilePath = QDir::currentPath() + QDir::separator() + "config.json";
        qDebug()<<"Json file path is: "<<jsonFilePath;
        QFile jsonFile(jsonFilePath);
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

    ~Json(){}

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
};

#endif // JSON_HPP
