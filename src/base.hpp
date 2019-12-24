#ifndef BASE_HPP
#define BASE_HPP

#include <QApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QHeaderView>
#include <QIcon>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QMainWindow>
#include <QMessageBox>
#include <QObject>
#include <QPixmap>
#include <QPrinter>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QString>
#include <QThread>
#include <QUrl>
#include <QWebEngineView>
#include <QWebView>
#include <QWidget>
#include <QtWebKit/QtWebKit>

inline QString loadTheme(QString themeName){
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

inline void sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

#endif // BASE_HPP
