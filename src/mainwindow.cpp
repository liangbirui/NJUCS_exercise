#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , iw_ptr(new Insert())
{
    ui->setupUi(this);

    //加载配置文件
    Json jsonConfig;
    dbPath =jsonConfig.getValue("db");
    theme = jsonConfig.getValue("theme");
    currentId = jsonConfig.getValue("id").toInt();

    qDebug()<<"dbPath is: "<<dbPath<<" \ntheme name is: "<<theme;

    QString styleSheet = loadTheme(theme);
    this->setStyleSheet(styleSheet);

    db_ptr = new Database("main",dbPath);
    model = new QStandardItemModel();

    maxId = db_ptr->getMaxId("data");
    if(currentId<maxId) ui->lineEditId->setText(QString::number(currentId));
    else ui->lineEditId->setText("0");

    ui->comboBoxSubject->addItems(db_ptr->getListData("subject"));
    ui->comboBoxType->addItems(db_ptr->getListData("type"));
    ui->comboBoxLevel->addItems(db_ptr->getListData("level"));
    ui->comboBoxCatalog->addItems(db_ptr->getListData("catalog"));
}

MainWindow::~MainWindow()
{
    delete model;
    delete iw_ptr;
    delete db_ptr;
    delete ui;
}

bool MainWindow::displayDetailById(int id)
{
    m_webViewCtx.clear();
    m_webViewCtx +="<h2>Question</h1>";

    //如果id号溢出则退出
    if(id<0) return false;
    if(id>maxId) return false;

    currentId = id;

    qDebug()<<"Current display id is: "<<currentId;
    m_sql = QString("select id,question from data where id= %1").arg(currentId);
    db_ptr->ptr_query->prepare(m_sql);
    if(db_ptr->ptr_query->exec()){
        while(db_ptr->ptr_query->next()){
            ui->lineEditId->setText(db_ptr->ptr_query->value("id").toString());
            QString context = db_ptr->ptr_query->value("question").toString();
            if(context.isEmpty()) return displayDetailById(++currentId);
            m_webViewCtx += "<h3>"+context +"</h3>";
            ui->webView->setHtml(m_webViewCtx);
        }
    }
    else {
        qDebug()<<"Cannot query detail from data table :"<<db_ptr->ptr_query->lastError();
    }

    return true;
}

QString MainWindow::generateSQL()
{
    m_sql = QString("select id from property ");
    int status =0;

    switch(ui->comboBoxSubject->currentIndex()){
    case 0:
        break;
    case 1:
        m_sql += QString("where subject = '%1'").arg(ui->comboBoxSubject->currentText());
        status =1;
    }

    switch(ui->comboBoxType->currentIndex()){
    case 0:
        break;
    default:
    {
        if(status){
            m_sql += QString("and type = '%1'").arg(ui->comboBoxType->currentText());
        }
        else{
            m_sql += QString("where type = '%1'").arg(ui->comboBoxType->currentText());
        }
    }
    }

    switch(ui->comboBoxLevel->currentIndex()){
    case 0:
        break;
    default:
    {
        if(status){
            m_sql += QString("and level = '%1'").arg(ui->comboBoxLevel->currentText());
        }
        else{
            m_sql += QString("where level = '%1'").arg(ui->comboBoxLevel->currentText());
        }
    }
    }

    switch(ui->comboBoxCatalog->currentIndex()){
    case 0:
        break;
    default:
    {
        if(status){
            m_sql += QString("and catalog = '%1'").arg(ui->comboBoxCatalog->currentText());
        }
        else{
            m_sql += QString("where catalog = '%1'").arg(ui->comboBoxCatalog->currentText());
        }
    }
    }

    QString keywords = ui->lineKeyword->text();
    if(!keywords.isEmpty()){
        m_sql += QString("and keywords like '%%1%'").arg(keywords);
    }

    qDebug()<<"Current sql is: "<<m_sql;

    return m_sql;
}

bool MainWindow::generateList(QString sql)
{
    model->setColumnCount(1);
    model->clear();

    ui->listViewResult->setEditTriggers(QListView::NoEditTriggers);

    db_ptr->ptr_query->prepare(sql);
    if(db_ptr->ptr_query->exec()){
        while(db_ptr->ptr_query->next()){
            model->appendRow(new QStandardItem(db_ptr->ptr_query->value(0).toString()));
        }
    }

    qDebug()<<"Model's row count is: "<<model->rowCount();
    ui->listViewResult->setModel(model);

    return true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)

    QString id = ui->lineEditId->text();
    Json m_json;
    m_json.updateValue("id",id);
    qDebug()<<"Exit now";
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
    ui->lineEditId->setText(QString::number(maxId));
    generateList(generateSQL());
}

void MainWindow::on_actionClear_triggered()
{
    ui->webView->setHtml("Please restart app");
    ui->lineKeyword->clear();
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
    qDebug()<<"Update data in database";

    if(currentId<0 || currentId>maxId){
        QMessageBox::information(this,tr("Update"),tr("Invalid id"),QMessageBox::Ok);
        return;
    }

    iw_ptr->operation(currentId);
    iw_ptr->show();
}

void MainWindow::on_actionAnswer_triggered()
{
    QString msgA = QString("No answer");
    m_sql = QString("select answer from data where id = %1").arg(currentId);
    db_ptr->ptr_query->prepare(m_sql);
    if(db_ptr->ptr_query->exec()){
        while(db_ptr->ptr_query->next()){
            msgA = db_ptr->ptr_query->value(0).toString();
        }
    }

    m_webViewCtx += "<br><h2>Answer</h2><h3>"+msgA+"</h3>";
    ui->webView->setHtml(m_webViewCtx);
}

void MainWindow::on_comboBoxSubject_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    generateList(generateSQL());
}

void MainWindow::on_listViewResult_doubleClicked(const QModelIndex &index)
{
    int id = index.data().toInt();
    qDebug()<<"Current id is: "<<id;
    displayDetailById(id);
}

void MainWindow::on_comboBoxType_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    generateList(generateSQL());
}

void MainWindow::on_comboBoxLevel_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    generateList(generateSQL());
}

void MainWindow::on_lineKeyword_textChanged(const QString &keyword)
{
    if(keyword.isEmpty()) return;

    generateList(generateSQL());
}

void MainWindow::on_actionExport_triggered()
{
    qDebug()<<"Export data now";
    Progress *pro = new Progress();
    pro->setWindowModality(Qt::ApplicationModal);
    pro->show();
}
