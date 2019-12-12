#ifndef BASE_HPP
#define BASE_HPP

#include <QApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QHeaderView>
#include <QIcon>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QMainWindow>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QString>
#include <QUrl>
#include <QWebEngineView>
#include <QWebView>
#include <QWidget>
#include <QtWebKit/QtWebKit>

static QString loadTheme(QString themeName){
    QString themeCtx;
    QFile file(QString(":/resources/themes/%1.qss").arg(themeName));
    qDebug()<<"theme file path is:"<<file.fileName();
    if(file.open(QIODevice::Text|QIODevice::ReadOnly)){
        QTextStream readIn(&file);
        themeCtx = readIn.readAll();
    }
    else{
        qDebug()<<"Cannot open file: "<<file.errorString();
    }
    file.close();

    return themeCtx;
}

#endif // BASE_HPP
