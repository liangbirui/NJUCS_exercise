#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QDebug>
#include <QString>
#include <QDesktopServices>
#include <QMessageBox>
#include <QUrl>
#include <QWebEngineView>
#include <QWebView>
#include <QtWebKit/QtWebKit>
#include <QMainWindow>
#include <QCloseEvent>

#include "insert.h"
#include "database.hpp"
#include "export.h"
#include "json.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool displayDetailById(int id);

    bool generateList();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_actionExit_triggered();

    void on_actionAbout_triggered();

    void on_actionQt_triggered();

    void on_actionIndex_triggered();

    void on_actionInsert_triggered();

    void on_actionClear_triggered();

    void on_actionPrevious_triggered();

    void on_actionNext_triggered();

    void on_actionDelete_triggered();

    void on_actionUpdate_triggered();

    void on_actionExport_triggered();

    void on_actionTips_triggered();

    void on_actionAnswer_triggered();

private:
    Ui::MainWindow *ui;
    Insert *iw_ptr;
    Database *db_ptr;
    Export *m_export;

    QString m_webViewCtx;
    QString m_sql;
    QString dbPath;
    QString theme;

    int currentId;
    int maxId;
};
#endif // MAINWINDOW_H
