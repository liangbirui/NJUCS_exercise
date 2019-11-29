#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , iw_ptr(new Insert())
{
    ui->setupUi(this);
    loadConfig();

    this->setStyleSheet(theme);
    db_ptr = new Database("main",dbPath);

    maxId = db_ptr->getMaxId("data");
    ui->lineEditId->setText(QString::number(currentId));
    ui->comboBoxSubject->addItems(db_ptr->getListData("subject"));

    displayDetailById(currentId);
}

MainWindow::~MainWindow()
{
    delete iw_ptr;
    delete db_ptr;
    delete ui;
}

void MainWindow::loadConfig()
{
    QString jsonFilePath = QDir::currentPath() + QDir::separator() + "config.json";
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
    QByteArray allData = jsonFile.readAll();
    jsonFile.close();
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData,&json_error));
    if(json_error.error !=QJsonParseError::NoError){
        qDebug()<<"json error: "<<json_error.errorString();
        return;
    }
    QJsonObject rootObj = jsonDoc.object();

    dbPath = rootObj.value("db").toString();
    theme = rootObj.value("theme").toString();
    currentId = rootObj.value("id").toInt();

    qDebug()<<"dbPath is: "<<dbPath<<" \nsrc path is: "<<theme;
}

bool MainWindow::displayDetailById(int id)
{
    //如果id号溢出则退出
    if(id<0) return false;
    if(id>maxId) return false;

    //先进行关键词过滤
    QString filter = ui->plainTextCata->toPlainText();
    QString keywords;
    if(!filter.isEmpty()){
        qDebug()<<"Filter data";
        m_sql = QString("select keywords from property where id = %1").arg(currentId);
        db_ptr->ptr_query->prepare(m_sql);
        if(db_ptr->ptr_query->exec()){
            while(db_ptr->ptr_query->next()){
                keywords = db_ptr->ptr_query->value(0).toString();
            }
        }

        QStringList filterList = filter.split(" ");
        QStringList keywordsList = keywords.split(" ");
        for(QString filterWord : filterList){
            for(QString keyword : keywordsList){
                if(keyword == filterWord) return displayDetailById(id+1);
            }
        }
        //过滤结束
    }

    qDebug()<<"Current display id is: "<<currentId;
    m_sql = QString("select id,question from data where id= %1").arg(currentId);
    db_ptr->ptr_query->prepare(m_sql);
    if(db_ptr->ptr_query->exec()){
        while(db_ptr->ptr_query->next()){
            ui->webView->setHtml(db_ptr->ptr_query->value("question").toString());
            ui->lineEditId->setText(db_ptr->ptr_query->value("id").toString());
        }
    }
    else {
        qDebug()<<"Cannot query detail from data table :"<<db_ptr->ptr_query->lastError();
    }

    return true;
}


void MainWindow::on_actionExit_triggered()
{
    qApp->exit(0);
}

void MainWindow::on_actionAbout_triggered()
{
    QString msg=QString("<h1>NJUCS-exercise</h1>"
                        "<h2>NJUCS配套的刷题软件</h2>"
                        "<h3>作者：幽弥狂</h3>"
                        "<h3>联系方式：13812991101</h3>"
                        "<h3>邮件：1768478912@qq.com</h3>"
                        "<h3>QQ:1768478912</h3>"
                        "<h4>NJUCS: <a href=\"https://github.com/JackeyLea/NJUCS\">NJUCS</a></h4>"
                        "<h4>NJUCS-exercise: <a href=\"https://github.com/JackeyLea/NJUCS-exercise\">NJUCS-exercise</a></h4>"
                        "<h4>本软件使用LGPL3授权，请遵循开源社区的规则</h4>");

    QMessageBox::information(this,tr("About"),msg,QMessageBox::Ok);
}

void MainWindow::on_actionQt_triggered()
{
    qApp->aboutQt();
}

void MainWindow::on_actionIndex_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/JackeyLea/Warehouse"));
}

void MainWindow::on_actionInsert_triggered()
{
    iw_ptr->show();

    maxId = db_ptr->getMaxId("data");
}

void MainWindow::on_buttonTips_clicked()
{
    QString msg = QString("No tip");
    m_sql = QString("select tips from data where id = %1").arg(currentId);
    db_ptr->ptr_query->prepare(m_sql);
    if(db_ptr->ptr_query->exec()){
        while(db_ptr->ptr_query->next()){
            msg = db_ptr->ptr_query->value(0).toString();
        }
    }

    iw_ptr->display("Tips",msg);
    iw_ptr->show();
}

void MainWindow::on_buttonAnswer_clicked()
{
    QString msg = QString("No answer");
    m_sql = QString("select answer from data where id = %1").arg(currentId);
    db_ptr->ptr_query->prepare(m_sql);
    if(db_ptr->ptr_query->exec()){
        while(db_ptr->ptr_query->next()){
            msg = db_ptr->ptr_query->value(0).toString();
        }
    }

    iw_ptr->display("Answer",msg);
    iw_ptr->show();
}

void MainWindow::on_actionClear_triggered()
{
    ui->webView->setHtml("Please restart app");
    ui->plainTextCata->clear();
}

void MainWindow::on_actionPrevious_triggered()
{
    if(currentId<=1){
        qDebug()<<"Going to the latest question";
        currentId = maxId;
    }
    else {
        currentId--;
    }

    displayDetailById(currentId);
}

void MainWindow::on_actionNext_triggered()
{
    if(currentId>=maxId){
        currentId = 1;
    }
    else {
        currentId++;
    }

    displayDetailById(currentId);
}

void MainWindow::on_actionDelete_triggered()
{
    if(currentId<0 || currentId>maxId){
        QMessageBox::information(this,tr("Deletion"),tr("Invalid id"),QMessageBox::Ok);
        return;
    }

    m_sql = QString("delete from data where id = %1").arg(currentId);
    db_ptr->ptr_query->prepare(m_sql);
    if(db_ptr->ptr_query->exec()){
        QMessageBox::information(this,tr("Deletion"),tr("Deleted"),QMessageBox::Ok);
    }
    else{
        QMessageBox::information(this,tr("Deletion"),QString("Cannot delete from data: %1").arg(db_ptr->ptr_query->lastError().text()),QMessageBox::Ok);
    }
}

void MainWindow::on_actionUpdate_triggered()
{
    if(currentId<0 || currentId>maxId){
        QMessageBox::information(this,tr("Update"),tr("Invalid id"),QMessageBox::Ok);
        return;
    }
}
