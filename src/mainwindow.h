#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "insert.h"
#include "database.hpp"
#include "json.hpp"
#include "progress.h"

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
    QString generateSQL();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    bool generateList(QString sql);

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

    void on_actionTips_triggered();

    void on_actionAnswer_triggered();

    void on_comboBoxSubject_currentIndexChanged(int index);

    void on_listViewResult_doubleClicked(const QModelIndex &index);

    void on_comboBoxType_currentIndexChanged(int index);

    void on_comboBoxLevel_currentIndexChanged(int index);

    void on_lineKeyword_textChanged(const QString &keyword);

    void on_actionExport_triggered();

private:
    Ui::MainWindow *ui;
    Insert *iw_ptr;
    Database *db_ptr;

    QString m_webViewCtx;
    QString m_sql;
    QString dbPath;
    QString theme;

    QStandardItemModel *model;

    int currentId;
    int maxId;
};
#endif // MAINWINDOW_H
